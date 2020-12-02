/** @file       safetySupervisor.cpp safety task function. 
 *  @brief      File that contains the safety task function file implementation.
 *              Monitors the temperature sensor and has a state machine to have the 
 *              fan turn on when the laser is on and making sure the fan stays on 
 *              for three seconds after the laser turns off
 *
 *  @author  Niko Banks, Matthew Carlson, Ethan Czuppa
 *  @date    Nov 5 2020 Original file
 */

#include "libraries&constants.h"


// some extern share from the controller to tell the safety supervisor to turn on the fan
extern Share<bool> fan_state;
Share<uint8_t> warning_code ("Safety Event Flag");


/** @brief   Safety supervising task set up
 *  @details This task monitors incoming results from the temperature sensor and flags from the control task
 *           that say when the laser is turned on or turned off. If the temperature sensor detects a flame
 *           this task will set a flag that can be used to turn off the later. The fan will turn on when 
 *           the laser turns on. When the laser is off for more than 3 seconds in a row, the fan will turn off.
 *           
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void safety_task (void* p_params)
{
    (void)p_params;                             // Shuts up a compiler warning

    

    // - - - - - FAN - - - - - 


    // FAN STATE MACHINE PSEUDO CODE
    // if laser is on (we get the flag from Control) change state to on state to turn on the fan.
    // else if laser is done cutting change state to off state after delaying for a few seconds to clear out the enclosure 


    // Initialize "state" to the default state of zero
    uint8_t state = 0;

    uint8_t fan_pin = PB10;                 // PB10 = 6     This is the pin for the transistor gate
    // Set up pin for fan
    pinMode(fan_pin, OUTPUT);               // Do we need to set this up?

    // This is where the fan state from the share will be put
    bool fan_flag;

    // Set up threshold and counter to make sure the fan will only turn off 
    // once the laser has been off for a consectutive 3 seconds
    float wait_time = 30000;                                // this is 3 seconds
    float time_threshold = wait_time / float(safety_period);          // = 300000 (0.1ms) / 100 (ms/count)
    // float time_threshold = wait_time / (safety_period);          // = 300000 (0.1ms) / 100 (ms/count)

    // Serial << "time threshold = " << time_threshold << endl;         // For testing
    uint32_t counter = 0;        


    for (;;)
    {

        // get the fan state from the share
        fan_state.get(fan_flag);
        // Serial << "Fan Flag = " << fan_flag << endl;
        
        // Initialize fan in the off position
        bool fan_switch;

        // State 0
        if (state == 0)                         // The fan is off in state 0
        {
            if (fan_flag == 0)                  // Flag is down (0)
            {
                fan_switch = LOW;                   // Turn fan off
                // Serial << "Fan is off, fan switch = " << fan_switch << "            " << '\r';
            }
            
            else if (fan_flag == 1)             // Flag is up (1)
            {
                state = 1;                      // Go to state 1 (which will turn the fan on)
            }
        }

        // State 1
        else if (state == 1)                    // The fan is on in state 1
        {
            
            if (fan_flag == 0)
            {   // This makes sure fan doesn't turn off for 3 seconds after the laser turns off
                if (++counter >= time_threshold)     // Compare "counter" to "timethreshhold"
                {
                    state = 0;                  // If threshhold is met, reset back to State 0
                    // Serial << "Final count = " << counter << "            " << endl;

                    counter = 0;                // Reset counter so it can be used again  
                }
                
                else
                {
                    state = 1;                  // Stays in state 1
                    // Serial << "Counting... fan_switch = " << fan_switch << "            " << '\r';
                }
            }

            if (fan_flag == 1)
            {
                fan_switch = HIGH;              // Turn fan on (this will at least happen the first time we go to state 1, because fan_flag = 1)
                state = 1;                      // Keep in state 1
                counter = 0;                    // Reset counter so it can be used again
                // Serial << "Fan is on, fan_switch = " << fan_switch << "            " << '\r';
            }
        }
        
        // give the fan either a HIGH or LOW command per each run of the loop
        digitalWrite(fan_pin, fan_switch);      // write "fan_pin" to either HIGH or LOW (ON or OFF)


        




        // - - - - - LIMIT - - - - -




        
        // Temp Simulated PWM stuff, probably not going to use this...
        // The simulation code goes here...you probably knew that already





        // uint8_t fan_pin = PC5;                   // PC5 goes to GND or 3V3... 
        // pinMode(fan_pin, OUTPUT);
        
        // // Set up the variables of the simulation here
        // const TickType_t sim_period = 500;         // RTOS ticks (ms) between runs
        // float sim_speed = 0.0;                      // Initialized SIM_SPEED at 0
        // int32_t duty_cycle_var;                     // Needs to keep DUTY_CYCLE_VAR as a int32_t so it can SHARE with DUTY_CYCLE
        // float sim_A = 0.95;
        // float sim_B;
        // sim_B = 1.0 - sim_A;


        // Initialise the xLastWakeTime variable with the current time.
        // It will be used to run the task at precise intervals



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






