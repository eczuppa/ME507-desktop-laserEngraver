/** @file       control_task.cpp
 *  @brief      File containing the main control task.
 *  @details    This file contains the definition of the control_task function, which is the main function
 *              that runs the motors and laser in the DigitalCarpenter-Lite system. 
 *
 *  @author Niko Banks
 *  @author Ethan Czuppa
 *  @author Matthew Carlson
 *  @date Nov 20 2020
*/


#include "libraries&constants.h"
///@cond
//setup externs for Incoming shares and queues here

// TRANSLATED GCODE QUEUE
extern Queue<ramp_segment_coefficients> ramp_segment_coefficient_queue;

// CHECK HOME FLAG
extern Share<bool> check_home;

//Uncomment as used to ensure we don't make anything we don't use:
// // MOTOR A ENCODER DATA
// extern Share<float> encoder_A_pos; 
// extern Share<float> encoder_A_velocity;
// extern Share<uint32_t> encoder_A_dt;

// // MOTOR B ENCODER DATA
// extern Share<float> encoder_B_pos;
// extern Share<float> encoder_B_velocity;
// extern Share<uint32_t> encoder_B_dt;

// // HOMING CYCLE DATA
// extern Share<bool> check_home;
// extern Share<bool> X_home;
// extern Share<bool> Y_home;

// // WARNINGS FROM SAFETY SUPERVISOR
// extern Share<uint8_t> warning_code; 


//Setup Outgoing Shares and Queues Here

// MOTOR A&B PWM COMMANDS
Queue<float> queue_PWM_motor_A (8,"Motor A DutyCycle");
Queue<float> queue_PWM_motor_B (8,"Motor B DutyCycle");

///@endcond


/** @brief      Task which recieves incoming commands, runs them through a control loop, and then sends them back out.
 *  @details    This task receives input commands with desired values for the two motors, sent from @c task_translate, and
 *              input positions from each of the motor encoders from @c encoder_A_task and @c encoder_B_task. First, the
 *              desired positions and velocities are turned into ramp inputs for each motor by creating more points in between
 *              two consecutive points through linear interpolation. This allows the laser head to be have tighter control over
 *              velocity of the laser head. Each of the inputs of desired and measured positions and velocities are then sent 
 *              through a PID control loop inside this task, which then outputs PWM signals that are sent via queues to
 *              @c motor_A_driver_task and @c motor_B_driver_task to control the motors. 
 * 
 *              The task has two main states, NORMAL_OPERATION and SAFETY_STOP. Under NORMAL_OPERATION, any incomming desired
 *              positions will be converted to ramps, controlled, and sent to the motors. SAFETY_STOP, if enabled, will immediately
 *              shut down the laser and keep it that way until the Safety_Supervisor task gives the go-ahead.
 * 
 *  @param      p_params A pointer to function parameters which we don't use.
 */
void control_task(void* p_params)
{
    TickType_t xLastWakeTime = xTaskGetTickCount(); //Start timing the task

    uint8_t control_state = NORMAL_OPERATION;       //Variable for controller state
    bool home_flag = false;                         //Homing flag

    //Struct containing desired values from task_translate
    ramp_segment_coefficients desired;

    //Class containing motion-planning data
    MotionPlanning Ramper_A(0,0,0,0);
    MotionPlanning Ramper_B(0,0,0,0);

    //Crete PID loop classes
    Controller_PID ControlLoopPID_A(0,0,0,0,0,0,0,0);
    Controller_PID ControlLoopPID_B(0,0,0,0,0,0,0,0);

    //Infinite task loop
    for(;;)
    {
        //State checker: Change the state if necessary
        check_home.get(home_flag);
        if (home_flag)
        {
            control_state = HOMING;
        }


        switch(control_state)
        {
            //When running in this state, control_task looks for any incomming information into desired_queue. When it 
            //gets somthing there, it checks 
            case NORMAL_OPERATION:

                //If we have new desired values, 
                // * and we have space to put values into the motor A and B queues (later):
                if (ramp_segment_coefficient_queue.any())
                {
                    //Get the desired values:
                    ramp_segment_coefficient_queue.get(desired);

                    //Get the current encoder positions and velocities:

                    //Put desired values through control loop:

                    //Then send outputs to the queues for the motors.
                    
                }

                break;
            
            //This state will be reached when the Safety task detects something wrong with the system. At this point, 
            //the laser will be shut down immediately.
            case SAFETY_STOP:
                //Set Motor PWM signals to 0
                //Set laser PWM to 0 
                break;
            
            case HOMING:
                //Bypass controller and set Motor PWM signals to aim at home; run homing procedure
                
            default:
                break;
                //Shouldn't get here right?
        };


// Old state machine - see report for actual one planned. 

      // STATE 0 HUB - checks for conditions to transition to appropriate state, otherwise waits in the hub
      //        if A_ and B_desired (queues) have been revcieved from the decoding task, and the machine has been homed, 
      //                    and check that the encoder shares are available then, transition to state 1
      //        if a warning code that is non-zero has been received, transition to the warning code handling - state 2
      //        if Homing flag is set by ?? something ... go to homing handler, the state that works with the check_home task
      //        if state 1 has been completed (maybe a better condition than this) - go to the serial writing handling, state 4

      
      // STATE 1 MOTOR CONTROL LOOP - of the state machine in control makes and queues up motor commands
      //      Run this state if the A and B_desired have been received       
      // state 1_0: init memory for motion planning classes,
      // state 1_1: run generate_ramp method for both motion planning classes, don't transition until both are done 
      // state 1_1 cont'd: get output ramp values into control loops and run control loops inside an unpacking for loop in the forever loop, 
      // state 1_1 cont'd: put output motor commands in corresponding queues
      // state 1_2: deallocate memory for ramp input arrays to avoid memory leaks
      
      // STATE 2 SAFETY_STOP - enacts software safegaurds to try and protect the system or keep it from running when it shouldn't
      
      // STATE 3 HOMING - home the machine
            
    //Below is experimental- not used.
      // STATE 4 RESET/E-STOP/CRASH state - something that handles those occuring in a commensesne manner -e.g. turn off the laser, tell motors to go home slowly (?)
      //                                    and then when it gets home and is okayed by the user (?) future work

        
        vTaskDelayUntil(&xLastWakeTime, control_task_period);
    }
}