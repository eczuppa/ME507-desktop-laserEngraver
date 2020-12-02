/** @file   temperature_task.cpp
 *  @brief  File that contains the temperature task function file implementation.
 * 
 *  @author Ethan A Czuppa
 *  @date Nov 10 2020 Original file
 */


#include "temperature_task.h"

/** @brief   Task to detect flame-out conditions on the laser. 
 *  @details This task will interface with a DS18B20 Onewire (c) digital 
 *           temperature sensor to detect sudden rises in temperature.
 * 
 *  @param   p_params A pointer to function parameters which we don't use.
 */

void temperature_task (void* p_params)
{
    // instantiate a class for one wire, instantiate the bus
    // declare pin for onewire bus on digital thermometer
    // set user alarms for high and low temperature
    // set desired resolution
    // initialize state machine (the state variable to start condition)
    // 
    for (;;)
    {
       // State machine pseudo code
       // state 0: wait for conversion - if data transistion to state 1
       // state 1: get data and calculate the rate of rise over the measured time period with a StopWatch. and queue data to safety supervisor go back to state 0.
       

        vTaskDelay(temperature_task_period);
    }
}