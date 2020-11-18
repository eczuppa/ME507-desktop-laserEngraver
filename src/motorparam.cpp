/** @file motorparam.cpp motor driver and encoder task function file implementation
 * 
 *  @author Ethan A Czuppa
 * 
 *  @date 11 Nov 2020 Created files to test motor driver and encoder code
 *
 */


#include "libraries&constants.h"


extern Share<int32_t> s_duty_cycle;
Queue<float> encoder_queue (600,"position");

// void motor_task (void* p_params)
// {
//     (void)p_params;
//     // uint8_t stby = 16;  //PC10
//     // uint8_t mot_1 = 37; //PA11
//     // uint8_t mot_2 = 36; //PA12
//     // uint8_t pwm_pin = A7;  //PA6, predefined arduino pin
//     // uint8_t tim_chan_num = 1;
//     // TIM_TypeDef * a_p_timer = TIM3;
    


//     TB6612FNG LeftMotor(16, 37, 36, A7, 1, TIM3);
//     LeftMotor.enable();
//     int32_t duty_cycle;
//     TickType_t xLastWakeTime = xTaskGetTickCount();
//     //Serial << "motor is setup" << endl;

//     for (;;)
//     {
//         s_duty_cycle.get(duty_cycle);
//         LeftMotor.setDutyCycle(duty_cycle); 
//         vTaskDelayUntil(&xLastWakeTime,motor_period);
//     }

// }

void encoder_task (void* p_params)
{
    // uint8_t L_enc_sigpin_A = 9; // PC7
    
    // uint8_t L_enc_sigpin_B = 34; // PC6

    // TIM_TypeDef *a_p_eTIM = TIM8;
    Quad_Encoder LeftEncoder (PC6, PC7, TIM8);
    LeftEncoder.enc_zero();
    TickType_t xLastWakeTime = xTaskGetTickCount();
    float a_position;
    float a_velocity;
    int32_t a_dt;
    //Serial << LeftEncoder.get_overflow() << LeftEncoder.get_prescale() << endl;

    for (;;)
    {
        // get position, velocity, and change in time from the encoder
        a_velocity = LeftEncoder.enc_read_vel();
        a_dt = LeftEncoder.get_enc_dt();
        a_position = LeftEncoder.enc_read_pos();
        

        // put all those values into their respective queues for printing out (parameterization purposes)
        encoder_queue.put(a_position);
        encoder_queue.put(a_velocity);
        encoder_queue.put(a_dt);
        
        vTaskDelayUntil(&xLastWakeTime,encoder_period);
    }

}



