/** @file motorparam.cpp motor driver and encoder task function file implementation
 * 
 *  @author Ethan A Czuppa
 * 
 *  @date 11 Nov 2020 Created files to test motor driver and encoder code
 *
 */


#include "libraries&constants.h"
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif



void motor_task (void* p_params)
{
    (void)p_params;
    // uint8_t stby = 42;  //PC10
    // uint8_t mot_1 = 37; //PA11
    // uint8_t mot_2 = 36; //PA12
    // uint8_t pwm_pin = 6;  //PA6
    // uint8_t tim_chan_num = 1;
    // TIM_TypeDef * a_p_timer = TIM3;

    TB6612FNG LeftMotor(42, 37, 36, 6, 1, TIM3);

    for (;;)
    {



        vTaskDelay(motor_period);

    }

}

void encoder_task (void* p_params)
{
    // uint8_t L_enc_sigpin_A = 39; // PC7
    // uint8_t L_enc_sigpin_B = 38; // PC6
    // TIM_TypeDef *a_p_eTIM = TIM8;
       Quad_Encoder LeftEncoder (39, 38, TIM8);

    for (;;)
    {

        vTaskDelay(encoder_period);
    }

}



