/** @file main.cpp
 *    This file contains a program to use debouncing to activate limit switches
 *
 *  @author  Matthew Carlson
 * 
 *  @date    05 Nov 2020 Original file
 */


#include "libraries&constants.h"


/** @brief   Task which runs a debouncer for a button or a switch 
 *  @details This task uses a Constructor called Debouncer to set up a class called blue_button
 *          in order to filter out noise that can be caused when pushing a button.
 *          This program is simulated by hand using the blue button (PC13) on the Nucleo
 *          at a task called every 500 ms or so. When the task is run quicker (at say 10ms) the 
 *          blue button behaves very much like a button should.
 *  @param   p_params A pointer to function parameters which we don't use.
 */


void task_debouncer (void* p_params)
{
    (void)p_params;                     // Shuts up a compiler warning


    // ----- Define each switch as a class -----


    // Make "limit_switch_x" as the switch we want to debounce
    // Given input pin PB5 and threshhold of 5
    Debouncer limit_switch_x(PB5, 5);     
    // Define some output variable that the "update" function can spit out to
    bool output_limit_switch_x = false;


    // Make "limit_switch_y" as the switch we want to debounce
    // Given input pin PB3 and threshhold of 5
    Debouncer limit_switch_y(PB3, 5);     
    // Define some output variable that the "update" function can spit out to
    bool output_limit_switch_y = false;


    for (;;)
    {
        // Limit switch x value as a bool
        output_limit_switch_x = limit_switch_x.update();
        
        // Limit switch x value as a bool
        output_limit_switch_y = limit_switch_y.update();


        // Timing accuracy isn't extremely important, so use the simpler delay
        vTaskDelay (500);
    }           

}


// task creation declaration which will be migrated to main.

//     // Create a task which prints a more agreeable message
//     xTaskCreate (task_debouncer,
//                  "Yep",
//                  1256,                             // Stack size
//                  NULL,
//                  3,                                // Priority
//                  NULL);




