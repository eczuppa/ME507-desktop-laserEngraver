/** @file       motor_task.h 
 *  @brief      The header for the motor driver task function file
 * 
 *  @author Ethan A Czuppa
 *  @author Matthew Carlson
 * 
 *  @date    Nov 11 2020 Original file
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