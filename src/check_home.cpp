/** @file encoder_task.cpp is the check home task function file implementation.
 *        It will interface with the limit switches and debouncer class in order
 *        to run give the X and Y line commands to the motors until it triggers 
 *        the X and Y limit swtiches respectively. When it does that, it will set
 *        the encoder's value to zero.
 *  
 *  @author Matthew Carlson
 *  @author Ethan Czuppa
 * 
 *  @date 23 Nov 2020 Created these files and started outlining the code structure.
 *        There is a lot of pseudocode and questions, but the general outline of the
 *        state machine is there.
 * 
 */


#include "libraries&constants.h"

// Set up shares and queues

// Queues for Encoder A
// HOMING CYCLE DATA
extern Share<bool> X_home ("Check the home of X position");
extern Share<bool> Y_home ("Check the home of Y position");

extern Share<bool> check_home_state;

// Check Home Task

/** @brief   Task run at the beginning of a job to establish home and zero the encoders.
 *  @details This task will run the motors blindly in X and Y directions respectfully and 
 *           only stop them when a limit switch is triggered (and debounced). It will then
 *           talk with the encoder class and zero the encoders in X and Y.
 * 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void check_home_task (void* p_params)
{

    // Initialize debouncer code and maybe interface with motors


    // Initialize "state" to the default state of zero
    uint8_t state = 0;

    // this flag tells the task to run
    bool home_flag;



    // * * * * * * LARGE UNCERTAINTY HERE * * * * * *
    //             (See notes in state 0)
    // determine a motor speed PWM for the motors to move for calibration
    uint8_t motor_speed = 100;              // This is a PWM signal... TUNE THIS NUMBER PLEASE

    // PSEUDOCODE: SOMEHOW TELL THE MOTOR TO SPIN... THESE ARE KINDA JUST DUMMY VARIABLES
    uint8_t PWM_check_home_motor_A = 0;
    uint8_t PWM_check_home_motor_B = 0;
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * *




    // Make instances of each switch using the Debouncer class

    // Make "limit_switch_x" as the switch we want to debounce
    // Given input pin PC13 and threshhold of 5
    Debouncer limit_switch_x(PC13, 5);      // !!! NOTE!!! These pin are set up as INPUT_PULLUP, is that what we want? (I think yes, but maybe not)
    
    // Define some output variable that the "update" function can spit out to
    bool output_limit_switch_x = 0;


    // Make "limit_switch_y" as the switch we want to debounce
    // Given input pin PC14 and threshhold of 5
    Debouncer limit_switch_y(PC14, 5);      // !!! NOTE!!! These pin are set up as INPUT_PULLUP, is that what we want? (I think yes, but maybe not)
    
    // Define some output variable that the "update" function can spit out to
    bool output_limit_switch_y = 0;
 

    

    for (;;)
    {


        check_home_state.get(home_flag);
        

        // Limit switch x value as a bool
        output_limit_switch_x = limit_switch_x.update();
        
        // Limit switch x value as a bool
        output_limit_switch_y = limit_switch_y.update();




        // Put code here to run the motors in the X and Y directions
            // Send the same PWM signal to each motor, but keep in mind the directions.
            // Same direction will be X
            // Opposite directions will be Y
                // Note: refer to the CoreXY diagram for correct signs and directions
         

        // Probably write a state machine for two states, 
        // state 0: Do nothing
        // state 1: X direction motor and limit switch debouncer
        // state 2: Y direction motor and limit switch debouncer
        

        // State 0
        if (state == 0)                         // Do nothing... wait for activation
        {
            // Put code here that waits for a flag from control telling this to run.
            if (home_flag == 0)
            {
                // PSEUDOCODE: do nothing, tell the motors not to move.
                PWM_check_home_motor_A = 0;
                PWM_check_home_motor_B = 0;

                // put these in shares/queues to talk to the motors 
                // * * * * * QUESTION: How are we going to do this?!?! Ideally we have a special "check home"
                // method somewhere right? Maybe in the motor driver class?! 
                // Will we have to instanciate the motors in this code as well? Will that confuse 
                // the motor task?! Ideally not cause they won't be running at the same time right?
                // * * * * * 
            }

            else if (home_flag == 1)
            {
                // Go to state 1 to do the X calibration
                state = 1;

            }
            
        }


        // State 1
        else if (state == 1)                    // Calibrate X coordinate
        {
            // PSEUDOCODE: Tell the motors to run blindly at some speed... spins the same direction calibrates X
            PWM_check_home_motor_A = -motor_speed;
            PWM_check_home_motor_B = -motor_speed;

            if (output_limit_switch_x == 0)     // If switch has not been hit, then keep running state 1
            {
                state = 1;
            }

            else if (output_limit_switch_x == 1)    // If switch is engaged, stop motors and zero encoders, go to state 2
            {
                // PSEUDOCODE: immediately stop motors!
                PWM_check_home_motor_A = 0;
                PWM_check_home_motor_B = 0;
                // .. or maybe we can use the "brake" method in the motor driver class

                // PSEUDOCODE: Set encoders equal to zero... using the "enc_zero" method

                // I guess we'll have to create two instances of the encoders also...
                // How are we going to make sure that the enocders remember the "zero" do we have to convert back to A and B since encoders are working in A and B?


                // Serial << "X is calibrated" << endl;

                // go to state 2
                state = 2;

            }

        }

        // State 2
        else if (state == 2)                    // Calibrate Y coordinate
        {
            // PSEUDOCODE: Tell the motors to run blindly at some speed... spins different directions calibrates X
            PWM_check_home_motor_A = -motor_speed;
            PWM_check_home_motor_B = motor_speed;

            if (output_limit_switch_y == 0)     // If switch has not been hit, then keep running state 2
            {
                state = 2;
            }

            else if (output_limit_switch_y == 1)    // If switch is engaged, stop motors and zero encoders, go back to state 0
            {
                // PSEUDOCODE: immediately stop motors!
                PWM_check_home_motor_A = 0;
                PWM_check_home_motor_B = 0;
                // .. or maybe we can use the "brake" method in the motor driver class

                // PSEUDOCODE: Set encoders equal to zero... using the "enc_zero" method

                // I guess we'll have to create two instances of the encoders also...
                // How are we going to make sure that the enocders remember the "zero" do we have to convert back to A and B since encoders are working in A and B?


                // Serial << "X is calibrated" << endl;

                // go to state 2
                state = 0;
                
            }

        }






        vTaskDelay(check_home_latency);
    }

}



