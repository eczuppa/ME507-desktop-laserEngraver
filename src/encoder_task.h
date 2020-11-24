/** @file encoder_task.h is the encoder task function file header
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

// Encoder A Task
void encoder_A_task (void* p_params);

const TickType_t encoder_period_A = 3;          // According to the task diagram this is only 0.3 ms


// Encoder B Task
void encoder_B_task (void* p_params);

const TickType_t encoder_period_B = 3;          // According to the task diagram this is only 0.3 ms




#endif // ENCODERTASK_H