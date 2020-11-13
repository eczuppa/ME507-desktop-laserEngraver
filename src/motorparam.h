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

void motor_task (void* p_params);

const TickType_t motor_period = 100;



void encoder_task (void* p_params);

const TickType_t encoder_period = 200;

#endif // MOTORPARAM_H