/** @file       encoder_task.h 
 *  @brief      File that contains the header for encoder_task.cpp.
 * 
 *  @author Matthew Carlson
 *  @author Ethan Czuppa
 * 
 *  @date 22 Nov 2020 Created files for the encoder tasks
 *
 */


#ifndef ENCODERTASK_H
#define ENCODERTASK_H

#include "libraries&constants.h"

///@cond

#define FILTER_A_ALPHA 0.5
#define FILTER_B_ALPHA 0.5

// Modes for converting units
#define ENC_POSITION_MODE_TICKS 0
#define ENC_POSITION_MODE_DEGOUT 1
#define ENC_POSITION_MODE_REVOUT 2
#define ENC_POSITION_MODE_BELT_MM 3

#define ENC_VELOCITY_MODE_TICKS_PER_SEC 4
#define ENC_VELOCITY_MODE_DEGOUT_PER_SEC 5
#define ENC_VELOCITY_MODE_RPMOUT 6
#define ENC_VELOCITY_MODE_BELT_MM_PER_SEC 7


// Constants as defined by system
#define ENCODER_PULSES_PER_REV 11       //Our encoder specifically has 11 pulses per rev
#define ENCODER_COUNTS_PER_PULSE 4      //Quadrature encoder reads 4 counts (ticks) for each full set of pulses from each encoder channel
#define REV_ENC_PER_REVOUT_MOTOR 6.3    //Gear ratio of motor
#define OUTPUT_WHEEL_RADIUS_MM 5.95     //Radius of belt wheel on output shaft (mm)

// Task timing constants
#define ENCODER_PERIOD_A 10     //Encoder A task period
#define ENCODER_PERIOD_B 10     //Encoder B task period


// Struct for containing encoder output values
struct encoder_output
{
    float pos = 0;
    float vel = 0;
    float time = 0;
};

///@endcond

// Encoder A Task
void task_encoder_A (void* p_params);

// Encoder B Task
void task_encoder_B (void* p_params);

// ============ SUBFUNCTIONS ============ 

//Convert units function to change from encoder ticks to desired units
float convert_units(float value, uint8_t convert_mode);

//Update total time depending on the timing mode
float update_total_time(float total_time, uint32_t delta_time);


#endif // ENCODERTASK_H