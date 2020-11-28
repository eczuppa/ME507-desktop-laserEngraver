/** @file serial.h
 *    This file contains the header for the file containing functions that allow for 
 *    communication through the serial port. 
 * 
 *    The full Doxygen header for each of the functions is in the .cpp file, so there is
 *    just a brief description of the functions here. 
 *
 *  @author Niko Banks
 *  @author Ethan Czuppa
 *  @date   Nov 10 2020
 */

// This code prevents errors if this file is #included more than once
#ifndef SERIAL_H
#define SERIAL_H

#include "libraries&constants.h"

//Line buffers
#define LINE_BUFFER_SIZE 80
#define READ_Q_SIZE 32
#define WRITE_Q_SIZE 32
#define PAUSE_Q_LIMIT 4

//States of the reader
#define READY 0
#define READING 1
#define NOT_READY 2


//Function to read incomming messages from the serial port
void task_read_serial(void* p_params);

//Function to write outgoing messages to the serial port
void task_print_serial(void* p_params);

//Function to add items to the serial print queue to be executed by the printing task function
void print_serial(String string_to_print);
void print_serial(float printed_float);
void print_serial(uint8_t printed_int);
void print_serial(char *printed_char);
void print_serial(char printed_char);
void print_serial(const char* printed_char);




//Function to parse ints sent in the serial port
int32_t parseIntWithEcho (Stream& stream);

//Function to ask for inputs for encoder
void task_ui (void* p_params);
const TickType_t UI_period = 100; // UI task's latency


#endif // SERIAL_H