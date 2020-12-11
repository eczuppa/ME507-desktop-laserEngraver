/** @file   temperature_task.cpp
 *  @brief  File that contains the temperature task function file implementation.
 * 
 *  @author Ethan A Czuppa
 *  @date Nov 10 2020 Original file
 */


// extern Queue<float> temperature_data;
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
    uint8_t resolution = 9;                         // Set flame sensor resolution to 10 bits
    uint8_t one_wire_bus_pin = PB11;                //  OneWire Bus Pin
    OneWire temp_Sens(one_wire_bus_pin);            //  OneWire Class Instance
    DallasTemperature Flame_sens(&temp_Sens);       //  Dallas Temperature Class Instance for Flame Sensor
     
    Flame_sens.setResolution(resolution);               
    uint16_t sens_delay_ms = 750 / (1<<(12-resolution));  // set wait for conversion delay 

    // Start all sensors attached to the bus
    Flame_sens.begin();
    // set user alarms for high and low temperature
    
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