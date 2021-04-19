/** @file       TEST_single_control.h
 *  @brief      This file contains the header for the single control file.
 *              
 *  
 *  @author Niko Banks
 *  @date    3-23-2021 File Created
 *  @date    3-24-2021 Last Updated
 * 
 */

#ifndef TEST_SINGLE_CONTROL_H
#define TEST_SINGLE_CONTROL_H

#include "libraries&constants.h"


#define CONTROLLER_A_KP 100
#define CONTROLLER_A_KI 0
#define CONTROLLER_A_KD 0

void task_single_control_B(void* p_params);
void task_single_control_A(void* p_params);
void task_single_control_both(void* p_params);


#endif //TEST_SINGLE_CONTROL_H