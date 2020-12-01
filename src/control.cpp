/** @file control.cpp
 *    This file contains the source code for a class that implements a P, PI, or PID motor control
 *    code to drive the motors.
 * 
 *  @author Matthew Carlson
 *  @date  2020-Nov-10 Original file
 *  @date 2020-Nov-11 Added control loop drafts
 *  @date 2020-Nov-13 Added set and get methods for gains
 *  @date 2020-Nov-14 Added comments after talking with Ethan
 *  @date 2020-Nov-15 Working on adding in the Velocity and Position inputs
 *  @date 2020-Nov-17 Testing using a single line of GCode, fixed and cleaned up variables
 *  @date 2020-Nov-20 Cleaned up old comments and organized the code, it shoudl now be ready for integration and testing
 */


/** @brief   Class which implements a P, PI, or PID controller for a motor control
 *  @details Sets up the P, PI, and PID controller classes and sets up the methods 
 *           for each of these controller classes. 
 *  
 */


#include "libraries&constants.h"


// ------ General Questions about the code ------

// ADD OVERALL SATURATION LIMIT!!!! To output


// I've set everything as floats, but maybe some should be double's or int's?

// How's our timer going to work?
// timing, potentially use Micros or Millis
// Potentially use VTaskDelayUntil

// Should we include the "output_filter" and "output_PWM" variables as inputs so we can tune them easier?

// Do I need to have the shares and queues in this file or will then just be in the "main" and be inputs?

// What should we be calling the output of the control loop? "output"? "setpoint_new"?

// How to give the output from the control loop to back to the motor? Shares/queues?
// The output is 0-100% 

// Will the "setpoint" variable need to be a 1X2 array? setpoint_P: [x, y]?
// Or will we just have 5 instances of Controller_PID, motor1_x, motor1_y, motor2_x, motor2_y, laser_power
// Do I need to call it "setpoint_P"? ... I don't think I need to cause it's protected within this class, right?

// The control loops will need to control both motors, x and y for each. In order to achieve an [X, Y]
// position, both motors will need to be moving at the same time


// ------ General Notes about the code ------

// Look at the final "output" to make sure it is indeed giving a PWM signal to the motor






 // ------------ PID Controller ------------ //

/** @brief   Creates a PID controller to drive a motor
 *  @details This constructor sets up the PID controller. It does this
 *           by taking in two gain values and a setpoint (gotten from the GCode). It reads
 *           the actual position from the encoder output, compares this to the theoretical
 *           position from the setpoint (initially from the GCode), computes an error value 
 *           and updates the setpoint (motor command) to minimize the error.
 *           It also will have functions that allow the gain and setpoint protected variables
 *           to be changed by either the user or by the GCode.
 *  @param   kP_gain This is the parameter for the input gain for the Proportional control
 *  @param   kI_gain This is the parameter for the input gain for the Integral control
 *  @param   kD_gain This is the parameter for the input gain for the Derivative control
 *  @param   pos_desired_initial This is the initial value of the postion gotten from the GCode
 *  @param   pos_actual_initial This is the initial value of the postion gotten from the Encoder
 *  @param   vel_desired_initial This is the initial value of the velocity gotten from the GCode
 *  @param   vel_actual_initial This is the initial value of the velocity gotten from the Encoder
 *  @param   delta_time_initial This is the initial value of the delta time gotten from the Encoder
 */

Controller_PID::Controller_PID (float kP, float kI, float kD, float pos_desired_initial, float pos_actual_initial, float vel_desired_initial, float vel_actual_initial, float delta_time_initial)
{
    // Save the parameter and initialize it as "gain_KP" 
    gain_kP = kP;

    // Save the parameter and initialize it as "gain_KI" 
    gain_kI = kI;

    // Save the parameter and initialize it as "gain_KD" 
    gain_kI = kD;

    // From the GCode: "Desired" (setpoint) (This is going to be some sort of GCode_Share value)
    pos_desired = pos_desired_initial;       // Save the parameter and initialize it
    vel_desired = vel_desired_initial;       // Save the parameter and initialize it

    // From the Encoder: "Actual" (feedback) (This is going to be some sort of Encoder_Share value)
    pos_actual = pos_actual_initial;         // Save the parameter and initialize it
    
    vel_actual = vel_actual_initial;         // Save the parameter and initialize it

    delta_time = delta_time_initial;         // Save the parameter and initialize it

    // Parameters that are used in this class specifically

   //  pos_actual_current = 0;
   //  vel_actual_current = 0;

    pos_last = 0;
    vel_last = 0;
    
   //  current_time = 10;
   //  last_time = 0;

    pos_error = 2;
    pos_error_last = 0;

    vel_error = 0;
    vel_error_last = 0;


    integral_cumulation = 0;
    integral_cumulation_max = 1000;           // Set this number when we have a better understanding of the device parameters

    output_filter = 1;                      // Set this number when we have a better understanding of the device parameters
    output_PWM = 1;                          // Set this number so that the units work out for outputing a PWM signal
    
    output = 0;                              // This is the value of the final output of the control loop
    
}


/** @brief   Set up the PID control loop
 *  @details Takes the encoder's position and compares this with the current 
 *           setpoint from the GCode. Does calculations to determine a new motor 
 *           command.
 */
void Controller_PID::control_loop_PID ()
{

    // Calculate the change in position for each cycle
    float delta_position = pos_actual - pos_last;

    // Error between the currect "desired" and the current "actual" encoder position/velocity
    pos_error = pos_desired - pos_actual;           // This is not being fed values from the inputs
    vel_error = vel_desired - vel_actual;

   // Testing print-outs
   // Serial << "pos_error:  " << pos_error << endl;
   // Serial << "vel_error:  " << vel_actual << endl;

    // Do integral calculations per cycle (using the position error)
    float integral_per_cycle = (pos_error_last + pos_error / 2) * delta_time;       // numerically doing an integral using midpoint calc

    //Add this cycle's integral to the integral cumulation
    integral_cumulation += integral_per_cycle;

    // make sure that this integral_cumulation does not exceed a max value or get super huge
    if (integral_cumulation > integral_cumulation_max)
    {
        integral_cumulation = integral_cumulation_max;      // set to max value 
    }
    if (integral_cumulation < -integral_cumulation_max)
    {
        integral_cumulation = -integral_cumulation_max;      // set to neg max value 
    }


    // Do the derivative calculations per cycle (using the velocity error)

    derivative_per_cycle = (vel_error - vel_error_last) / delta_time;


    // Implement control loop and motor control command

    // Follow control loop, need to multiply the Propertional and Derivative terms by another gain, "output_filter"
    float output_sum1 = ((pos_error * gain_kP) + (derivative_per_cycle * gain_kD)) * output_filter;

    // This "output_sum2" should be in units of the individual component outputs
    float output_sum2 = output_sum1 + (integral_cumulation * gain_kI);

    // *  *  *  *  *  *  *  *  * 
    // Last thing to do is to make sure the units work out so this final "output" is in PWM to go to the motors
    // *  *  *  *  *  *  *  *  * 
    
    // Combine individual outputs to get the final output
    // Multiply "output_sum2" by the constant "output_PWM" in order to output a PWM signal
    
    // LIMIT THIS TO MAKE SURE IT DOESNT BLOW UP!
    output = output_sum2 * output_PWM;

    //  Serial << "output:  " << output << endl;


    // Prepare variables for the next iteration, make sure this line is after when the 
    // get used above in the control loop code
    pos_error_last = pos_error;     
    vel_error_last = vel_error;
    pos_last = pos_actual;        



    // I think we'll probably put this in the GCode module or the kinematics module, but the example PID code used this one
   
    // This code could be nice so the code doesn't accidently crash the laser head!
    // //Trim the output to the bounds if needed.
    // if(outputBounded)
    // {
    //   if(output > outputUpperBound) output = outputUpperBound;
    //   if(output < outputLowerBound) output = outputLowerBound;
    // }

}




/** @brief   Set the controller's gain coefficient kP and kI in one method
 *  @param   gain_kP_new A new value for the controller's gain coefficient, kP
 *  @param   gain_kI_new A new value for the controller's gain coefficient, kI
 */
 void Controller_PID::set_gain_PID (float kP, float kI, float kD)
 {
    // In order to be able to change the protected variable "gain_kP" and "gain_kI"
    gain_kP = kP;

    gain_kI = kI;

    gain_kD = kD;
    
 }


/** @brief   Set the controller's gain coefficient kP
 *  @param   kP A new value for the controller's gain coefficient, kP
 */
 void Controller_PID::set_gain_kP (float kP)
 {
    // In order to be able to change the protected variable "gain_kP"
    gain_kP = kP;

 }

/** @brief   Get the controller's gain coefficient kP
 *  @param   gain_kP The value for the controller's gain coefficient, kP
 */
 float Controller_PID::get_gain_kP ()
 {
    return gain_kP;
 }


/** @brief   Set the controller's gain coefficient kI
 *  @param   kI A new value for the controller's gain coefficient, kI
 */
 void Controller_PID::set_gain_kI (float kI)
 {
    // In order to be able to change the protected variable "gain_kI"
    gain_kI = kI;

 }

/** @brief   Get the controller's gain coefficient kI
 *  @param   gain_kI The value for the controller's gain coefficient, kI
 */
 float Controller_PID::get_gain_kI ()
 {
    return gain_kI;
 }


/** @brief   Set the controller's gain coefficient kD
 *  @param   kD A new value for the controller's gain coefficient, kD
 */
 void Controller_PID::set_gain_kD (float kD)
 {
    // In order to be able to change the protected variable "gain_kD"
    gain_kD = kD;

 }

/** @brief   Get the controller's gain coefficient kD
 *  @param   gain_kD The value for the controller's gain coefficient, kD
 */
 float Controller_PID::get_gain_kD ()
 {
    return gain_kD;
 }




/** @brief   Set the controller's desired position command that comes from the GCode
 *  @param   pos_desired_initial A new value for the controller's desired position. This value will be contantly
 *           being updated by the GCode processor
 */
void Controller_PID::set_pos_desired (float pos_desired_initial)
 {
    // In order to be able to change the protected variable "pos_desired"
    // The GCode processer will use this to constantly feed in new datapoints from the GCode
    pos_desired = pos_desired_initial;

 }

/** @brief   Get the controller's desired position command that comes from the GCode
 *  @param   pos_desired The value for the controller's desired position. 
 */
float Controller_PID::get_pos_desired ()
 {
    // Return the "pos_desired" value
    return pos_desired;
 }


/** @brief   Set the controller's actual position command that comes from the encoder
 *  @param   pos_actual_initial A new value for the controller's actual position. This value will be contantly
 *           being updated by the encoder processor
 */
void Controller_PID::set_pos_actual (float pos_actual_initial)
 {
    // In order to be able to change the protected variable "pos_actual"
    // The encoder processer will use this to constantly feed in new datapoints from the encoder
    pos_actual = pos_actual_initial;

 }


/** @brief   Get the controller's actual position command that comes from the encoder
 *  @param   pos_actual The value for the controller's actual position. 
 */
float Controller_PID::get_pos_actual ()
 {
    // Return the "pos_actual" value
    return pos_actual;
 }



/** @brief   Set the controller's desired velocity command that comes from the GCode
 *  @param   vel_desired_initial A new value for the controller's desired velocity. This value will be contantly
 *           being updated by the GCode processor
 */
void Controller_PID::set_vel_desired (float vel_desired_initial)
 {
    // In order to be able to change the protected variable "vel_desired"
    // The GCode processer will use this to constantly feed in new datapoints from the GCode
    vel_desired = vel_desired_initial;


   
 }

/** @brief   Get the controller's desired velocity command that comes from the GCode
 *  @param   vel_desired The value for the controller's desired velocity. 
 */
float Controller_PID::get_vel_desired ()
 {
    // Return the "vel_desired" value, will mostly be used for testing
    return vel_desired;
 }


/** @brief   Set the controller's actual velocity command that comes from the encoder
 *  @param   vel_actual_initial A new value for the controller's actual velocity. This value will be contantly
 *           being updated by the encoder processor
 */
void Controller_PID::set_vel_actual (float vel_actual_initial)
 {
    // In order to be able to change the protected variable "vel_actual"
    // The encoder processer will use this to constantly feed in new datapoints from the encoder
    vel_actual = vel_actual_initial;

 }


/** @brief   Get the controller's actual position command that comes from the encoder
 *  @param   pos_actual The value for the controller's actual position. 
 */
float Controller_PID::get_vel_actual ()
 {
    // Return the "vel_actual" value
    return vel_actual;
 }



/** @brief   Get the output of the control loop
 *  @param   output The value for the controller's desired velocity. 
 */
float Controller_PID::get_output ()
 {
    // Return the "output" value, will be used to (hopefully) correct the motors and make this control loop worthwhile
    return output;
 }

// FOR TESTING

/** @brief   Get the position error of the control loop
 *  @param   pos_error The value for the controller's position error. 
 */
float Controller_PID::get_pos_error ()
 {
    // Return the "pos_error" value, will be used to (hopefully) correct the motors and make this control loop worthwhile
    return pos_error;
 }




















