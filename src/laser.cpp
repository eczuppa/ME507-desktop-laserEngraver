/** @file laser.cpp
 *      This file contains the code which sets the laser PWM signal. 
 * 
 *  @author  Niko Banks
 *  @date    7-02-2022 File Created
 *  @date    7-02-2022 Last Updated
 * 
*/

#include "libraries&constants.h"


void set_laser_PWM(uint8_t laser_PWM_signal)
{
    //Check to make sure laser signal is within range, correct it if not
    if(laser_PWM_signal>100)
    {
        laser_PWM_signal = 100;
    }
    else if(laser_PWM_signal<0)
    {
        laser_PWM_signal = 0;
    }
    //Convert laser PWM input to 0-255 scale for analogWrite()
    laser_PWM_signal = (float)laser_PWM_signal*255/100;
    
    //Set laser PWM
    analogWrite(L_PWM,laser_PWM_signal);

}


void laser_test_task(void* p_params)
{
    // Account for the length of time it takes to run the task in the task timing requirement
    TickType_t xLastWakeTime = xTaskGetTickCount();

    //Initialize laser % signal
    uint8_t value = 0;

    for(;;)
    {
        //Increment once every task period
        value +=25;

        //If we're past 100, recycle value
        if(value>100)
        {
            value = 0;
        }

        set_laser_PWM(value);
        // Delay until reset        
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
}
