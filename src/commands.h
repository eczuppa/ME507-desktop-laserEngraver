/** @file       commands.h
 *  @brief      File containing the header for commands.cpp.
 *  @details    This file contains the header for a file containing the functions necessary
 *              to run the full translation of commands coming from the serial port before sending
 *              them to the controller. 
 * 
 *  @author Niko Banks
 *  @date Nov 20 2020
 */

// This code prevents errors if this file is #included more than once
#ifndef COMMANDS_H
#define COMMANDS_H

///@cond
//Define Machine Commands
#define MACHINE_CMD_NULL 0
#define MACHINE_CMD_HOME 1

// Queue size for ramp segment coefficient structs
#define RAMP_COEFFICIENT_Q_SIZE 32

//Struct to contain all desired values to be sent from the translator to the control task via a queue
struct ramp_segment_coefficients
{
    float t0 = 0;       //Initial time of ramp segment
    float t_end = 0;    //End time of ramp segment
    float pos_A0 = 0;   //Initial A position
    float pos_B0 = 0;   //Initial B position
    float vel_A = 0;    //Velocity of A
    float vel_B = 0;    //Velocity of B
    float S = 0;        //Laser PWM signal
};
///@endcond

//Task function to translate and send out necessary control data
void task_translate(void* p_params);

//function which interprets a machine command
uint8_t interpret_machinecmd_line(char *line);

#endif //COMMANDS_H