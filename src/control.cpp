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

#include <Arduino.h>
#include <PrintStream.h>
#include "control.h"


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

    // Calculate the error between the current setpoint and the encoder position

    // Calculate the motor command


        // ????? WHERE DOES THE ACTUAL CONTROL LOOP GO? The thing that uses kP?
        // Is that under calculating the error or calculating the motor command?

    // Return the result of the motor command
        // ????? Will this result then feed back into the motor, which will 
        // in turn feed back to this function as a encoder reading?
        // I think yes, but maybe not

  
}


/** @brief   Set the controller's gain coefficient kP
 *  @param   gain_kP_new A new value for the controller's gain coefficient, kP
 */
 float Controller_P::set_gain_kP (float gain_kP_new)
 {
    // In order to be able to change the protected variable "gain_kP"
    gain_kP = gain_kP_new;

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