// /** @file main.cpp
//  *    This file contains a program to use debouncing to activate limit switches
//  *
//  *  @author  Matthew Carlson
//  * 
//  *  @date    05 Nov 2020 Original file
//  */


// #include <Arduino.h>
// #include <PrintStream.h>
// #if (defined STM32L4xx || defined STM32F4xx)
//     #include <STM32FreeRTOS.h>
// #endif
// #include "debouncer.h"


// /** @brief   Task which runs a debouncer for a button or a switch 
//  *  @details This task uses a Constructor called Debouncer to set up a class called blue_button
//  *          in order to filter out noise that can be caused when pushing a button.
//  *          This program is simulated by hand using the blue button (PC13) on the Nucleo
//  *          at a task called every 500 ms or so. When the task is run quicker (at say 10ms) the 
//  *          blue button behaves very much like a button should.
//  *  @param   p_params A pointer to function parameters which we don't use.
//  */


// void task_debouncer (void* p_params)
// {
//     (void)p_params;                     // Shuts up a compiler warning


//     // ----- Define each switch as a class -----


//     // Make "limit_switch_x" as the switch we want to debounce
//     // Given input pin PB5 and threshhold of 5
//     Debouncer limit_switch_x(PB5, 5);     
//     // Define some output variable that the "update" function can spit out to
//     bool output_limit_switch_x = 0;


//     // Make "limit_switch_y" as the switch we want to debounce
//     // Given input pin PB3 and threshhold of 5
//     Debouncer limit_switch_y(PB3, 5);     
//     // Define some output variable that the "update" function can spit out to
//     bool output_limit_switch_y = 0;


//     for (;;)
//     {
//         // Limit switch x value as a bool
//         output_limit_switch_x = limit_switch_x.update();
        
//         // Limit switch x value as a bool
//         output_limit_switch_y = limit_switch_y.update();


//         // Timing accuracy isn't extremely important, so use the simpler delay
//         vTaskDelay (500);
//     }           

// }


// /** @brief   Arduino setup function which runs once at program startup.
//  *  @details This function sets up a serial port for communication and creates the
//  *           tasks which will be run.
//  */
// // void setup () 
// // {
// //     // Start the serial port, wait a short time, then say hello. Use the
// //     // non-RTOS delay() function because the RTOS hasn't been started yet
// //     Serial.begin (115200);
// //     delay (2000);
// //     Serial << endl << endl << "Running a Debouncer on a button or switch" << endl;

// //     // Create a task which prints a more agreeable message
// //     xTaskCreate (task_debouncer,
// //                  "Yep",
// //                  1256,                             // Stack size
// //                  NULL,
// //                  3,                                // Priority
// //                  NULL);

// //     // If using an STM32, we need to call the scheduler startup function now;
// //     // if using an ESP32, it has already been called for us
// //     #if (defined STM32L4xx || defined STM32F4xx)
// //         vTaskStartScheduler ();
// //     #endif
// // }


// // /** @brief   Arduino's low-priority loop function, which we don't use.
// //  *  @details A non-RTOS Arduino program runs all of its continuously running
// //  *           code in this function after @c setup() has finished. When using
// //  *           FreeRTOS, @c loop() implements a low priority task on most
// //  *           microcontrollers, and crashes on some others, so we'll not use it.
// //  */
// // void loop () 
// // {
// // }