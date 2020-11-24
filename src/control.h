/** @file control.h
 *    This file contains the headers for a class that implements a PID motor position control
 *    code to drive the motors given inputs of position, velocity, and delta time.
 *    This file sets up "Controller_PID"
 * 
 *  @author Matthew Carlson
 *  @date  2020-Nov-10 Original file
 */



#ifndef CONTROLLER_H
#define CONTROLLER_H

// #include "libraries&constants.h"


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
    float pos_actual;           // comes from the encoder (feedback) (Current)


    // Velocity Desired and Actual:

    float vel_desired;          // comes from the GCode (setpoint/target)
    float vel_actual;           // comes from the encoder (feedback) (Current)

    // Time difference between encoder values. Comes from encoder method (Note the units)
    float delta_time;           // comes from the encoder


    // These are the position and velocity from the previous iteration
    float pos_last;
    float vel_last;

    // The error will be computed as the difference between the Desired and the Actual
    float pos_error;
    float pos_error_last;

    float vel_error;
    float vel_error_last;

    // Used to calculate the I part of the PID controller
    float integral_cumulation;
    float integral_cumulation_max;

    // Used to calculate the D part of the PID controller
    float derivative_per_cycle;

    // Constants that will be used to tune the controller
    float output_filter;
    float output_PWM;

    // PWM signal that the controller outputs to the motor
    float output;

public:

    // ----- Define the Constructor -----

    // Constructor for Controller_P that takes in a gain, position desired, position actual, velocity desired, and velocity actual)
    Controller_PID (float kP, float kI, float kD, float pos_desired_initial, float pos_actual_initial, float vel_desired_initial, float vel_actual_initial, float delta_time_initial);              
    // Note: make these input values equal to the Protected variables in the .cpp file



    // ----- Define the Methods -----

    // The PID Control Loop: takes in current position from encoder and 
    // compares this with the GCode data. Calculate Error and issue
    // the new motor command (a PWM signal).
    void control_loop_PID ();      


    // Set and Get gain values

    // set_gain_PID allows the user to input kP, kI and kD
    void set_gain_PID (float kP, float kI, float kD);

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
 


    // Position Desired and Actual

    // set_pos_desired allows the GCode to continually feed in new position commands
    void set_pos_desired (float pos_desired_initial);       // This will allow the GCode to overwrite it's previous commands

    // get_pos_desired helps with testing
    float get_pos_desired ();

    // set_pos_actual allows the encoder to continually feed in new position observations
    void set_pos_actual (float pos_actual_initial);         // This will allow the encoder to overwrite it's previous commands

    // get_pos_actual helps with testing
    float get_pos_actual ();



    // Velocity Desired and Actual

    // set_vel_desired allows the GCode to continually feed in new velocity commands
    void set_vel_desired (float vel_desired_initial);       // This will allow the GCode to overwrite it's previous commands

    // get_vel_desired helps with testing
    float get_vel_desired ();
    
    // set_vel_actual allows the encoder to continually feed in new velocity observations
    void set_vel_actual (float vel_actual_initial);         // This will allow the encoder to overwrite it's previous commands

    // get_vel_actual helps with testing
    float get_vel_actual ();


    // Output's

    // get_output displays the output from the PID loop
    float get_output ();                                    // Value will be fed to the motors, PWM

    // Used in testing
    // get_pos_error displays the position error from the PID loop
    float get_pos_error ();                                 // Value of position error



};
#endif  //CONTROLLER_H












