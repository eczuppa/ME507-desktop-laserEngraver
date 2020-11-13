
#include <stdint.h>
#include <Arduino.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include <PrintStream.h>

#ifndef SERIAL_H
#define SERIAL_H

int32_t parseIntWithEcho (Stream& stream);

void task_ui (void* p_params);
const TickType_t UI_period = 100; // UI task's latency

#endif // SERIAL_H