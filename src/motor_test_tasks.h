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
#define CONST_VELOCITY_TEST_MOTOR LASER_CUTTER_MOTOR_BOTH          // LASER_CUTTER_MOTOR_A, LASER_CUTTER_MOTOR_B, LASER_CUTTER_MOTOR_BOTH
#define CONST_VELOCITY_TEST_DC_A 10                             // Value -100 to 100
#define CONST_VELOCITY_TEST_DC_B 10                             // Value -100 to 100
#define CONST_VELOCITY_TEST_MODE CONST_VEL_MODE_INSTANTANEOUS   // CONST_VEL_MODE_INSTANTANEOUS or CONST_VEL_MODE_AVERAGE


// Parameters for path test
#define PATH_TEST_MOTOR LASER_CUTTER_MOTOR_BOTH             // LASER_CUTTER_MOTOR_A, LASER_CUTTER_MOTOR_B, LASER_CUTTER_MOTOR_BOTH
#define PATH_TEST_CONTROL_KP 1
#define PATH_TEST_CONTROL_KI 0.5
#define PATH_TEST_CONTROL_KD 0
#define PATH_TEST_MOTOR_DEADBAND 9      // %
#define PATH_TEST_DRV_FILTER_TAU 0.5    // sec


//Necessary defines for choosing options
#define CONST_VEL_MODE_INSTANTANEOUS 0
#define CONST_VEL_MODE_AVERAGE 1

// Run a test on motor at specific DC, read off average speed
void task_test_const_velocity(void* p_params);

// Run a test on motor and controller to follow a simple path
void task_test_control_path(void* p_params);


void task_single_control_both(void* p_params);

void task_test_gcode_response(void* p_params);
void task_test_script(void* p_params);



#endif //TEST_SINGLE_CONTROL_H