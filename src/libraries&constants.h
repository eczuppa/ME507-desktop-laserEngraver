/** @file gcode.h
 *      This file contains the header which contains the definition of multiple constants
 *      and libraries that will be used in this laser code. 
 * 
 *      The full Doxygen header for the function is in the .cpp file, so there is
 *      just a brief description of the function here. 
 *
 *  @author Niko Banks
 *  @date Nov 10 2020
 */

// This code prevents errors if this file is #included more than once
#ifndef LIBRARIES_H
#define LIBRARIES_H


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


//ME507 Laser Library
#include "control.h"
#include "debouncer.h"
#include "gcode.h"
#include "Quad_Encoder.h"
#include "TB6612FNG_Driver.h"
#include "nuts_bolts.h"
#include "motorparam.h"
#include "serial.h"
#include "limit_switch.h"
#include "kinematics.h"
#include "stopwatch.h"
#include "safetySupervisor.h"
#include "motionplanning.h"
#include "commands.h"
#include "temperature_task.h"



#endif //LIBRARIES_H