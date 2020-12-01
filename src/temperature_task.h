/**  @file temperature_task.h
 *              the header file for the temperature reading task function that attempts to 
 *              detect flame-out conditions on the laser cutter using a DS18B20 Onewire (c)
 *              digital temeprature sensor.
 *   @author Ethan A Czuppa
 *  
 *   @date 22 Nov 2020 Original file
 */

#ifndef TEMPERATURE_TASK_H
#define TEMPERATURE_TASK_H

#include "libraries&constants.h"

/** @brief  implements a task function to use digital temperature sensor like an industrial heat detector
 *  @details  uses the OneWire library and DallasTemperature library to interface with the a DS18B20 
 *            digital temperature sensor that is looking at the rate of rise in temperature over a sustained
 *            period of time. This task sets up and runs the temperature sensor in this configuration and queues the
 *            data to the safety supervisor task function. 
 */

void temperature_task(void* p_params);
const TickType_t temperature_task_period = 100000;  

#endif // TEMPERATURE_TASK_H