/** @file       libraries&constants.h
 *  @brief      File containing the header which contains the definition of multiple constants
 *              and libraries that will be used with DigitalCarpenter-lite.
 * 
 *      
 *  @author Niko Banks
 *  @date Nov 10 2020
 */

// This code prevents errors if this file is #included more than once
#ifndef LIBRARIES_H
#define LIBRARIES_H

///@cond

//Standard and copied libraries
#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "taskshare.h"
#include "taskqueue.h"
#include "baseshare.h"
#include <HardwareTimer.h>
#include <stdint.h>
#include <pins_arduino.h>



//From Github
#include "BasicLinearAlgebra.h"
#include "DallasTemperature.h"
#include "OneWire.h"


//ME507 Laser Library
#include "control.h"
#include "check_home.h"
#include "debouncer.h"
#include "encoder_task.h"
#include "gcode.h"
#include "Quad_Encoder.h"
#include "TB6612FNG_Driver.h"
#include "motor_task.h"
#include "serial.h"
#include "kinematics.h"
#include "stopwatch.h"
#include "safetySupervisor.h"
#include "motionplanning.h"
#include "commands.h"
#include "temperature_task.h"
#include "control_task.h"

///@endcond
#endif //LIBRARIES_H