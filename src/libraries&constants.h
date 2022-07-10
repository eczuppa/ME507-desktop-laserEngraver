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

// ---------------------------------------------------------------------------------
// ------------------------- Standard and copied libraries -------------------------
// ---------------------------------------------------------------------------------
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
// #include "BasicLinearAlgebra.h"
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
// #include "kinematics.h"
#include "stopwatch.h"
#include "safetySupervisor.h"
// #include "motionplanning.h"
#include "temperature_task.h"
#include "control_task.h"
#include "motor_test_tasks.h"
#include "translate.h"
#include "test_script.h"
#include "laser.h"

// ---------------------------------------------------------------------------------
// -------------------- Define pin names as routed on the board -------------------- 
// ---------------------------------------------------------------------------------

// Motor Direction Input Pins
#define AIN1 PC11 
#define AIN2 PD2
#define BIN1 PB12
#define BIN2 PB6

//Motor Encoder Pins:
#define A_C1 PC6
#define A_C2 PC7
#define B_C1 PA9
#define B_C2 PA8

//GPIO Pin Bank:
#define LASER_GPIO_2 PC8
#define LASER_GPIO_3 PC9
#define LASER_GPIO_4 PC11
#define LASER_GPIO_5 PA12
#define LASER_GPIO_6 PC5
#define LASER_GPIO_7 PB9
#define LASER_GPIO_8 PB8

//PWM output pins:
#define PWM_A PA0
#define PWM_B PB11
// #define L_PWM PB0
#define L_PWM pinNametoDigitalPin(PB_0_ALT2)


//ADC Pin Bank:
#define LASER_ADC_2 PA1
#define LASER_ADC_3 PA4
#define LASER_ADC_4 PC1
#define LASER_ADC_5 PC0
       
//Additional Pins:
#define L_TEMP PC3
#define TEMP_SIG PC2
#define FAN_SIG PC10
#define STBY PA7


//Motor calls:
#define LASER_CUTTER_MOTOR_A 0
#define LASER_CUTTER_MOTOR_B 1
#define LASER_CUTTER_MOTOR_BOTH 2

// ---------------------------------------------------------------------------------

///@endcond

#endif //LIBRARIES_H