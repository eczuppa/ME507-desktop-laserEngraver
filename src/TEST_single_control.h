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


#define CONTROLLER_A_KP 10
#define CONTROLLER_A_KI 1
#define CONTROLLER_A_KD 0
#define MOTOR_DEADBAND 5



void task_single_control_B(void* p_params);
void task_single_control_A(void* p_params);
void task_single_control_both(void* p_params);
void task_test_script(void* p_params);


struct motor_setpoint
{
    float A_pos = 0;
    float B_pos = 0;
    float A_vel = 0;
    float B_vel = 0;
};

// Different way to motion plan- give any desired position for the time specified
class setpoint_of_time
{
    protected:
    ramp_segment_coefficients _seg_coeff;

    public: 
    //Contstuctor of the class
    setpoint_of_time(void);

    motor_setpoint get_desired_pos_vel(float time);                 //Get desired position and velocity
};



#endif //TEST_SINGLE_CONTROL_H