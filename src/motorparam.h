/** @file motorparam.h motor driver and encoder task function file header
 * 
 *  @author Ethan A Czuppa
 * 
 *  @date 11 Nov 2020 Created files to test motor driver and encoder code
 *
 */
#ifndef MOTORPARAM_H
#define MOTORPARAM_H

#include "libraries&constants.h"

// Motor A Driver Task
void motor_A_driver_task (void* p_params);

const TickType_t motor_period_A = 10;           // This is 1ms (yeah?)



// Motor A Driver Task
void motor_B_driver_task (void* p_params);

const TickType_t motor_period_B = 10;           // This is 1ms (yeah?)






void encoder_task (void* p_params);

const TickType_t encoder_period = 2;




#endif // MOTORPARAM_H