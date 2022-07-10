/** @file       test_script.cpp 
 *  @brief      File that is used for testing things quickly. 
 *  
 *  @author Niko Banks
 * 
 *  @date 02 Jul 2022 Initial creation.
 * 
 */


#include "libraries&constants.h"


// setup

void test_script_task(void* p_params)
{

    // Account for the length of time it takes to run the task in the task timing requirement
    TickType_t xLastWakeTime = xTaskGetTickCount();

    TIM_TypeDef *timer_name = TIM3;

    HardwareTimer *EncTmr8;
    
    EncTmr8 = new HardwareTimer(timer_name); 

    pinMode(A_C1,INPUT);
    pinMode(A_C2,INPUT);

    EncTmr8 -> pause();

    EncTmr8 -> setMode(1, TIMER_INPUT_CAPTURE_RISING, A_C1);  // Puts the timer channel A into input capture falling and rising edge detection mode
    EncTmr8 -> setMode(2, TIMER_INPUT_CAPTURE_RISING, A_C2);  // Puts the timer channel B into input capture falling and rising edge detection mode

    EncTmr8 -> setCount(0);


    timer_name -> SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; // both of the timer channels used enabled to increment the count in the timer counter register
    timer_name -> CR1  |= TIM_CR1_CEN;  // start the actual timer

    uint16_t _lastcount = EncTmr8 -> getCount();

    Serial << "Test Script Variables Initialized" << endl;

    set_laser_PWM(50);

    for(;;)
    {
        uint16_t count = EncTmr8 -> getCount();

        Serial << "Encoder Count:" << count << endl;




        // Delay until reset        
        vTaskDelayUntil(&xLastWakeTime, 100);
    }
}

