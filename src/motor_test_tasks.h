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


// Parameters for constant velocity test
#define CONST_VELOCITY_TEST_MOTOR 'A'       // 'A' or 'B'
#define CONST_VELOCITY_TEST_DC 50           // Value -100 to 100
#define CONST_VELOCITY_TEST_MODE 0          // 0 = instantaneous velocity, 1 = average velocity


// Parameters for path test
#define PATH_TEST_MOTOR 'B'             // 'A' or 'B'
#define PATH_TEST_CONTROL_KP 10
#define PATH_TEST_CONTROL_KI 1
#define PATH_TEST_CONTROL_KD 0
#define PATH_TEST_MOTOR_DEADBAND 5      // %



// Run a test on motor at specific DC, read off average speed
void task_test_const_velocity(void* p_params);

// Run a test on motor and controller to follow a simple path
void task_test_control_path(void* p_params);


void task_single_control_both(void* p_params);
void task_test_script(void* p_params);



#endif //TEST_SINGLE_CONTROL_H