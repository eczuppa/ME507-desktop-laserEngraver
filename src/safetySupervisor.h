/** @file safetySupervisor.h
 * 
 * 
 */


#ifndef _SAFETYSUPERVISOR_H
#define _SAFETYSUPERVISOR_H

#include "libraries&constants.h"
void safety_task (void* p_params);
const TickType_t safety_period = 100;                   // Try not to make this less than 50 (5ms) if possible cause the code doesn't like it    

#endif // _SAFETYSUPERVISOR_H
