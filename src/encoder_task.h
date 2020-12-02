/** @file       encoder_task.h 
 *  @brief      File that contains the header for encoder_task.cpp.
 * 
 *  @author Matthew Carlson
 *  @author Ethan Czuppa
 * 
 *  @date 22 Nov 2020 Created files for the encoder tasks
 *
 */


#ifndef ENCODERTASK_H
#define ENCODERTASK_H

#include "libraries&constants.h"

///@cond
//Encoder A task period
const TickType_t encoder_period_A = 2;          // According to the task diagram this is only 0.3 ms

//Encoder B task period
const TickType_t encoder_period_B = 2;          // According to the task diagram this is only 0.3 ms

///@endcond

// Encoder A Task
void encoder_A_task (void* p_params);

// Encoder B Task
void encoder_B_task (void* p_params);






#endif // ENCODERTASK_H