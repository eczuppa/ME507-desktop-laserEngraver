#include "temperature_task.h"

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
       // state 0: get data/wait for conversion
       // state 1: look for alarm flag (on high temp side) - if alarm set flag in share to safety task
       // state 2: if no alarm get temperature and store in something (two vars one for current and last to do rate of rise) use task delay period as delta T(?)
       //         
       // state 3: 

        vTaskDelay(temperature_task_period);
    }
}