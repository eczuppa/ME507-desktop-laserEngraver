/** @file TEST_single_control.cpp
 *      This file is a test controller that will control a single motor.
 *
 *  @author  Niko Banks
 *  @date    3-23-2021 File Created
 *  @date    3-24-2021 Last Updated
 * 
*/

#include "libraries&constants.h"

// Shares for Encoder B
extern Share<float> encoder_B_pos;
extern Share<float> encoder_B_velocity;
extern Share<uint32_t> encoder_B_dt;

/** @brief      Task which moves a single motor.
 *  @details    Fill in later
 *  @param      p_params A pointer to function parameters which we don't use.
 */
void task_single_control(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning

    // Set up instance of the motor driver class
    TB6612FNG Motor_B(STBY,BIN2,BIN1,PWM_B);

    //Set up motor and set duty cycle
    Motor_B.enable();
    float DC = 100;

    //Set up variables for encoder
    float B_pos;
    float B_vel;
    // uint32_t B_dt;

    //Set the duty cycle
    Motor_B.setDutyCycle(DC);

    print_serial("Motor B Initialized\n");

    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity
        encoder_B_pos.get(B_pos);
        encoder_B_velocity.get(B_vel);

        //Print the encoder positions and velocity
        print_serial("Position:  ");   print_serial(B_pos);
        print_serial("  Velocity:  "); print_serial(B_vel);
        print_serial("                                \r");


        vTaskDelay(500);
    }
}

