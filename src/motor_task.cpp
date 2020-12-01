/** @file motor_task.cpp is the motor driver task function file implementation.
 *        It gets a PWM signal from the controller via a queue and then feeds this
 *        into a motor driver instance (which this code also creates).
 * 
 *  @author Ethan A Czuppa
 *  @author Matthew Carlson
 * 
 *  @date 11 Nov 2020 Created files to test motor driver and encoder code
 *  @date 20 Nov 2020 Wrote first drafts of motor A and motor B driver tasks
 *        NOTE - Ethan your testing code is at the very bottom of the code
 *  @date 21 Nov 2020 Updated the timing and timers (TIM3) and reviewed the code
 */


#include "libraries&constants.h"

// Set up shares and queues

// queue_PWM_motor_A is set up in the controller task
extern Queue<int8_t> queue_PWM_motor_A;                 // This is just the "output" from the controller
// queue_PWM_motor_B is set up in the controller task
extern Queue<int8_t> queue_PWM_motor_B;                 // This is just the "output" from the controller




// Motor A Driver Task

/** @brief   Task to run the left motor, motor A (Motor 1) 
 *  @details This task will interface with the TB6612FNG motor driver in order
 *           to send commands to the left motor (motor A). It will constantly get the output 
 *           from the controller task via a queue.
 *  @param   p_params A pointer to function parameters which we don't use.
 */

void motor_A_driver_task (void* p_params)
{
    (void)p_params;                     // Does nothing but shut up a compiler warning

    // HardwareTimer* MotorTmr = ((HardwareTimer*)p_params);

    uint8_t stby_motor = PC10;          //PC10 = 16     !! Where did this come from?!?!?
    uint8_t mot_dir_1 = PA14;           //PA15 = 21
    uint8_t mot_dir_2 = PA15;           //PA14 = 20
    uint8_t pwm_pin_motor = PA6;        //PA6, predefined arduino pin = A7
    // uint8_t tim_chan_num_motor = 1;     // Channel 1, for timer
    // TIM_TypeDef * a_p_timer = TIM3;     // a pointer to the hardware timer object that is user defined, in this case TIM3

    
    // //Note: TB6612FNG(stby_pin, mot_pin_1, mot_pin_2, a_pwm_pin, a_tim_chan_num, TIM_TypeDef * _p_timer);
    // // Create instance of motor driver called motor_A_driver
    TB6612FNG motor_A_driver (stby_motor, mot_dir_1, mot_dir_2, pwm_pin_motor);
    
    // // Use enable method for the motor driver
    motor_A_driver.enable();
    
    // The is where the duty cycle will be put from when it is read from the queue
    int8_t duty_cycle_motor_A = 75;

    // This is for precise timing
    TickType_t xLastWakeTime = xTaskGetTickCount();

    //Serial << "Motor A is setup" << endl;

    for (;;)
    {
        
        // get duty cycle for Motor A from the controller output and put it into the variable "duty_cycle_motor_A"
        // CHECK: need "if item in queue, then do get it below"
        // if (queue_PWM_motor_A.any())
        // {
        //     //read queue


        //     // Get item from queue
        //     queue_PWM_motor_A.get(duty_cycle_motor_A);
        
            
        //     // send this duty cycle to the motor
        //     motor_A_driver.setDutyCycle(duty_cycle_motor_A); 

        // }
        
        motor_A_driver.setDutyCycle(duty_cycle_motor_A);            // For testing


        // This helps this task run at the correct speed
        vTaskDelayUntil(&xLastWakeTime, motor_period_A);
    }

}





// Motor B Driver Task

/** @brief   Task to run the right motor, motor B (Motor 2) 
 *  @details This task will interface with the TB6612FNG motor driver in order
 *           to send commands to the right motor (motor B). It will constantly get the output 
 *           from the controller task via a queue.
 *  @param   p_params A pointer to function parameters which we don't use.
 */

void motor_B_driver_task (void* p_params)
{
    (void)p_params;                     // Does nothing but shut up a compiler warning

    

    // HardwareTimer* MotorTmr = ((HardwareTimer*)p_params);


                                        // Pin names PC10, etc are defines that live in variant.h
    uint8_t stby_motor = PC10;          // PC10 = 16     
    uint8_t mot_dir_1 = PA11;           // PA11 = 37
    uint8_t mot_dir_2 = PA12;           // PA12 = 36
    uint8_t pwm_pin_motor = PA7;        // PA7, predefined arduino pin = A6
    // uint8_t tim_chan_num_motor = 2;     // Channel 2, for timer    
    //TIM_TypeDef * a_p_timer = TIM3;    

    
    //Note: TB6612FNG(stby_pin, mot_pin_1, mot_pin_2, a_pwm_pin, a_tim_chan_num, TIM_TypeDef * _p_timer);
    // Create instance of motor driver called motor_A_driver
    TB6612FNG motor_B_driver (stby_motor, mot_dir_1, mot_dir_2, pwm_pin_motor);
    

    // Use enable method for the motor driver
    motor_B_driver.enable();
    
    // The is where the duty cycle will be put from when it is read from the queue
    int8_t duty_cycle_motor_B = -75;

    // This is for precise timing
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {



        // get duty cycle for Motor B from the controller output and put it into the variable "duty_cycle_motor_B"
        // CHECK: need "if item in queue, then do get it below"
        // if (queue_PWM_motor_B.any())
        // {
        //     // Get item from queue
        //     queue_PWM_motor_B.get(duty_cycle_motor_B);
        
            
        //     // send this duty cycle to the motor
        //     motor_B_driver.setDutyCycle(duty_cycle_motor_B); 

        // }

        motor_B_driver.setDutyCycle(duty_cycle_motor_B);                // For testing



        // This helps this task run at the correct speed
        vTaskDelayUntil(&xLastWakeTime, motor_period_B);
    }

}
















// // Ethan's original testing code

// TB6612FNG LeftMotor(16, 37, 36, A7, 1, TIM3);
//     LeftMotor.enable();
//     int32_t duty_cycle;
//     uint16_t count = 0;
//     TickType_t xLastWakeTime = xTaskGetTickCount();
//     //Serial << "motor is setup" << endl;

//     for (;;)
//     {
//         s_duty_cycle.get(duty_cycle);
//         LeftMotor.setDutyCycle(duty_cycle); 
//         if (count < 100 )
//         {
//             count++;
//         }
//         else if (count >=1000)
//         {
//             LeftMotor.brake();
            
//         }
//         vTaskDelayUntil(&xLastWakeTime,motor_period);
//     }

// }