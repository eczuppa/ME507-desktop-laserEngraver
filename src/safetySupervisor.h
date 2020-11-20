
#ifndef _SAFETYSUPERVISOR_H
#define _SAFETYSUPERVISOR_H

#include "libraries&constants.h"
void safety_task (void* p_params);
const TickType_t safety_period = 100; 

#endif // _SAFETYSUPERVISOR_H
