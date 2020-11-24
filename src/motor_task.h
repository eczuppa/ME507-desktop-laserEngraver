/** @file motor_task.h is motor driver task function file header
 * 
 *  @author Ethan A Czuppa
 *  @author Matthew Carlson
 * 
 *  @date 11 Nov 2020 Created files to test motor driver and encoder code
 *
 */
#ifndef MOTORTASK_H
#define MOTORTASK_H

#include "libraries&constants.h"

// Motor A Driver Task
void motor_A_driver_task (void* p_params);

const TickType_t motor_period_A = 10;           // This is 1ms as according to the task diagram



// Motor A Driver Task
void motor_B_driver_task (void* p_params);

const TickType_t motor_period_B = 10;           // This is 1ms as according to the task diagram


#endif // MOTORTASK_H