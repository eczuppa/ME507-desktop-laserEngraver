
#include "libraries&constants.h"

//setup externs for incoming shares and queues here

// MOTOR A ENCODER DATA
extern Share<float> encoder_A_pos; 
extern Share<float> encoder_A_velocity;
extern Share<uint32_t> encoder_A_dt;

// MOTOR B ENCODER DATA
extern Share<float> encoder_B_pos;
extern Share<float> encoder_B_velocity;
extern Share<uint32_t> encoder_B_dt;

//Setup Outgoing Shares and Queues Here

// MOTOR A&B PWM COMMANDS
Queue<float> queue_PWM_motor_A (8,"Motor A DutyCycle");
Queue<float> queue_PWM_motor_B (8,"Motor B DutyCycle");


void control_task(void* p_params)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {

        
        
        
        
        
        
        
        
        
        
        
        
        vTaskDelayUntil(&xLastWakeTime, control_task_period);
    }
}