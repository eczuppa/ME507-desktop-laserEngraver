/**  @file control_task.h
 *    This file contains the header for a file containing the control_task function.
 * 
 *    The full Doxygen header for the function is in the .cpp file, so there is
 *    just a brief description of the function here. 
 */

#ifndef _CONTROL_TASK_H_
#define _CONTROL_TASK_H_

#include "libraries&constants.h"


//control_task main states
#define NORMAL_OPERATION 0
#define SAFETY_STOP 1


//Control task function to operate PID controller
void control_task (void* p_params);

//Time period for control task
const TickType_t control_task_period = 30;



#endif // _CONTROL_TASK_H_