/** @file       laser.h
 *  @brief      This file contains the header for the laser.cpp file.
 *              
 *  
 *  @author Niko Banks
 *  @date    7-02-2022 File Created
 *  @date    7-02-2022 Last Updated
 * 
 */
#include "libraries&constants.h"

#ifndef LASER_H
#define LASER_H

//Laser initializing function
void initialize_laser(void);

//Set the laser PWM signal to a percentage
void set_laser_PWM(uint8_t laser_PWM_signal);

//Laser hardcode task for testing
void laser_test_task(void* p_params);


#endif //LASER_H