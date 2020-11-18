/** @file control.h
 *    This file contains the headers for a class that implements a P, PI, or PID motor control
 *    code to drive the motors.
 *    This file sets up three classes: "Controller_P", "Controller_PI", and "Controller_PID"
 * 
 *  @author Matthew Carlson
 *  @date  2020-Nov-10 Original file
 */





/** @brief   Class which implements a P controller that can be used to drive the motors
 *  @details Defines Protected variables that are used in the control.cpp file with the
 *           "Controller_P" class.
 *           Also sets up contructor "Controller_P" and method's "control_loop_P", 
 *           "set_gain_kP", and "change_setpoint_P"
 */


#ifndef CONTROLLER_H
#define CONTROLLER_H

// #include "libraries&constants.h"

// Controller to take in 3 Gains, P I D and Setpoint (GCode) and a Feedback (Encoder)



/** @brief   Class which implements a PID controller that can be used to drive the motors
 *  @details Defines Protected variables that are used in the control.cpp file with the
 *           "Controller_PID" class.
 *           Also sets up contructor "Controller_PID" and method's "control_loop_PID", 
 *           "set_gain_kP", "set_gain_kI", "set_gain_kD" and "change_setpoint_PID"
 * 
 */

class Controller_PID
{
protected:

    // The value of the gain for the Proportional part of the Controller: kP
    float gain_kP;

    // The value of the gain for the Integal part of the Controller: kI
    float gain_kI;

    // The value of the gain for the Derivative part of the Controller: kD
    float gain_kD;

    // Position Desired and Actual:

    float pos_desired;          // comes from the GCode (setpoint/target)
    float pos_actual;           // comes from the encoder (feedback)


    // Velocity Desired and Actual:

    float vel_desired;          // comes from the GCode (setpoint/target)
    float vel_actual;           // comes from the encoder (feedback)


    // // The value for the Setpoint, this is the commanded position from GCode
    // float setpoint;                     // potentially make this a large integer instead of a float
    // // This will be in terms of A and B (converted from X and Y values in the kinematics module)

    // // This is the value for Feedback that comes from the encoder
    // float feedback;
    // // This will be in terms of linear A and B that will be calculated in the kinematics module 


    // This is the current actual position and velocity (feedback) that is 
    // continually being read by the encoder.
    float pos_actual_current;
    float vel_actual_current;

    // time will come from our overall timer (using millis or something)
    float current_time;
    float last_time;

    // The error will be computed as th difference between the Desired and the Actual
    float pos_error;
    float pos_last_error;

    float vel_error;
    float vel_last_error;

    float integral_cumulation;
    float integral_cumulation_max;

    float derivative_per_cycle;

    float output_filter;
    float output_PWM;

    float output;

public:

    // ----- Define the Constructor -----

    // Constructor for Controller_P that takes in a gain, position desired, position actual, velocity desired, and velocity actual)
    Controller_PID (float kP_gain, float kI_gain, float kD_gain, float pos_desired_initial, float pos_actual_initial, float vel_desired_initial, float vel_actual_initial);              
    // Note: make these input values equal to the Protected variables in the .cpp file



    // ----- Define the Methods -----

    // The PI Control Loop: takes in current position from encoder and 
    // compares this with the setpoint data. Calculate Error and calculate
    // the new motor command. Return the new motor command as a float.
    void control_loop_PID ();      
    // We will use this loop to control the A (left) and B (right) motors
    // Might need to include inputs and outputs for this one instead of just "void"





    // set_gain_PID allows the user to input kP, kI and kD
    void set_gain_PID (float gain_kP_new, float gain_kI_new, float gain_kD_new);

    // set_gain_kP allows the user to input a new gain value. Allows us to change the Protected variable
    void set_gain_kP (float gain_kP_new);        // This will be for testing only

    // get_gain_kP allows the user to get the new gain value.
    float get_gain_kP ();                        // This will be for testing only

    // set_gain_kI allows the user to input a new gain value for the integral part of the controller. 
    // Allows us to change the Protected variable.
    void set_gain_kI (float gain_kI_new);        // This will be for testing only

    // get_gain_kI allows the user to get the new gain value. 
    float get_gain_kI ();                        // This will be for testing only

    // set_gain_kD allows the user to input a new gain value for the derivative part of the controller. 
    // Allows us to change the Protected variable.
    void set_gain_kD (float gain_kD_new);        // This will be for testing only

    // get_gain_kD allows the user to get the new gain value. 
    float get_gain_kD ();                        // This will be for testing only
 

    // set_pos_desired_PID allows the GCode to continually feed in new position commands
    void set_pos_desired_PID (float pos_desired_new);        // This will allow the GCode to overwrite it's previous commands

    // get_pos_desired_PID helps with testing
    float get_pos_desired_PID ();


    // set_vel_desired_PID allows the GCode to continually feed in new velocity commands
    void set_vel_desired_PID (float vel_desired_new);        // This will allow the GCode to overwrite it's previous commands

    // get_vel_desired_PID helps with testing
    float get_vel_desired_PID ();

    // get_output displays the output from the PID loop
    float get_output ();                               // Value will be fed to the motors, PWM

    // get_pos_error displays the position error from the PID loop
    float get_pos_error ();                               // Value of position error










// // * * * * * * * * * * This is the P and PI controllers only code * * * * * * * * * 

// // Will need to be updated to match the PID controller code if you want to run it well

// class Controller_P
// {
// protected:

//     // The value of the gain for the Proportional part of the Controller: kP
//     float gain_kP;

//     // The value for the Setpoint, this is the commanded position from GCode
//     float setpoint;                   // potentially make this a large integer instead of a float



//     // feedback comes from the encoder
//     float current_feedback;
//     float feedback;

//     // time will come from our overall timer
//     float current_time;
//     float last_time;

//     // error will be computed as a difference between setpoint and feedback
//     float error;
//     float last_error;



// public:

//     // ----- Define the Constructor -----

//     // Constructor for Controller_P that takes in a gain and setpoint
//     Controller_P (float kP_gain, float setpoint_initial);              
//     // Note: make these input values equal to the Protected variables in the .cpp file


//     // ----- Define the Methods -----

//     // The P Control Loop: takes in current position from encoder and 
//     // compares this with the setpoint data. Calculate Error and calculate
//     // the new motor command. Return the new motor command as a float.
//     float control_loop_P (float encoder_position);

//     // set_gain_kP allows the user to input a new gain value. Allows us to change the Protected variable
//     void set_gain_kP (float gain_kP_new);        // This will be for testing only

//     // get_gain_kP allows the user to get the new gain value.
//     float get_gain_kP ();                        // This will be for testing only

//     // change_setpoint_kp allows the user to input a new gain value. Allows us to change the Protected variable
//     float change_setpoint_P (float setpoint_new);        // This will allow the GCode to overwrite the initial setpoint

// };








// /** @brief   Class which implements a PI controller that can be used to drive the motors
//  *  @details Defines Protected variables that are used in the control.cpp file with the
//  *           "Controller_PI" class.
//  *           Also sets up contructor "Controller_PI" and method's "control_loop_PI", 
//  *           "set_gain_kP", "set_gain_kI" and "change_setpoint_PI"
//  * 
//  */

// class Controller_PI
// {
// protected:

//     // The value of the gain for the Proportional part of the Controller: kP
//     float gain_kP;

//     // The value of the gain for the Integal part of the Controller: kI
//     float gain_kI;

//     // The value for the Setpoint, this is the commanded position from GCode
//     float setpoint;                   // potentially make this a large integer instead of a float

//     // Will this need to be a 1X2 array? setpoint_PI: [x, y]
//     // Do I need to call this "setpoint_PI"?
//     // I don't think I need to cause it's protected within this class, cheers?


//     // feedback comes from the encoder
//     float current_feedback;
//     float feedback;

//     // time will come from our overall timer
//     float current_time;
//     float last_time;

//     // error will be computed as a difference between setpoint and feedback
//     float error;
//     float last_error;

//     float integral_cumulation;
//     float integral_cumulation_max;


// public:

//     // ----- Define the Constructor -----

//     // Constructor for Controller_P that takes in a gain and setpoint
//     Controller_PI (float kP_gain, float kI_gain, float setpoint_initial);              
//     // Note: make these input values equal to the Protected variables in the .cpp file


//     // ----- Define the Methods -----

//     // The PI Control Loop: takes in current position from encoder and 
//     // compares this with the setpoint data. Calculate Error and calculate
//     // the new motor command. Return the new motor command as a float.
//     float control_loop_PI (float encoder_position);      
    
//     // Note: encoder_position is the position calculated from the encoder
//     // We will probably need to have a encoder_position be an array:
//     // [encoder_position_x, encoder_position_y]


//     // set_gain_PI allows the user to input both kP and kI
//     void set_gain_PI (float gain_kP_new, float gain_kI_new);

//     // set_gain_kP allows the user to input a new gain value. Allows us to change the Protected variable
//     void set_gain_kP (float gain_kP_new);        // This will be for testing only

//     // get_gain_kP allows the user to get the new gain value.
//     float get_gain_kP ();                        // This will be for testing only

//     // set_gain_kI allows the user to input a new gain value for the integral part of the controller. 
//     // Allows us to change the Protected variable.
//     void set_gain_kI (float gain_kI_new);        // This will be for testing only

//     // get_gain_kI allows the user to get the new gain value. 
//     float get_gain_kI ();                        // This will be for testing only


//     // change_setpoint_PI allows the user to input a new gain value. Allows us to 
//     // change the Protected variable. The GCode will continually overwrite this value
//     float change_setpoint_PI (float setpoint_new);        // This will allow the GCode to overwrite the initial setpoint

// };
































 
};
#endif  //CONTROLLER_H












