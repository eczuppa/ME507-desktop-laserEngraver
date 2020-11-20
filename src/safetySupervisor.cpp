/** @file safetySupervisor.cpp safety task function. 
 *
 *  @author  Niko Banks, Matthew Carlson, Ethan Czuppa
 * 
 *  @date    05 Nov 2020 Original file
 */

#include "libraries&constants.h"


//Share<int32_t> duty_cycle_fan ("Power");
// some extern share from the controller to tell the safety supervisor to turn on the fan
// extern share<bool> fan_state ("Fan Flag");



/** @brief   Safet Supervising Task. 
 *  @details This task monitors incoming results from our temperature sensor and limit switches and 
 *           if an error condition occurs this task sets different flags to make safety stuff happen 
 *           e.g. turning off the laser, turning on the fan before cutting begins, and not letting the 
 *           cutting start until the top of the laser's enclosure is closed. 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void safety_task (void* p_params)
{
    (void)p_params;                             // Shuts up a compiler warning

    uint8_t fan_pin = PC5;
    pinMode(fan_pin, OUTPUT);
    // // Set up the variables of the simulation here
    // const TickType_t sim_period = 500;         // RTOS ticks (ms) between runs
    // float sim_speed = 0.0;                      // Initialized SIM_SPEED at 0
    // int32_t duty_cycle_var;                     // Needs to keep DUTY_CYCLE_VAR as a int32_t so it can SHARE with DUTY_CYCLE
    // float sim_A = 0.95;
    // float sim_B;
    // sim_B = 1.0 - sim_A;


    // Initialise the xLastWakeTime variable with the current time.
    // It will be used to run the task at precise intervals


    for (;;)
    {
        // FAN STATE MACHINE PSEUDO CODE
        // if laser is on (we get the flag from Control) change state to on state to turn on the fan.
        // else if laser is done cutting change state to off state after delaying for a few seconds to clear out the enclosure 

        // LIMIT 


        
        // Temp Simulated PWM stuff, probably not going to use this...
        // The simulation code goes here...you probably knew that already

        // duty_cycle_fan.get (duty_cycle_var);

        // // ????????? can we keep this motor equation or should we charaterize the fan's motor????????
        // sim_speed = sim_speed * sim_A + duty_cycle_var * sim_B;

        // // ????????? do we need to set up this pin
        // // PC5 is the pin for the fume extraction/fan pwm signal
        // analogWrite (PC5, (int32_t)sim_speed);

        // // Might need this code if we want to switch the direction fo the fan
        // // Sends a "Phase" signal to Pin D3 to set what direction it is going
        // // With a positive duty_cycle_var, the motor will be sending

        // if (duty_cycle_var >= 0)
        // {
        //     digitalWrite (D4, LOW);
        // }
        // else 
        // {
        //     digitalWrite (D4, HIGH);
        // }

        // digitalWrite (D4, HIGH);

        // This type of delay waits until it has been the given number of RTOS
        // ticks since the task previously began running. This prevents timing
        // inaccuracy due to not accounting for how long the task took to run
        vTaskDelay (safety_period);
    }

}






