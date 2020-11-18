#ifndef SERIAL_H
#define SERIAL_H

#include "libraries&constants.h"

int32_t parseIntWithEcho (Stream& stream);

void task_ui (void* p_params);
const TickType_t UI_period = 10; // UI task's latency

void echo_serial(void* p_params);

void print_serial(void* p_params);

#endif // SERIAL_H