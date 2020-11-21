#ifndef TEMPERATURE_TASK_H
#define TEMPERATURE_TASK_H

#include "libraries&constants.h"

void temperature_task(void* p_params);
const TickType_t temperature_task_period = 100000;

#endif // TEMPERATURE_TASK_H