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

// Share for signalling to check home
extern Share<bool> check_home_share;

// Queue that holds read character arrays
extern Queue<char[LINE_BUFFER_SIZE]> read_chars_queue;

// Share for timing mode
extern Share<uint8_t> timing_mode_share;

// ========================================  Class: coreXY_to_AB ========================================

coreXY_to_AB::coreXY_to_AB(void)
{
    //Nothing to do; values in structs are all reset on initiation
}

/** @brief      Function which translates XYSF into AB coordinates and sends ramp coefficients to the queue
 *  @details    This function runs the kinematics functions to take the XYSF values from the Gcode interpreter, transform
 *              them into the corresponding ramp coefficients, and put the ramp coefficients into the queue. 
 */
void coreXY_to_AB::translate_to_queue(XYSFvalues XYSF_input)
{
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

    // Calculate A and B starting positions (same units as X and Y; mm in this case)
    _ramp_coeff.pos_A0 =   _last_XYSF.X - _last_XYSF.Y;
    _ramp_coeff.pos_B0 = - _last_XYSF.X - _last_XYSF.Y;

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





// ========================================= Task: task_translate =========================================


/** @brief      Task which reads data from the serial port, translates it, and sends it where it needs to go.
 *  @details    This task function reads a line from the read_chars queue of gcode or other commands 
 *              and splits it up into the separate commands. Commands are then sent to the control task via queues
 *              and shares. Gcode is translated into @c X @c Y @c S and @c F values by the decoder class
 * 
 *  @param      p_params A pointer to function parameters which we don't use.
 */
// void task_translate(void* p_params)
// {
//     (void)p_params;                   // Does nothing but shut up a compiler warning
//
//     //Initialize translator and decoder class members 
//     coreXY_to_AB translator;
//     decode decoder;
//
//     //Create line char array to hold incoming data from the @c read_chars_queue
//     char line[LINE_BUFFER_SIZE];
//
//     //Create empty machine_cmd state variable
//     uint8_t machine_cmd = MACHINE_CMD_NULL;
//
//     //Reset homing_flag to false
//     check_home_share.put(false);
//
//     for(;;)
//     {   
//         //If we have space in the queues for A and B positions and feedrates...
//         // if 
//
//         //If there is a read line of gcode/commands in the queue...
//         if (read_chars_queue.any())
//         {
//             //Read the line
//             read_chars_queue.get(line);
//
//             //if the line is actually a machine command:
//             if (line[0] == '$')
//             {
//                 machine_cmd = interpret_machinecmd_line(line);
//                 switch (machine_cmd)
//                 {
//                     //Error in command: Command not supported
//                     case MACHINE_CMD_NULL:
//                         //Print an error?
//                         break;
//                     //Go into homing cycle
//                     case MACHINE_CMD_HOME:
//                         check_home_share.put(true);
//                         break;
//                     default:
//                         //Shouldn't get here, hopefully
//                         break;
//                 };
//             }
//             //If we didn't get into a machine command, it must be a gcode command!
//             else
//             {
//                 //Interpret the line as gcode (data is stored in decoder class)
//                 decoder.interpret_gcode_line(line);
//                 //update kinematic translator queue with new values
//                 translator.translate_to_queue(decoder.get_XYSF());
//
//             }
//         }//if(read_chars.any())
//         vTaskDelay(100);
//     }// for loop
// }

void task_translate(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning

    //Initialize translator and decoder class members 
    coreXY_to_AB translator;
    decode decoder;

    //Create line char array to hold incoming data from the @c read_chars_queue
    char line[LINE_BUFFER_SIZE];

    //Create empty machine_cmd and gcode_cmd state variables
    uint8_t machine_cmd = MACHINE_CMD_NULL;
    uint8_t gcode_cmd = GC_CMD_NULL;

    //Check if there is space in ramp coeff queue
    bool ramp_q_full = false;

    //Main states of function
    uint8_t translate_state = TRANSLATE_STATE_NORMAL_OPERATION;

    for(;;)
    {   
        //At the beginning of each loop, check to see if we should pause:
        //(insert code here)

        //Switch case for the main states of the translate task
        switch(translate_state)
        {
            case TRANSLATE_STATE_NORMAL_OPERATION:
                //Read a line, interpret it, and move from there.

                //If there is a line in the read_chars_queue...
                if (read_chars_queue.any())
                {
                    //Read the line
                    read_chars_queue.get(line);

                    //if the line is actually a machine command:
                    if (line[0] == '$')
                    {
                        machine_cmd = decoder.interpret_machinecmd_line(line);
                        switch (machine_cmd)
                        {
                            //Go into homing cycle
                            case MACHINE_CMD_HOME:
                                translate_state = TRANSLATE_STATE_HOMING;
                                break;

                            //Error in command: Command not supported
                            case MACHINE_CMD_NULL:
                            default:
                                //Print an error?
                                break;
                        };
                    }

                    else    //If we didn't get into a machine command, it must be a gcode command!
                    {
                        // Interpret the line as gcode (data is stored in decoder class)
                        gcode_cmd = decoder.interpret_gcode_line(line);

                        switch(gcode_cmd)
                        {
                            case GC_CMD_UPDATE_XYSF:
                                // Update XYSF values in the queue
                                ramp_q_full = true;
                                while(ramp_q_full)      //If queue isn't full, we get out of the loop and translate. Otherwise, we pause and check again. 
                                {
                                    if(ramp_segment_coefficient_queue.available() < RAMP_COEFF_Q_SIZE - RAMP_COEFF_Q_PAUSE_LIMIT)   //There is space in the queue
                                    {
                                        ramp_q_full = false;
                                    }
                                    else    //No space in the queue; can't translate just yet. Initiate task delay so we can continue
                                    {
                                        vTaskDelay(TRANSLATE_TASK_TIMING);
                                    }  
                                }
                                // Once we're here (out of the while loop), we know that there's space in the queue; ok to translate to it. 
                                translator.translate_to_queue(decoder.get_XYSF());

                                break;
                            
                            case GC_CMD_HOME:
                                //Go into homing state
                                translate_state = TRANSLATE_STATE_HOMING;
                                break;
                            
                            case GC_CMD_END_PROGRAM:
                                //Somehow signal to python that we're done with the gcode...
                                break;

                            case GC_CMD_ERROR:
                                //Get and print error message?

                            case GC_CMD_NULL:
                            default:
                                //Print an error?
                                break;
                        }//switch(gcode_cmd)
                    }//reading gcode commands
                }//if(read_chars.any())
                break;  //case TRANSLATE_STATE_NORMAL_OPERATION
            

            case TRANSLATE_STATE_HOMING:
                //Send the commands to home the machine
                check_home_share.put(true);
                break;


            case TRANSLATE_STATE_PAUSED:
                //Send the commands to brake the motors and cut PWM signal to laser; send message to printer
                break;
        }

        vTaskDelay(TRANSLATE_TASK_TIMING);
    }// for loop
}//task_translate


// // ========================================= Task: task_translate_test =========================================

// /** @brief      Task to test using the translator for converting XYSF points into ramp coefficients. 
//  *  @details    This task only runs the final part of the translate task, which converts XYSF points into ramp 
//  *              coefficients to be used by the controller. The full task will convert Gcode into @c X @c Y @c S 
//  *              and @c F coefficients first, then convert those coeffiients into ramp coefficients.
//  * 
//  *  @param      p_params A pointer to function parameters which we don't use.
//  */


// void task_translate_test(void* p_params)
// {
//     (void)p_params;                   // Does nothing but shut up a compiler warning

//     coreXY_to_AB gcode_translator;

//     //Hard code points
//     XYSFvalues point1;      XYSFvalues point2;      XYSFvalues point3;
//     point1.X = 0;           point2.X = 5;           point3.X = 0;
//     point1.Y = 0;           point2.Y = 5;           point3.Y = 0;


//     gcode_translator.translate_to_queue(point1);
//     gcode_translator.translate_to_queue(point2);
//     gcode_translator.translate_to_queue(point3);

//     for(;;)
//     {
        
//         vTaskDelay(TRANSLATE_TASK_TIMING);
//     }

// }



// ======================================== Subfunctions ========================================