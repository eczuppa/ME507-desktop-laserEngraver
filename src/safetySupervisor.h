/** @file       safetySupervisor.h
 *  @brief      Contains the safety supervisor header file that sets up the safety task.   
 * 
 * 
 *  @author  Niko Banks, Matthew Carlson, Ethan Czuppa
 *  @date    Nov 5 2020 Original file
 */


#ifndef _SAFETYSUPERVISOR_H
#define _SAFETYSUPERVISOR_H

#include "libraries&constants.h"
void safety_task (void* p_params);
const TickType_t safety_period = 100;                   // Try not to make this less than 50 (5ms) if possible cause the code doesn't like it    

#endif // _SAFETYSUPERVISOR_H
