/** @file motorparam.cpp motor driver and encoder task function file implementation
 * 
 *  @author Ethan A Czuppa
 * 
 *  @date 11 Nov 2020 Created files to test motor driver and encoder code
 *
 */


#include "libraries&constants.h"
#include <variant.h>

extern Share<int32_t> s_duty_cycle;
Queue<int32_t> encoder_queue (100,"position");

void motor_task (void* p_params)
{
    (void)p_params;
    // uint8_t stby = 16;  //PC10
    // uint8_t mot_1 = 37; //PA11
    // uint8_t mot_2 = 36; //PA12
    // uint8_t pwm_pin = A7;  //PA6, predefined arduino pin
    // uint8_t tim_chan_num = 1;
    // TIM_TypeDef * a_p_timer = TIM3;
    


    TB6612FNG LeftMotor(16, 37, 36, A7, 1, TIM3);
    int32_t duty_cycle;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        s_duty_cycle.get(duty_cycle);
        LeftMotor.setDutyCycle(duty_cycle); 
        vTaskDelayUntil(&xLastWakeTime,motor_period);
    }

}

void encoder_task (void* p_params)
{
    // uint8_t L_enc_sigpin_A = 39; // PC7
    // uint8_t L_enc_sigpin_B = 38; // PC6
    // TIM_TypeDef *a_p_eTIM = TIM8;
    Quad_Encoder LeftEncoder (39, 38, TIM8);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    int64_t a_position;

    for (;;)
    {
        a_position = LeftEncoder.enc_read();
        encoder_queue.put(a_position);
        vTaskDelayUntil(&xLastWakeTime,encoder_period);
    }

}



