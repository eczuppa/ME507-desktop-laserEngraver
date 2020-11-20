/** @file serial.h
 *    This file contains the header for the file containing functions that allow for 
 *    communication through the serial port. 
 * 
 *    The full Doxygen header for the function is in the .cpp file, so there is
 *    just a brief description of the function here. 
 *
 *  @author Niko Banks
 *  @author Ethan Czuppa
 *  @date   Nov 10 2020
 */

// This code prevents errors if this file is #included more than once
#ifndef SERIAL_H
#define SERIAL_H

#include "libraries&constants.h"

#define LINE_BUFFER_SIZE 80


//Function to read incomming messages from the serial port
void task_read_serial(void* p_params);

//Function to write outgoing messages to the serial port
void task_print_serial(void* p_params);

//Function to add items to the serial print queue to be executed by the printing task function
void print_serial(String string_to_print);
void print_serial(float printed_float);
void print_serial(char string_to_print[LINE_BUFFER_SIZE]);
void print_serial(char printed_char);
void print_serial(const char* printed_char);



//Function to parse ints sent in the serial port
int32_t parseIntWithEcho (Stream& stream);

//Function to ask for inputs for encoder
void task_ui (void* p_params);
const TickType_t UI_period = 100; // UI task's latency


#endif // SERIAL_H