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

class Controller_P
{
protected:

    // The value of the gain for the Proportional part of the Controller: kP
    float gain_kP;

    // The value for the Setpoint, this is the commanded position from GCode
    float setpoint;                   // potentially make this a large integer instead of a float

    // Will this need to be a 1X2 array? setpoint_P: [x, y]
    // Do I need to call this "setpoint_P"?
    // I don't think I need to cause it's protected within this class, right?

public:

    // ----- Define the Constructor -----

    // Constructor for Controller_P that takes in a gain and setpoint
    Controller_P (float kP_gain, float setpoint_initial);              
    // Note: make these input values equal to the Protected variables in the .cpp file


    // ----- Define the Methods -----

    // The P Control Loop: takes in current position from encoder and 
    // compares this with the setpoint data. Calculate Error and calculate
    // the new motor command. Return the new motor command as a float.
    float control_loop_P (float encoder_position);      
    
    // Note: encoder_position is the position calculated from the encoder
    // We will probably need to have a encoder_position be an array:
    // [encoder_position_x, encoder_position_y]


    // set_gain_kP allows the user to input a new gain value. Allows us to change the Protected variable
    float set_gain_kP (float gain_kP_new);        // This will be for testing only

    // change_setpoint_kp allows the user to input a new gain value. Allows us to change the Protected variable
    float change_setpoint_P (float setpoint_new);        // This will allow the GCode to overwrite the initial setpoint

};








/** @brief   Class which implements a PI controller that can be used to drive the motors
 *  @details Defines Protected variables that are used in the control.cpp file with the
 *           "Controller_PI" class.
 *           Also sets up contructor "Controller_PI" and method's "control_loop_PI", 
 *           "set_gain_kP", "set_gain_kI" and "change_setpoint_PI"
 * 
 */

class Controller_PI
{
protected:

    // The value of the gain for the Proportional part of the Controller: kP
    float gain_kP;

    // The value of the gain for the Integal part of the Controller: kI
    float gain_kI;

    // The value for the Setpoint, this is the commanded position from GCode
    float setpoint;                   // potentially make this a large integer instead of a float

    // Will this need to be a 1X2 array? setpoint_PI: [x, y]
    // Do I need to call this "setpoint_PI"?
    // I don't think I need to cause it's protected within this class, cheers?

public:

    // ----- Define the Constructor -----

    // Constructor for Controller_P that takes in a gain and setpoint
    Controller_PI (float kP_gain, float kI_gain, float setpoint_initial);              
    // Note: make these input values equal to the Protected variables in the .cpp file


    // ----- Define the Methods -----

    // The PI Control Loop: takes in current position from encoder and 
    // compares this with the setpoint data. Calculate Error and calculate
    // the new motor command. Return the new motor command as a float.
    float control_loop_PI (float encoder_position);      
    
    // Note: encoder_position is the position calculated from the encoder
    // We will probably need to have a encoder_position be an array:
    // [encoder_position_x, encoder_position_y]


    // set_gain_kP allows the user to input a new gain value for the proportional 
    // part of the controller. Allows us to change the Protected variable.
    float set_gain_kP (float gain_kP_new);        // This will be for testing only

    // set_gain_kI allows the user to input a new gain value for the integral 
    // part of the controller. Allows us to change the Protected variable.
    float set_gain_kI (float gain_kI_new);        // This will be for testing only
 
    // change_setpoint_PI allows the user to input a new gain value. Allows us to 
    // change the Protected variable. The GCode will continually overwrite this value
    float change_setpoint_PI (float setpoint_new);        // This will allow the GCode to overwrite the initial setpoint

};









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

    // The value for the Setpoint, this is the commanded position from GCode
    float setpoint;                     // potentially make this a large integer instead of a float

    // Will this need to be a 1X2 array? setpoint_PI: [x, y]
    // Do I need to call this "setpoint_PID"?
    // I don't think I need to cause it's protected within this class, yeah?

public:

    // ----- Define the Constructor -----

    // Constructor for Controller_P that takes in a gain and setpoint
    Controller_PID (float kP_gain, float kI_gain, float kD_gain, float setpoint_initial);              
    // Note: make these input values equal to the Protected variables in the .cpp file


    // ----- Define the Methods -----

    // The PI Control Loop: takes in current position from encoder and 
    // compares this with the setpoint data. Calculate Error and calculate
    // the new motor command. Return the new motor command as a float.
    float control_loop_PID (float encoder_position);      
    
    // Note: encoder_position is the position calculated from the encoder
    // We will probably need to have a encoder_position be an array:
    // [encoder_position_x, encoder_position_y]


    // set_gain_kP allows the user to input a new gain value for the proportional 
    // part of the controller. Allows us to change the Protected variable.
    float set_gain_kP (float gain_kP_new);        // This will be for testing only

    // set_gain_kI allows the user to input a new gain value for the integral 
    // part of the controller. Allows us to change the Protected variable.
    float set_gain_kI (float gain_kI_new);        // This will be for testing only
 
    // set_gain_kD allows the user to input a new gain value for the integral 
    // part of the controller. Allows us to change the Protected variable.
    float set_gain_kD (float gain_kD_new);        // This will be for testing only

    // change_setpoint_PID allows the user to input a new gain value. Allows us to 
    // change the Protected variable. The GCode will continually overwrite this value
    float change_setpoint_PID (float setpoint_new);        // This will allow the GCode to overwrite the initial setpoint


 
};