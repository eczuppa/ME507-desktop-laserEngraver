/**  @file      control_task.h
 *   @brief     File containing the header for a file containing the @c control_task function.
 * 
 *  @author Niko Banks
 *  @author Ethan Czuppa
 *  @author Matthew Carlson
 *  @date Nov 20 2020 Original file
 */

#ifndef _CONTROL_TASK_H_
#define _CONTROL_TASK_H_

#include "libraries&constants.h"

///@cond
//control_task main states
#define NORMAL_OPERATION 0
#define SAFETY_STOP 1
#define HOMING 2

//Time period for control task
const TickType_t control_task_period = 30;

///@endcond

//Control task function to operate PID controller
void control_task (void* p_params);


#endif // _CONTROL_TASK_H_