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
 */


/** @brief   Class which implements a P, PI, or PID controller for a motor control
 *  @details Sets up the P, PI, and PID controller classes and sets up the methods 
 *           for each of these controller classes. 
 *  
 */


#include "libraries&constants.h"


// ------ General Questions about the code ------

// Use Shares and Queues!!

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




// Declare the shares and queues that this file needs to draw from




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
 */

Controller_PID::Controller_PID (float kP_gain, float kI_gain, float kD_gain, float pos_desired_initial, float pos_actual_initial, float vel_desired_initial, float vel_actual_initial)
{
    // Save the parameter and initialize it as "gain_KP" 
    gain_kP = kP_gain;

    // Save the parameter and initialize it as "gain_KI" 
    gain_kI = kI_gain;

    // Save the parameter and initialize it as "gain_KD" 
    gain_kI = kI_gain;

    // From the GCode: "Desired" (setpoint) (I'm pretty sure this is going to be some sort of GCode_Share value)
    pos_desired = pos_desired_initial;          // Save the parameter and initialize it
    vel_desired = vel_desired_initial;          // Save the parameter and initialize it

    // From the Encoder: "Actual" (feedback) (I'm pretty sure this is going to be some sort of Encoder_Share value)
    pos_actual = pos_actual_initial;          // Save the parameter and initialize it
    vel_actual = vel_actual_initial;          // Save the parameter and initialize it


    // Parameters that are used in this class specifically

    pos_actual_current = 0;
    vel_actual_current = 0;


    current_time = 0;
    last_time = 0;

    pos_error = 0;
    pos_last_error = 0;

    vel_error = 0;
    vel_last_error = 0;


    integral_cumulation = 0;
    integral_cumulation_max = 1000;           // Set this number when we have a better understanding of the device parameters

    output_filter = 1;                      // Set this number when we have a better understanding of the device parameters
    output_PWM = 1;                          // Set this number so that the units work out for outputing a PWM signal
    
    output = 0;                              // This is the value of the final output of the control loop
    
}


/** @brief   Set up the Proportional only control loop
 *  @details Takes the encoder's position and compares this with the current 
 *           setpoint from the GCode. Does calculations to determine a new motor 
 *           command and return this new motor command.
 *  @param   --------- This input is the value of perceived position sent 
 *           from the encoder - - - - > i don't think we need an input to this method
 *  @returns The current value of the filter's output
 */
void Controller_PID::control_loop_PID ()
{

    // Take in the encoder position from either a share or a queue
    // Do I need to define the share's and queues as inputs in the control_loop_PID(**Define Here**)?
    // Also will I need to configure an output (instead of it just being "void")?

    // current_feedback = encoder_share.get ()          // fill in with correct names



    // // calculate delta_time from grabbing current_time
   //  current_time = timer_share.get ()          // fill in with correct names

    // FOR TESTING ONLY
    current_time = 100;                        // ms


    float delta_time = current_time - last_time;

    // Save this for the next iteration
    current_time = last_time;




    // Calculate the vel_actual_current using postion and time
    // This needs to be calculated every time using the position because 
    // the encoder only meausures the position
    float delta_position = pos_actual_current - pos_actual;

    vel_actual_current = delta_position / delta_time;


    // Error between the currect "desired" and the "actual" encoder position/velocity
    pos_error = pos_desired - pos_actual_current;           // This is not being fed values from the inputs
    vel_error = vel_desired - vel_actual_current;


    

    // Do integral calculations per cycle (using the position error)
    float integral_per_cycle = (pos_last_error + pos_error / 2) * delta_time;       // numerically doing an integral using midpoint calc

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

    derivative_per_cycle = (vel_error - vel_last_error) / delta_time;



    // Implement control loop and motor control command

    // Follow control loop, need to multiply the Propertional and Derivative terms by another gain, "output_filter"
    float output_sum1 = ((pos_error * gain_kP) + (derivative_per_cycle * gain_kD)) * output_filter;

    // Combine Outputs to get the actual output
    // This "output_sum2" should be in units of the individual component outputs
    float output_sum2 = output_sum1 + (integral_cumulation * gain_kI);

    // *  *  *  *  *  *  *  *  * 
    // Last thing to do is to make sure the units work out so this final "output" is in PWM to go to the motors
    // *  *  *  *  *  *  *  *  * 

    // Multiply "output_sum2" by the constant "output_PWM" in order to output a PWM signal


    output = output_sum2 * output_PWM;


   //  Serial << "Output" << output << endl;


    // prepare variables for the next iteration, make sure this line is after when the 
    // get used above in the control loop code
    current_time = last_time;
    pos_error = pos_last_error;
    vel_error = vel_last_error;
    pos_actual_current = pos_actual;         // This should update the current position



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




/** @brief   Set the controller's desired position command that comes from the GCode
 *  @param   pos_desired_new A new value for the controller's desired position. This value will be contantly
 *           being updated by the GCode processor
 */
void Controller_PID::set_pos_desired_PID (float pos_desired_new)
 {
    // In order to be able to change the protected variable "pos_desired"
    // The GCode processer will use this to constantly feed in new datapoints from the GCode
    pos_desired = pos_desired_new;

 }


/** @brief   Get the controller's desired position command that comes from the GCode
 *  @param   pos_desired The value for the controller's desired position. 
 */
float Controller_PID::get_pos_desired_PID ()
 {
    // Return the "pos_desired" value, will mostly be used for testing
    return pos_desired;
 }



/** @brief   Set the controller's desired velocity command that comes from the GCode
 *  @param   vel_desired_new A new value for the controller's desired velocity. This value will be contantly
 *           being updated by the GCode processor
 */
void Controller_PID::set_vel_desired_PID (float vel_desired_new)
 {
    // In order to be able to change the protected variable "vel_desired"
    // The GCode processer will use this to constantly feed in new datapoints from the GCode
    vel_desired = vel_desired_new;


   
 }

/** @brief   Get the controller's desired velocity command that comes from the GCode
 *  @param   vel_desired The value for the controller's desired velocity. 
 */
float Controller_PID::get_vel_desired_PID ()
 {
    // Return the "vel_desired" value, will mostly be used for testing
    return vel_desired;
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























// // * * * * * * * * * * This is the P and PI controllers only code * * * * * * * * * 

// // Will need to be updated to match the PID controller code if you want to run it well


// // ------------ P Controller ------------ //


// /** @brief   Creates a Proportional only controller to drive a motor
//  *  @details This constructor sets up the Proportional only controller. It does this
//  *           by taking in a gain value and a setpoint (gotten from the GCode). It reads
//  *           the actual position from the encoder output, compares this to the theoretical
//  *           position from the setpoint (initially from the GCode), computes an error value 
//  *           and updates the setpoint (motor command) to minimize the error.
//  *           It also will have functions that allow the gain and setpoint protected variables
//  *           to be changed by either the user or by the GCode.
//  *  @param   kP_gain This is the parameter for the input gain for the Proportional control
//  *  @param   setpoint_initial This is the initial value of the setpoint 
//  */

// Controller_P::Controller_P (float kP_gain, float setpoint_initial)
// {
//     // Save the parameter and initialize it as "gain_KP" 
//     gain_kP = kP_gain;

//     // Save the parameter and initialize it as "setpoint" 
//     setpoint = setpoint_initial;

//     // Parameters that are used in this class specifically

//     current_feedback = 0;
//     feedback = 0;

//     current_time = 0;
//     last_time = 0;

//     error = 0;
//     last_error = 0;



//     // Set up any pins that need to be configured here

// }


// /** @brief   Set up the Proportional only control loop
//  *  @details Takes the encoder's position and compares this with the current 
//  *           setpoint from the GCode. Does calculations to determine a new motor 
//  *           command and return this new motor command.
//  *  @param   encoder_position This input is the value of perceived position sent 
//  *           from the encoder
//  *  @returns The current value of the filter's output
//  */
// float Controller_P::control_loop_P (float encoder_position)
// {

//     // Take in the encoder position from either a share or a queue
//     // Use function pointer to do this? See example PID code

//     // current_feedback = encoder_share.get ()          // fill in with correct names


//     // Calculate the error between the current setpoint and the encoder position

//     error = setpoint - current_feedback;



//     // // ----- Won't need delta_time for a Proportional Controller only -----
//     // // calculate delta_time from grabbing current_time

//     // // current_time = timer_share.get ()          // fill in with correct names

//     // float delta_time = current_time - last_time;

//     // // Save this for the next iteration
//     // current_time = last_time;



//     // Implement control loop and motor control command

//     float output = error * gain_kP;             // I think "output" is the same as "setpoint_new"



//     // prepare variables for the next iteration, make sure this line is after when the 
//     // get used above in the control loop code
//     current_time = last_time;
//     error = last_error;



//     // Return the result of the motor command
//         // ????? Will this result then go back into the motor, which will 
//         // in turn come back to this function as a encoder reading?
//         // I think yes, but maybe not

// }


// /** @brief   Set the controller's gain coefficient kP
//  *  @param   gain_kP_new A new value for the controller's gain coefficient, kP
//  */
//  void Controller_P::set_gain_kP (float gain_kP_new)
//  {
//     // In order to be able to change the protected variable "gain_kP"
//     gain_kP = gain_kP_new;

//  }

// /** @brief   Get the controller's gain coefficient kP
//  *  @param   gain_kP The value for the controller's gain coefficient, kP
//  */
//  float Controller_P::get_gain_kP ()
//  {
//     // In order to be able to change the protected variable "gain_kP"
//     return gain_kP;

//  }


// /** @brief   Set the controller's setpoint
//  *  @param   setpoint_new A new value for the controller's setpoint. This value will be contantly
//  *           being updated by the GCode processor
//  */
// float Controller_P::change_setpoint_P (float setpoint_new)
//  {
//     // In order to be able to change the protected variable "setpoint"
//     // The GCode processer will use this to constantly feed in new datapoints from the GCode
//     setpoint = setpoint_new;

//     return setpoint;
//  }













// // ------------ PI Controller ------------ //

// /** @brief   Creates a Proportional and Integral  controller to drive a motor
//  *  @details This constructor sets up the Proportional and Integral controller. It does this
//  *           by taking in two gain values and a setpoint (gotten from the GCode). It reads
//  *           the actual position from the encoder output, compares this to the theoretical
//  *           position from the setpoint (initially from the GCode), computes an error value 
//  *           and updates the setpoint (motor command) to minimize the error.
//  *           It also will have functions that allow the gain and setpoint protected variables
//  *           to be changed by either the user or by the GCode.
//  *  @param   kP_gain This is the parameter for the input gain for the Proportional control
//  *  @param   kI_gain This is the parameter for the input gain for the Integral control
//  *  @param   setpoint_initial This is the initial value of the setpoint 
//  */

// Controller_PI::Controller_PI (float kP_gain, float kI_gain, float setpoint_initial)
// {
//     // Save the parameter and initialize it as "gain_KP" 
//     gain_kP = kP_gain;

//     // Save the parameter and initialize it as "gain_KI" 
//     gain_kI = kI_gain;

//     // Save the parameter and initialize it as "setpoint" 
//     setpoint = setpoint_initial;

//     // Parameters that are used in this class specifically

//     current_feedback = 0;
//     feedback = 0;

//     current_time = 0;
//     last_time = 0;

//     error = 0;
//     last_error = 0;

//     integral_cumulation = 0;
//     integral_cumulation_max = 10000;            // Set this number when we have better understanding of the device parameters


//     // Set up any pins that need to be configured here

// }


// /** @brief   Set up the Proportional and Integral control loop
//  *  @details Takes the encoder's position and compares this with the current 
//  *           setpoint from the GCode. Does calculations to determine a new motor 
//  *           command and return this new motor command.
//  *  @param   encoder_position This input is the value of perceived position sent 
//  *           from the encoder
//  *  @returns The current value of the filter's output
//  */
// float Controller_PI::control_loop_PI (float encoder_position)
// {

//     // Take in the encoder position from either a share or a queue
//     // Use function pointer to do this? See example PID code

//     // current_feedback = encoder_share.get ()          // fill in with correct names


//     // Calculate the error between the current setpoint and the encoder position
//     error = setpoint - current_feedback;



//     // // calculate delta_time from grabbing current_time
//     // current_time = timer_share.get ()          // fill in with correct names

//     float delta_time = current_time - last_time;

//     // Save this for the next iteration
//     current_time = last_time;


//     // Do integral calculations per cycle
//     float integral_per_cycle = (last_error + error / 2) * delta_time;       // numerically doing an integral



//     // Should we? Compare to a Saturation for Motor (CONSTANT) (+- 100%) and do an IF statement from there
//     // This would probably work well for velocity control but maybe not so well for position control
   
//     //Add this cycle's integral to the integral cumulation
//     integral_cumulation += integral_per_cycle;



//     // PLEASE CHANGE ******** Compare integral_cumulation * gain_kI to some saturation 
//     // max value so our numbers don't get to huge.
   


//     // make sure that this integral_cumulation does not exceed a max value or get super huge
//     if (integral_cumulation > integral_cumulation_max)
//     {
//         integral_cumulation = integral_cumulation_max;      // set to max value 
//     }
//     if (integral_cumulation < -integral_cumulation_max)
//     {
//         integral_cumulation = -integral_cumulation_max;      // set to neg max value 
//     }



//     // Implement control loop and motor control command

//     float output = error * gain_kP + integral_cumulation * gain_kI;             // I think "output" is the same as "setpoint_new"




//     // prepare variables for the next iteration, make sure this line is after when the 
//     // get used above in the control loop code
//     current_time = last_time;
//     error = last_error;


//     // This code could be nice so the code doesn't accidently crash the laser head!
//     // Still need to define the upper and lower bounds in the header file
//     // //Trim the output to the bounds if needed.
//     // if(outputBounded)
//     // {
//     //   if(output > outputUpperBound) output = outputUpperBound;
//     //   if(output < outputLowerBound) output = outputLowerBound;
//     // }


//     // Return the result of the motor command
//         // ????? Will this result then go back into the motor, which will 
//         // in turn come back to this function as a encoder reading?
//         // I think yes, but maybe not

// }


// /** @brief   Set the controller's gain coefficient kP and kI in one method
//  *  @param   gain_kP_new A new value for the controller's gain coefficient, kP
//  *  @param   gain_kI_new A new value for the controller's gain coefficient, kI
//  */
//  void Controller_PI::set_gain_PI (float gain_kP_new, float gain_kI_new)
//  {
//     // In order to be able to change the protected variable "gain_kP" and "gain_kI"
//     gain_kP = gain_kP_new;

//     gain_kI = gain_kI_new;
    
//  }


// /** @brief   Set the controller's gain coefficient kP
//  *  @param   gain_kP_new A new value for the controller's gain coefficient, kP
//  */
//  void Controller_PI::set_gain_kP (float gain_kP_new)
//  {
//     // In order to be able to change the protected variable "gain_kP"
//     gain_kP = gain_kP_new;

//  }

// /** @brief   Get the controller's gain coefficient kP
//  *  @param   gain_kP The value for the controller's gain coefficient, kP
//  */
//  float Controller_PI::get_gain_kP ()
//  {
//     // In order to be able to change the protected variable "gain_kP"
//     return gain_kP;

//  }


// /** @brief   Set the controller's gain coefficient kI
//  *  @param   gain_kI_new A new value for the controller's gain coefficient, kI
//  */
//  void Controller_PI::set_gain_kI (float gain_kI_new)
//  {
//     // In order to be able to change the protected variable "gain_kI"
//     gain_kI = gain_kI_new;

//  }

// /** @brief   Get the controller's gain coefficient kI
//  *  @param   gain_kI The value for the controller's gain coefficient, kI
//  */
//  float Controller_PI::get_gain_kI ()
//  {
//     // In order to be able to change the protected variable "gain_kI"
//     return gain_kI;

//  }


// /** @brief   Set the controller's setpoint
//  *  @param   setpoint_new A new value for the controller's setpoint. This value will be contantly
//  *           being updated by the GCode processor
//  */
// float Controller_PI::change_setpoint_PI (float setpoint_new)
//  {
//     // In order to be able to change the protected variable "setpoint"
//     // The GCode processer will use this to constantly feed in new datapoints from the GCode
//     setpoint = setpoint_new;

//     return setpoint;
//  }




