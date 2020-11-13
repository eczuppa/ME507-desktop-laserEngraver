/** @file control.cpp
 *    This file contains the source code for a class that implements a P, PI, or PID motor control
 *    code to drive the motors.
 * 
 *  @author Matthew Carlson
 *  @date  2020-Nov-10 Original file
 */


/** @brief   Class which implements a P, PI, or PID controller for a motor control
 *  @details Sets up the P, PI, and PID controller classes and sets up the methods 
 *           for each of these controller classes. 
 * 
 */


#include "libraries&constants.h"


// ------ General Questions about the code ------

// How are we communicating with the encoders and GCode? I was thinking shares and queues but
// could/should we use function pointers instead?

// I've set everything as floats, but maybe some should be double's or int's?

// How's our timer going to work?

// What should we be calling the output of the control loop? "output"? "setpoint_new"?

// How to give the output from the control loop to back to the motor? Shares/queues?

// Will the "setpoint" variable need to be a 1X2 array? setpoint_P: [x, y]?
// Or will we just have 5 instances of Controller_PID, motor1_x, motor1_y, motor2_x, motor2_y, laser_power
// Do I need to call it "setpoint_P"? ... I don't think I need to cause it's protected within this class, right?




// ------------ P Controller ------------ //


/** @brief   Creates a Proportional only controller to drive a motor
 *  @details This constructor sets up the Proportional only controller. It does this
 *           by taking in a gain value and a setpoint (gotten from the GCode). It reads
 *           the actual position from the encoder output, compares this to the theoretical
 *           position from the setpoint (initially from the GCode), computes an error value 
 *           and updates the setpoint (motor command) to minimize the error.
 *           It also will have functions that allow the gain and setpoint protected variables
 *           to be changed by either the user or by the GCode.
 *  @param   kP_gain This is the parameter for the input gain for the Proportional control
 *  @param   setpoint_initial This is the initial value of the setpoint 
 */

Controller_P::Controller_P (float kP_gain, float setpoint_initial)
{
    // Save the parameter and initialize it as "gain_KP" 
    gain_kP = kP_gain;

    // Save the parameter and initialize it as "setpoint" 
    setpoint = setpoint_initial;

    // Parameters that are used in this class specifically

    current_feedback = 0;
    feedback = 0;

    current_time = 0;
    last_time = 0;

    error = 0;
    last_error = 0;



    // Set up any pins that need to be configured here

}


/** @brief   Set up the Proportional only control loop
 *  @details Takes the encoder's position and compares this with the current 
 *           setpoint from the GCode. Does calculations to determine a new motor 
 *           command and return this new motor command.
 *  @param   encoder_position This input is the value of perceived position sent 
 *           from the encoder
 *  @returns The current value of the filter's output
 */
float Controller_P::control_loop_P (float encoder_position)
{

    // Take in the encoder position from either a share or a queue
    // Use function pointer to do this? See example PID code

    // current_feedback = encoder_share.get ()          // fill in with correct names


    // Calculate the error between the current setpoint and the encoder position

    error = setpoint - current_feedback;



    // // ----- Won't need delta_time for a Proportional Controller only -----
    // // calculate delta_time from grabbing current_time

    // // current_time = timer_share.get ()          // fill in with correct names

    // float delta_time = current_time - last_time;

    // // Save this for the next iteration
    // current_time = last_time;



    // Implement control loop and motor control command

    float output = error * gain_kP;             // I think "output" is the same as "setpoint_new"



    // prepare variables for the next iteration, make sure this line is after when the 
    // get used above in the control loop code
    current_time = last_time;
    error = last_error;



    // Return the result of the motor command
        // ????? Will this result then go back into the motor, which will 
        // in turn come back to this function as a encoder reading?
        // I think yes, but maybe not

}


/** @brief   Set the controller's gain coefficient kP
 *  @param   gain_kP_new A new value for the controller's gain coefficient, kP
 */
 void Controller_P::set_gain_kP (float gain_kP_new)
 {
    // In order to be able to change the protected variable "gain_kP"
    gain_kP = gain_kP_new;

 }

/** @brief   Get the controller's gain coefficient kP
 *  @param   gain_kP The value for the controller's gain coefficient, kP
 */
 float Controller_P::get_gain_kP ()
 {
    // In order to be able to change the protected variable "gain_kP"
    return gain_kP;

 }


/** @brief   Set the controller's setpoint
 *  @param   setpoint_new A new value for the controller's setpoint. This value will be contantly
 *           being updated by the GCode processor
 */
float Controller_P::change_setpoint_P (float setpoint_new)
 {
    // In order to be able to change the protected variable "setpoint"
    // The GCode processer will use this to constantly feed in new datapoints from the GCode
    setpoint = setpoint_new;

    return setpoint;
 }













// ------------ PI Controller ------------ //

/** @brief   Creates a Proportional and Integral  controller to drive a motor
 *  @details This constructor sets up the Proportional and Integral controller. It does this
 *           by taking in two gain values and a setpoint (gotten from the GCode). It reads
 *           the actual position from the encoder output, compares this to the theoretical
 *           position from the setpoint (initially from the GCode), computes an error value 
 *           and updates the setpoint (motor command) to minimize the error.
 *           It also will have functions that allow the gain and setpoint protected variables
 *           to be changed by either the user or by the GCode.
 *  @param   kP_gain This is the parameter for the input gain for the Proportional control
 *  @param   kI_gain This is the parameter for the input gain for the Integral control
 *  @param   setpoint_initial This is the initial value of the setpoint 
 */

Controller_PI::Controller_PI (float kP_gain, float kI_gain, float setpoint_initial)
{
    // Save the parameter and initialize it as "gain_KP" 
    gain_kP = kP_gain;

    // Save the parameter and initialize it as "gain_KI" 
    gain_kI = kI_gain;

    // Save the parameter and initialize it as "setpoint" 
    setpoint = setpoint_initial;

    // Parameters that are used in this class specifically

    current_feedback = 0;
    feedback = 0;

    current_time = 0;
    last_time = 0;

    error = 0;
    last_error = 0;

    integral_cumulation = 0;
    integral_cumulation_max = 10000;            // Set this number when we have better understanding of the device parameters


    // Set up any pins that need to be configured here

}


/** @brief   Set up the Proportional and Integral control loop
 *  @details Takes the encoder's position and compares this with the current 
 *           setpoint from the GCode. Does calculations to determine a new motor 
 *           command and return this new motor command.
 *  @param   encoder_position This input is the value of perceived position sent 
 *           from the encoder
 *  @returns The current value of the filter's output
 */
float Controller_PI::control_loop_PI (float encoder_position)
{

    // Take in the encoder position from either a share or a queue
    // Use function pointer to do this? See example PID code

    // current_feedback = encoder_share.get ()          // fill in with correct names


    // Calculate the error between the current setpoint and the encoder position
    error = setpoint - current_feedback;



    // // calculate delta_time from grabbing current_time
    // current_time = timer_share.get ()          // fill in with correct names

    float delta_time = current_time - last_time;

    // Save this for the next iteration
    current_time = last_time;


    // Do integral calculations per cycle
    float integral_per_cycle = (last_error + error / 2) * delta_time;       // numerically doing an integral

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



    // Implement control loop and motor control command

    float output = error * gain_kP + integral_per_cycle * gain_kI;             // I think "output" is the same as "setpoint_new"



    // prepare variables for the next iteration, make sure this line is after when the 
    // get used above in the control loop code
    current_time = last_time;
    error = last_error;


    // This code could be nice so the code doesn't accidently crash the laser head!
    // Still need to define the upper and lower bounds in the header file
    // //Trim the output to the bounds if needed.
    // if(outputBounded)
    // {
    //   if(output > outputUpperBound) output = outputUpperBound;
    //   if(output < outputLowerBound) output = outputLowerBound;
    // }


    // Return the result of the motor command
        // ????? Will this result then go back into the motor, which will 
        // in turn come back to this function as a encoder reading?
        // I think yes, but maybe not

}


/** @brief   Set the controller's gain coefficient kP and kI in one method
 *  @param   gain_kP_new A new value for the controller's gain coefficient, kP
 *  @param   gain_kI_new A new value for the controller's gain coefficient, kI
 */
 void Controller_PI::set_gain_PI (float gain_kP_new, float gain_kI_new)
 {
    // In order to be able to change the protected variable "gain_kP" and "gain_kI"
    gain_kP = gain_kP_new;

    gain_kI = gain_kI_new;
    
 }


/** @brief   Set the controller's gain coefficient kP
 *  @param   gain_kP_new A new value for the controller's gain coefficient, kP
 */
 void Controller_PI::set_gain_kP (float gain_kP_new)
 {
    // In order to be able to change the protected variable "gain_kP"
    gain_kP = gain_kP_new;

 }

/** @brief   Get the controller's gain coefficient kP
 *  @param   gain_kP The value for the controller's gain coefficient, kP
 */
 float Controller_PI::get_gain_kP ()
 {
    // In order to be able to change the protected variable "gain_kP"
    return gain_kP;

 }


/** @brief   Set the controller's gain coefficient kI
 *  @param   gain_kI_new A new value for the controller's gain coefficient, kI
 */
 void Controller_PI::set_gain_kI (float gain_kI_new)
 {
    // In order to be able to change the protected variable "gain_kI"
    gain_kI = gain_kI_new;

 }

/** @brief   Get the controller's gain coefficient kI
 *  @param   gain_kI The value for the controller's gain coefficient, kI
 */
 float Controller_PI::get_gain_kI ()
 {
    // In order to be able to change the protected variable "gain_kI"
    return gain_kI;

 }


/** @brief   Set the controller's setpoint
 *  @param   setpoint_new A new value for the controller's setpoint. This value will be contantly
 *           being updated by the GCode processor
 */
float Controller_PI::change_setpoint_PI (float setpoint_new)
 {
    // In order to be able to change the protected variable "setpoint"
    // The GCode processer will use this to constantly feed in new datapoints from the GCode
    setpoint = setpoint_new;

    return setpoint;
 }







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
 *  @param   setpoint_initial This is the initial value of the setpoint 
 */

Controller_PID::Controller_PID (float kP_gain, float kI_gain, float kD_gain, float setpoint_initial)
{
    // Save the parameter and initialize it as "gain_KP" 
    gain_kP = kP_gain;

    // Save the parameter and initialize it as "gain_KI" 
    gain_kI = kI_gain;

    // Save the parameter and initialize it as "gain_KD" 
    gain_kI = kI_gain;

    // Save the parameter and initialize it as "setpoint" 
    setpoint = setpoint_initial;

    // Parameters that are used in this class specifically

    current_feedback = 0;
    feedback = 0;

    current_time = 0;
    last_time = 0;

    error = 0;
    last_error = 0;

    integral_cumulation = 0;
    integral_cumulation_max = 10000;            // Set this number when we have better understanding of the device parameters


    // Set up any pins that need to be configured here

}


/** @brief   Set up the Proportional only control loop
 *  @details Takes the encoder's position and compares this with the current 
 *           setpoint from the GCode. Does calculations to determine a new motor 
 *           command and return this new motor command.
 *  @param   encoder_position This input is the value of perceived position sent 
 *           from the encoder
 *  @returns The current value of the filter's output
 */
float Controller_PID::control_loop_PID (float encoder_position)
{

    // Take in the encoder position from either a share or a queue
    // Use function pointer to do this? See example PID code

    // current_feedback = encoder_share.get ()          // fill in with correct names


    // Calculate the error between the current setpoint and the encoder position
    error = setpoint - current_feedback;



    // // calculate delta_time from grabbing current_time
    // current_time = timer_share.get ()          // fill in with correct names

    float delta_time = current_time - last_time;

    // Save this for the next iteration
    current_time = last_time;


    // Do integral calculations per cycle
    float integral_per_cycle = (last_error + error / 2) * delta_time;       // numerically doing an integral

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


    // Do the derivative calculations per cycle

    derivative_per_cycle = (error - last_error) / delta_time;



    // Implement control loop and motor control command

    float output = (error * gain_kP) + (integral_per_cycle * gain_kI) + (derivative_per_cycle * gain_kD);             // I think "output" is the same as "setpoint_new"



    // prepare variables for the next iteration, make sure this line is after when the 
    // get used above in the control loop code
    current_time = last_time;
    error = last_error;


    // This code could be nice so the code doesn't accidently crash the laser head!
    // //Trim the output to the bounds if needed.
    // if(outputBounded)
    // {
    //   if(output > outputUpperBound) output = outputUpperBound;
    //   if(output < outputLowerBound) output = outputLowerBound;
    // }


    // Return the result of the motor command
        // ????? Will this result then go back into the motor, which will 
        // in turn come back to this function as a encoder reading?
        // I think yes, but maybe not

}








/** @brief   Set the controller's gain coefficient kP and kI in one method
 *  @param   gain_kP_new A new value for the controller's gain coefficient, kP
 *  @param   gain_kI_new A new value for the controller's gain coefficient, kI
 */
 void Controller_PID::set_gain_PID (float gain_kP_new, float gain_kI_new, float gain_kD_new)
 {
    // In order to be able to change the protected variable "gain_kP" and "gain_kI"
    gain_kP = gain_kP_new;

    gain_kI = gain_kI_new;

    gain_kD = gain_kD_new;
    
 }


/** @brief   Set the controller's gain coefficient kP
 *  @param   gain_kP_new A new value for the controller's gain coefficient, kP
 */
 void Controller_PID::set_gain_kP (float gain_kP_new)
 {
    // In order to be able to change the protected variable "gain_kP"
    gain_kP = gain_kP_new;

 }

/** @brief   Get the controller's gain coefficient kP
 *  @param   gain_kP The value for the controller's gain coefficient, kP
 */
 float Controller_PID::get_gain_kP ()
 {
    // In order to be able to change the protected variable "gain_kP"
    return gain_kP;

 }


/** @brief   Set the controller's gain coefficient kI
 *  @param   gain_kI_new A new value for the controller's gain coefficient, kI
 */
 void Controller_PID::set_gain_kI (float gain_kI_new)
 {
    // In order to be able to change the protected variable "gain_kP"
    gain_kI = gain_kI_new;

 }

/** @brief   Get the controller's gain coefficient kI
 *  @param   gain_kI The value for the controller's gain coefficient, kI
 */
 float Controller_PID::get_gain_kI ()
 {
    // In order to be able to change the protected variable "gain_kI"
    return gain_kI;

 }


/** @brief   Set the controller's gain coefficient kD
 *  @param   gain_kD_new A new value for the controller's gain coefficient, kD
 */
 void Controller_PID::set_gain_kD (float gain_kD_new)
 {
    // In order to be able to change the protected variable "gain_kD"
    gain_kD = gain_kD_new;

 }

/** @brief   Get the controller's gain coefficient kD
 *  @param   gain_kD The value for the controller's gain coefficient, kD
 */
 float Controller_PID::get_gain_kD ()
 {
    // In order to be able to change the protected variable "gain_kD"
    return gain_kD;

 }




/** @brief   Set the controller's setpoint
 *  @param   setpoint_new A new value for the controller's setpoint. This value will be contantly
 *           being updated by the GCode processor
 */
float Controller_PID::change_setpoint_PID (float setpoint_new)
 {
    // In order to be able to change the protected variable "setpoint"
    // The GCode processer will use this to constantly feed in new datapoints from the GCode
    setpoint = setpoint_new;

    return setpoint;
 }
