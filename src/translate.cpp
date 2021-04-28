/** @file       translate.cpp
 *  @brief      This file contains the code necessary to translate fully from position endpoints and velocities set by Gcode 
 *              to ramp functions that represent motor movement. It takes the place of files motionplanning.cpp, 
 *              motionplanning.h, kinematics.cpp, and kinematics.h, rendering those files obselete. 
 *
 *  @author  Niko Banks
 *  @date    4-26-2021 File Created
 *  @date    4-26-2021 Last Updated
 * 
*/

#include "libraries&constants.h"


// ============================================  Shares and queues ============================================

// Share for ramp segment coefficients: Coefficients for both motors are passed in a struct from the kinematic translation
// to the ramp translation.
extern Queue<ramp_segment_coefficients> ramp_segment_coefficient_queue;






// ========================================  Class: coreXY_to_AB ========================================

coreXY_to_AB::coreXY_to_AB(void)
{
    //Nothing to do; values in structs are all reset on initiation
}

/** @brief      Function which translates XYSF into AB coordinates and sends ramp coefficients to the queue
 *  @details    This function runs the kinematics functions to take the XYSF values from the Gcode interpreter, transform
 *              them into the corresponding ramp coefficients, and put the ramp coefficients into the queue. 
 */
void coreXY_to_AB::translate_to_queue(decode decoder)
{
    //Get the XYSF values from the class
    XYSFvalues XYSF_input = decoder.get_XYSF();

    //Translate XYSF values into ramp coefficients
    ramp_segment_coefficients ramp_coeff = calc_ramp_coeff(XYSF_input);

    //Put ramp coefficients into the queue
    ramp_segment_coefficient_queue.put(ramp_coeff);
}



/** @brief      Function which runs the kinematics functions in succession
 *  @details    This function runs the kinematics functions to take a new X Y and F value from the 
 *              Gcode interpreter and convert them into the motor A and B positions and velocities. 
 */
ramp_segment_coefficients coreXY_to_AB::calc_ramp_coeff(XYSFvalues XYSF_input)
{
    // Reference for CoreXY Directions, as defined with out system:
    // A and B directions are + CCW
    // delta_A =  delta_X - delta_Y             delta_X = 1/2*( delta_A - delta_B)
    // delta_B = -delta_X - delta_Y             delta_Y = 1/2*(-delta_A - delta_B)

    // Calculate A and B positions (same units as X and Y; mm in this case)
    _ramp_coeff.pos_A0 =   XYSF_input.X - XYSF_input.Y;
    _ramp_coeff.pos_B0 = - XYSF_input.X - XYSF_input.Y;

    // Calculate velocities:
    float magnitude = sqrt( pow(XYSF_input.X - _last_XYSF.X,2) + pow(XYSF_input.Y - _last_XYSF.Y,2) );
        //   Vx = (dx/L)*V
    float vel_x = (XYSF_input.X - _last_XYSF.X) / magnitude * XYSF_input.F;
    float vel_y = (XYSF_input.Y - _last_XYSF.Y) / magnitude * XYSF_input.F;

    _ramp_coeff.vel_A =  vel_x - vel_y;
    _ramp_coeff.vel_B = -vel_x - vel_y;

    //Reset start time to last end time
    _ramp_coeff.t0 = _ramp_coeff.t_end;

    //Calculate new end time (time it takes to complete ramp: t_end = t0 + (L/V)
    _ramp_coeff.t_end = _ramp_coeff.t0 + (magnitude / XYSF_input.F);

    //Update S value
    _ramp_coeff.S = XYSF_input.S;

    //Update _last_XYSF with new value
    _last_XYSF = XYSF_input;

    return _ramp_coeff;
}



/** @brief      Reset all class member data inside coreXY_to_AB class
 *  @details    This function resets all class member data for @c _ramp_coeff and @c _last_XYSF. 
 */
void coreXY_to_AB::reset(void)
{
    // Reset _ramp_coeff
    _ramp_coeff.t0     = 0;             _ramp_coeff.pos_A0 = 0;             _ramp_coeff.vel_A  = 0;
    _ramp_coeff.t_end  = 0;             _ramp_coeff.pos_B0 = 0;             _ramp_coeff.vel_B  = 0;
    _ramp_coeff.S      = 0;

    //Reset _last_XYSF
    _last_XYSF.X = 0;           _last_XYSF.F = 0;
    _last_XYSF.Y = 0;           _last_XYSF.S = 0;
}







// =========================================  Class: setpoint_of_time =========================================


/** @brief      Constructor for the setpoint_of_time class
 *  @details    This function creates the setpoint_of_time class object. 
 */
setpoint_of_time::setpoint_of_time(void)
{
    //Don't do anything with the constructor 
}



/** @brief      Function which returns the current desired setpoint calculated at the time input
 *  @details    This function takes an input of @c time, the time at which the setpoint is to be returned for. The function 
 *              then checks to see if the current held coeffifients for the ramp are still valid for the time requested; if 
 *              the time is not valid, a new set of coefficients is obtained from the queue @c ramp_segment_coefficient_queue 
 *              and then the new coefficients are checked for validity. This is repeated until either the coeffiecients are
 *              found to be valid, at which point the setpoint is calculated and returned; or, if the 
 *              @c ramp_segment_coefficient_queue is empty, the function returns a velocity of 0 and holds the last ending 
 *              position of the last set of coefficients. 
 */
motor_setpoint setpoint_of_time::get_desired_pos_vel(float time)
{
    //Figure out which segment of the desired curve that we're looking at based on the time we're given; 
    //that will determine what the constants we can use are
    
    //Create instance of struct containing setpoints
    motor_setpoint setpoint;

    bool checking_coefficients = true;

    //Find out which segment we're looking at
    while(checking_coefficients)    //Loop used to continuously check the coefficients until we find a set that is within our time range
    {
        if(time > _seg_coeff.t_end)     //We've passed the end of the current ramp segment; we may need to update coefficients
        {
            if(ramp_segment_coefficient_queue.is_empty())
            {
                //If there are no new coefficients available, set positions to the final desired position and then 
                //set velocities to 0 to keep the position steady
                _seg_coeff.pos_A0 = _seg_coeff.vel_A * (_seg_coeff.t_end - _seg_coeff.t0)  + _seg_coeff.pos_A0;
                _seg_coeff.pos_B0 = _seg_coeff.vel_B * (_seg_coeff.t_end - _seg_coeff.t0)  + _seg_coeff.pos_B0;

                _seg_coeff.vel_A = 0;
                _seg_coeff.vel_B = 0;

                //Get us out of the checking loop
                checking_coefficients = false;
            }
            else //We have new coefficients in the queue; get it and replace the old _seg_coeff with this new set.
            {
                ramp_segment_coefficient_queue.get(_seg_coeff);
            }
        }
        else //If time <= t_end of the current segment, we don't have to change the segment coefficients. 
        {
            checking_coefficients = false;
        }
    } //while(checking_coefficients)

    //Caclulate desired position
    // A(t)  =  vel_A*(t-t0) + A0
    setpoint.A_pos = _seg_coeff.vel_A * (time - _seg_coeff.t0)  + _seg_coeff.pos_A0;
    setpoint.B_pos = _seg_coeff.vel_B * (time - _seg_coeff.t0)  + _seg_coeff.pos_B0;

    //Send desired velocity to the output struct
    // setpoint.A_vel = _seg_coeff.vel_A;
    setpoint.A_vel = _seg_coeff.vel_A;
    setpoint.B_vel = _seg_coeff.vel_B;

    return setpoint;
}







