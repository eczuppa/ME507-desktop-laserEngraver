/** @file       debouncer.cpp
 *  @brief      File containing a debouncer class and its methods.
 *  @details    This file contains a debouncer class which can be used to read the pressing of a button
 *              correctly without any "bouncing" effects.
 * 
 *  @author Matthew Carlson
 *  @date  Oct 24 2020 Original file
 */


#include <Arduino.h>
#include <PrintStream.h>
#include "debouncer.h"


/** @brief   Creates a debouncer to filter through noise from buttons and switches
 *  @details This contructor looks at a data stream, identifies when the a signal has
 *           been activated, and switches state. It sets up an State Machine using a set 
 *           of IF statements to determine what the values are of an input pin, counts 
 *           how many consecutive 0's or 1's are detected, and what corresponding state
 *           it should be in. State 0 outputs a FALSE and State 2 outputs a TRUE.
 *  @param   pin This is the parameter of which pin our button or switch is connected to
 *  @param   threshold This is set as whatever we want our threshold to be for how many consecutive
 *                     1's or 0's are acceptable for saying the button or switch is active or not.
 */
Debouncer::Debouncer (uint8_t pin, uint8_t threshold_constructor)
{
    // define a variable input_pin that reads either a HIGH (1) or LOW (0) from
    // the blue push botton (PC13) on the Nucleo L476
    input_pin = pin;

    // Set some number of 0's or 1's in a row to be the threshhold for if the switch is activated
    threshold = threshold_constructor;


    // Set up button as in input using PinMode
    pinMode(input_pin, INPUT_PULLUP);

}


bool Debouncer::update (void)
{
    bool signal = digitalRead(input_pin);       // Read HIGH or LOW from input_pin



        if (state == 0)                         // State 0: Waiting and reading input pin: PC13
        {
           button_result = 0;                   // Set "result" equal to FALSE in State 0

           Serial << "Waiting...                  " << "\r";


            if (signal == 0)                     // Read pin: PC13 (Active state is Logic 0)
            {
                state = 1;                          // IF zero is detected, go to state 1
            Serial << "Going to Counting...    " << "\r";
            }
        }

        
        else if (state == 1)                    // State 1: Counting number of zeros, if enough go 
                                                // to State 2, if not go back to State 1, if there's
                                                // a 1, go back to State 0
        {
            Serial << "Counting...              " << "\r";

            if (++counter >= threshold)         // Compare "counter" to "threshhold"
            {
                state = 2;                      // If threshhold is met, go to state 2
                counter = 0;                    // Reset counter so it can be used again
                Serial << "Going to Triggered...  " << "\r";
            }

            else if (signal == 1)
            {
                state = 0;                      // IF one is detected, go back to state 0
                counter = 0;                    // Reset counter
                Serial << "Going to Waiting...   " << "\r";

            }

            else
            {
                state = 1;                      // ELSE keep state = 1 so it can run through
                                                // and add to the "++" the counter again
                
            }
        }


        else if (state == 2)                    // State 2: Triggered
        {
            // Returns a TRUE

            button_result = 1;                  // Set "result" equal to TRUE in State 2

            Serial << "Triggered!!               " << "\r";




            if (signal == 1)                    // Do another counter so we can reset to State 0 
                                                // if there are a bunch of Logic 1's in a row
            {

                if (++counter >= threshold)     // Compare "counter" to "threshhold"
                {
                    state = 0;                  // If threshhold is met, reset back to State 0
                    counter = 0;                // Reset counter so it can be used again
                    Serial << "Going to Waiting...     " << "\r";

                }
                
                else
                {
                    state = 2;                  // Stays in State 2
                }
            }

            else if (signal == 0)
            {
                counter = 0;                    // Reset counter when there is a zero
                state = 2;                      // Stays in State 2


            }


        }

        else
        {
            Serial << "There is a large problem happening, must reset to State 0" << "\r";
            state = 0;

        }

    return button_result;
}




