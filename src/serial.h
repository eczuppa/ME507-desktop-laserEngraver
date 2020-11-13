#ifndef SERIAL_H
#define SERIAL_H

#include "libraries&constants.h"

int32_t parseIntWithEcho (Stream& stream);

void task_ui (void* p_params);
const TickType_t UI_period = 20; // UI task's latency

#endif // SERIAL_H