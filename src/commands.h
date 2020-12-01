/** @file commands.h
 *      This file contains the header for a file containing the functions necessary
 *      to run the full translation of commands coming from the serial port before sending
 *      them to the controller. 
 * 
 *      The full Doxygen header for the function is in the .cpp file, so there is
 *      just a brief description of the function here. 
 *
 *  @author Niko Banks
 *  @date Nov 20 2020
 */

// This code prevents errors if this file is #included more than once
#ifndef COMMANDS_H
#define COMMANDS_H


//Struct to contain all desired values to be sent from the translator to the control task via a queue
struct desired_pos_vel_S
{
    float A_pos = 0;
    float B_pos = 0;
    float A_feed = 0;
    float B_feed = 0;
    float S = 0;
};


//Task function to translate and send out necessary control data
void task_translate(void* p_params);


#endif //COMMANDS_H