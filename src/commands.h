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


///@endcond

//Task function to translate and send out necessary control data
void task_translate(void* p_params);

//function which interprets a machine command
uint8_t interpret_machinecmd_line(char *line);

#endif //COMMANDS_H