/** @file main.cpp
 *      This is the main file for running laser cutters using a RTOS. This file is currently
 *      in testing mode: Each person has there own section marked with #ifdef lines to write
 *      their own testing code without interfereing with others. 
 *
 *  @author  Niko Banks 
 *  @author  Matthew Carlson
 *  @author  Ethan Czuppa
 * 
 *  @date    Nov 11 2020    File created
 */


#include "libraries&constants.h"

//Shares and queues should go here

// Queues for serial printing and reading
Queue<char[LINE_BUFFER_SIZE]> chars_to_print_queue(WRITE_Q_SIZE,"char array printer");
Queue<char[LINE_BUFFER_SIZE]> read_chars_queue(READ_Q_SIZE,"read_val");

// Shares for Encoder A and B
Share<encoder_output> enc_A_output_share ("Encoder A variables");
Share<encoder_output> enc_B_output_share ("Encoder B variables");

// Queue for ramp segments
Queue<ramp_segment_coefficients> ramp_segment_coefficient_queue(RAMP_COEFFICIENT_Q_SIZE,"Ramp Coefficients");

// Share for signalling to check home
Share<bool> check_home_share ("Homing Flag");

// Queue for Temperature Task 
// Queue<float> temperature_data (10,"Temp C Data");  




/**
 * In order to achieve sub-millisecond task timing, the default value of configTick_RATE_HZ was changed to 
 * (SysClock)/10000 instead of (SysClock)/1000. This means every RTOS tick is 1/10th of a millisecond instead of 
 * 1 millisecond. This comes at the cost of reducing the time slice each task can be given before the CPU is overloaded by 
 * how fast the RTOS is trying to run. Use this version with a lower powered microcontroller at your own risk. This fork was 
 * created solely for this config change for a grbl-like (but not compatible ... yet) desktop laser cutter than uses a STM32 
 * Nucleo L476RG. Also, delay quantities in vTaskDelay or vTaskDelayUntil can be updated to account for the change in 
 * TICK_RATE_HZ by using the pdMS_TO_TICKS() macro as follows:
 *         const TickType_t xBlockTime = pdMS_TO_TICKS( 100 );
 *         vTaskDelay( xBlockTime ); 
 * from https://arduino.stackexchange.com/questions/25746/changing-the-tick-time-in-freertos 
 * answered by FreeRTOS.org and rebatoma. This approach is significantly more convenient than 
 * going through and updating all the delay periods for each of your tasks.
 * 
 * UPDATE 4/27/20: sub-millisecond task timing doesn't work, it overloads the controller when multiple tasks are run at 
 * that speed. and also, we discovered that it isn't necessary; the encoder timers count all the ticks regardless of how 
 * fast the encoder task function is run. As a result, configTick_RATE_HZ was set back to (SysClock)/1000 in 
 * FreeRTOSConfig_Default.h to set timing back in milliseconds. 
 * 
 */ 



void setup() 
{
    // Start the serial port, wait a short time, then say hello. Use the
    // non-RTOS delay() function because the RTOS hasn't been started yet
    Serial.begin (115200);
    delay (2000);
    Serial << endl << "\nLaser Program Initializing" << endl;
    
    //Create instance of timer 3 for motors
    // TIM_TypeDef * _p_timer = TIM3;
    // static HardwareTimer *MotorTmr;    
    // MotorTmr = new HardwareTimer(_p_timer);

    //======================================================================================

    //Choose your testing section:
    // #define NIKO_TESTING 0
    // #define MATTHEW_TESTING 1
    // #define ETHAN_TESTING 2
    #define TEST_CONST_VELOCITY 3
    // #define TEST_CONTROL_PATH 4

    //======================================================================================
    
    //Niko test section
    #ifdef NIKO_TESTING

    Serial << "Running Niko Testing" << endl;

    pinMode(LED_BUILTIN,OUTPUT);

    // Create a task to read inputs from the serial port
    xTaskCreate (task_read_serial,              //Task Function name
                 "Reading Serial",              // Name for printouts
                 1000,                          // Stack size
                 NULL,                          // Parameters for task fn.
                 8,                             // Priority
                 NULL);                         // Task handle


    // Create a task to read inputs from the serial port
    xTaskCreate (task_print_serial,             //Task Function name
                 "Printing Serial",             // Name for printouts
                 1000,                          // Stack size
                 NULL,                          // Parameters for task fn.
                 3,                             // Priority
                 NULL);                         // Task handle

    // // Create a task to translate command codes to the contorller
    // xTaskCreate(task_translate,                 // Task Function name
    //              "Translating",                 // Name for printouts
    //              1000,                          // Stack size
    //              NULL,                          // Parameters for task fn.
    //              9,                             // Priority
    //              NULL);                         // Task handle


    // Create a task that test runs a single motor
    xTaskCreate (task_test_control_path,        // Task Function name
                 "Motor Test",                  // Name for printouts
                 1000,                          // Stack size
                 NULL,                          // Parameters for task fn.
                 1,                             // Priority
                 NULL);                         // Task handle

    //Task to run encoder A
    xTaskCreate(task_encoder_A,                 // Task Function name
                "Run encoder A",                // Name for printouts
                4096,                           // Stack size
                NULL,                           // Parameters for task fn.
                13,                             // Priority
                NULL);                          // Task handle

    //Task to run encoder B
    xTaskCreate(task_encoder_B,                 // Task Function name
                "Run encoder B",                // Name for printouts
                4096,                           // Stack size
                NULL,                           // Parameters for task fn.
                13,                             // Priority
                NULL);                          // Task handle

    // // Create a task to run a test script
    // xTaskCreate (task_test_script,              //Task Function name
    //              "Run Test",                    // Name for printouts
    //              1000,                          // Stack size
    //              NULL,                          // Parameters for task fn.
    //              10,                            // Priority
    //              NULL);                         // Task handle
    
    Serial << "Tasks created" << endl;

    vTaskStartScheduler();

    #endif //NIKO_TESTING

    //======================================================================================

    
    //MATTHEW test section
    #ifdef MATTHEW_TESTING

    Serial << "Running Matthew Testing" << endl;


// FAN TESTING CODE: Using the blue button on the Nucleo to simulate the "laser is on" flag.


//    // ˇˇˇˇˇ Paste this code above Setup()  ˇˇˇˇˇ
    // // Set up share for the flag (for testing this will come from the blue button)
    // Share<bool> fan_state ("Fan Flag");

    // void blue_button_task (void* p_params)
    // {
    //     (void)p_params;                             // Shuts up a compiler warning
        
    //     // Testing fan code with blue button on micro controller
    //     // Set up Blue Button

    //     // Set up blue button as in input using PinMode
    //     pinMode(PC13, INPUT);

    //     for (;;)
    //     {
    //         // Read HIGH or LOW from input_pin
    //         bool blue_button_output =  !digitalRead(PC13);          // Note: the "!" operator is a NOT logical operator      

    //         // Put this into the share
    //         fan_state.put(blue_button_output);

    //         // Serial << "Blue Button Output = " << blue_button_output << endl;

    //         vTaskDelay (10);
    //     }
    // }

//     // ^^^^^ Paste this code above Setup() ^^^^^


        xTaskCreate (blue_button_task,
                "get the blue button working",
                4096,
                NULL,
                4,
                NULL);

        xTaskCreate (safety_task,
                "test fan code",
                4096,
                NULL,
                3,
                NULL);
    // Serial << "Fan task has run successfully" << endl;


    // Start running the tasks, sir!
    vTaskStartScheduler ();






// CONTORL LOOP TESTING

//     // ˇˇˇˇˇ Paste this code above Setup()  ˇˇˇˇˇ

// // Now it's time to set up the share's so I can set up an instance of the class
// // Initial set up of shares
// Share<float> GCode_share_pos ("Desired Position");
// Share<float> GCode_share_vel ("Desired Velocity");
// Share<float> encoder_share_pos ("Actual Position");
// Share<float> encoder_share_vel ("Actual Velocity");
// Share<float> encoder_share_time ("Current Delta Time");


// void share_testing_send (void* p_params)
// {
//     (void)p_params;            // Does nothing but shut up a compiler warning

//     // Serial << "PRINT ME share_testing.cpp file" << endl;

//     // Set up a singular line of GCode that will go into the control loop for motor A
//     float GCode_A_pos = 4;              // this will go into a share GCode_share that will be the "desired" input for position
//     float GCode_A_vel = 10;             // this will go into a share GCode_share that will be the "desired" input for velocity

//     // Set up a singular line of encoder code that will go into the control loop for motor A
//     float encoder_A_pos = 4.2;          // this will go into a share Encoder_share that will be the "actual" input for position
//     float encoder_A_vel = 10.2;         // this will go into a share Encoder_share that will be the "actual" input for velocity

//     float encoder_A_time = 100;         // this will go into the share Encoder_share that will be the "current" delta time

//     for (;;)
//     {


//         // // Now it's time to set up the share's so I can set up an instance of the class
//         // // Initial set up of shares
//         // Share<float> GCode_share_pos ("Desired Position");
//         // Share<float> GCode_share_vel ("Desired Velocity");
//         // Share<float> encoder_share_pos ("Actual Position");
//         // Share<float> encoder_share_vel ("Actual Velocity");

//         // Putting values into shares... like a boss
//         GCode_share_pos.put (GCode_A_pos);          // put data into share
//         GCode_share_vel.put (GCode_A_vel);          // put data into share
//         encoder_share_pos.put (encoder_A_pos);      // put data into share
//         encoder_share_vel.put (encoder_A_vel);      // put data into share   
//         encoder_share_time.put (encoder_A_time);      // put data into share

//         // Timing accuracy isn't extremely important, so use the simpler delay
//         vTaskDelay (100);
//     }

    
    
// }



// void share_testing_receive (void* p_params)
// {
//     (void)p_params;                             // Shuts up a compiler warning
    

//     // make some constants that will receive the values that are in the shares
//     float position_desired;
//     float position_actual;
//     float velocity_desired;
//     float velocity_actual;
//     float delta_time;

//     // Set up the gain's for PID controller
//     float kP = 1;
//     float kI = 1;
//     float kD = 1;


//     // Serial << "PRINT ME, main.cpp file" << endl;

    
//     for (;;)
//     {



//         // ".get" the values from the shares
//         GCode_share_pos.get (position_desired);          // get data out of the share
//         encoder_share_pos.get (position_actual);         // get data out of the share
//         GCode_share_vel.get (velocity_desired);          // get data out of the share
//         encoder_share_vel.get (velocity_actual);         // get data out of the share   
//         encoder_share_time.get (delta_time);             // get data out of the share

//         // Create instance called motor_A with some fun inputs
//         // Controller_PID motor_A (kP, kI, kD, position_desired, position_actual, velocity_desired, velocity_actual);
//         Controller_PID motor_A_control (kP, kI, kD, position_desired, position_actual, velocity_desired, velocity_actual, delta_time);

//         // // Create instance called motor_A with some number inputs
//         // Controller_PID motor_A (1, 0.1, 0.01, 40, 40.2, 100, 100.2);

//         // Gotta actually run/call the control loop method
//         motor_A_control.control_loop_PID();


//         // use set functions to continually update from the share's
//         motor_A_control.set_pos_desired(position_desired);
//         motor_A_control.set_pos_actual(position_actual);
//         motor_A_control.set_vel_desired(velocity_desired);
//         motor_A_control.set_vel_actual(velocity_actual);
       


//         float testing_output = motor_A_control.get_output();
//         float testing_position_desired = motor_A_control.get_pos_desired();       // Position desired from control loop
//         float testing_pos_error = motor_A_control.get_pos_error();                    // Get position error

//         // Get a bunch of values to test things
//         Serial << "Printing from main.cpp file" << endl;

//         Serial << "Desired Position (Shared):  " << position_desired << endl;               // Position desired from share
//         Serial << "Desired Position (Control Loop):  " << testing_position_desired << endl;       // Position desired from control loop
        
//         // Serial << "Current Position:  " << position_actual << endl;
//         // // Serial << "Desired Velocity:  " << velocity_desired << endl;
//         // // Serial << "Current Velocity:  " << velocity_actual << endl;
        
//         Serial << "Output:  " << testing_output << endl;
//         Serial << "Error, Position:  " << testing_pos_error << endl;

//         // Serial << "______________________________" << endl;

//         // Serial << "PRINT ME" << endl;
//         // Then set up the a task... and maybe plot something to see how the response is? 
//         // This one might get a little complicated... but should be a grand time
        
//         // Timing accuracy isn't extremely FOR TESTING RIGHT NOW important, so use the simpler delay
//         vTaskDelay (1000);
//     }
    
// }



//     // ^^^^^ Paste this code above Setup() ^^^^^



//     // Create and run tasks for sending and receiving

//     xTaskCreate (share_testing_send,
//                 "test sharing",
//                 4096,
//                 NULL,
//                 4,
//                 NULL);
//     Serial << "Sharing task has run successfully" << endl;


//     xTaskCreate (share_testing_receive,
//                 "test receiving",
//                 4096,
//                 NULL,
//                 3,
//                 NULL);
//     Serial << "Receiving task has run successfully" << endl;

//     // Start running the tasks, sir!
//     vTaskStartScheduler ();



    #endif //MATTHEW_TESTING



    //======================================================================================
    
    //Ethan test section
    #ifdef ETHAN_TESTING

    Serial << "Running Ethan Testing" << endl;

    //Task to drive motor A
    xTaskCreate(motor_A_driver_task,            //Task Function name
                "test motor A",                 // Name for printouts
                1024,                           // Stack size
                NULL,                           // Parameters for task fn.
                12,                             // Priority
                NULL);                          // Task handle

    //Task to drive motor B 
    xTaskCreate(motor_B_driver_task,            //Task Function name
                "test motor B",                 // Name for printouts
                1024,                           // Stack size
                NULL,                           // Parameters for task fn.
                12,                             // Priority
                NULL);                          // Task handle

    Serial << "motor task init done" << endl;

    //Task to run encoder A
    xTaskCreate(task_encoder_A,                 //Task Function name
                "test encoder A",               // Name for printouts
                4096,                           // Stack size
                NULL,                           // Parameters for task fn.
                13,                             // Priority
                NULL);                          // Task handle

    //Task to run encoder B
    xTaskCreate(task_encoder_B,                 //Task Function name
                "test encoder B",               // Name for printouts
                4096,                           // Stack size
                NULL,                           // Parameters for task fn.
                13,                             // Priority
                NULL);                          // Task handle

    Serial << "encoder task init done" << endl;

    xTaskCreate(task_ui,
                "user", 
                4096,
                NULL,
                9,
                NULL);

    Serial << "ui task init done" << endl;

    vTaskStartScheduler ();




    #endif //ETHAN_TESTING



    //======================================================================================

    #ifdef TEST_CONST_VELOCITY

    Serial << "Running Constant Velocity Test" << endl;

    // Create a task to read inputs from the serial port
    xTaskCreate (task_print_serial,             //Task Function name
                 "Printing Serial",             // Name for printouts
                 1000,                          // Stack size
                 NULL,                          // Parameters for task fn.
                 3,                             // Priority
                 NULL);                         // Task handle

    // Create a task that test runs a single motor
    xTaskCreate (task_test_const_velocity,      // Task Function name
                 "Motor Test",                  // Name for printouts
                 1000,                          // Stack size
                 NULL,                          // Parameters for task fn.
                 1,                             // Priority
                 NULL);                         // Task handle

    //Task to run encoder A
    xTaskCreate(task_encoder_A,                 // Task Function name
                "Run encoder A",                // Name for printouts
                4096,                           // Stack size
                NULL,                           // Parameters for task fn.
                13,                             // Priority
                NULL);                          // Task handle

    //Task to run encoder B
    xTaskCreate(task_encoder_B,                 // Task Function name
                "Run encoder B",                // Name for printouts
                4096,                           // Stack size
                NULL,                           // Parameters for task fn.
                13,                             // Priority
                NULL);                          // Task handle

    Serial << "Tasks created" << endl;

    vTaskStartScheduler();

    #endif //TEST_CONST_VELOCITY



    //======================================================================================


    #ifdef TEST_CONTROL_PATH

    Serial << "Running Control Path Test" << endl;

    // Create a task to read inputs from the serial port
    xTaskCreate (task_print_serial,             //Task Function name
                 "Printing Serial",             // Name for printouts
                 1000,                          // Stack size
                 NULL,                          // Parameters for task fn.
                 3,                             // Priority
                 NULL);                         // Task handle

    // Create a task that test runs a single motor
    xTaskCreate (task_test_control_path,        // Task Function name
                 "Motor Test",                  // Name for printouts
                 1000,                          // Stack size
                 NULL,                          // Parameters for task fn.
                 1,                             // Priority
                 NULL);                         // Task handle

    //Task to run encoder A
    xTaskCreate(task_encoder_A,                 // Task Function name
                "Run encoder A",                // Name for printouts
                4096,                           // Stack size
                NULL,                           // Parameters for task fn.
                13,                             // Priority
                NULL);                          // Task handle

    //Task to run encoder B
    xTaskCreate(task_encoder_B,                 // Task Function name
                "Run encoder B",                // Name for printouts
                4096,                           // Stack size
                NULL,                           // Parameters for task fn.
                13,                             // Priority
                NULL);                          // Task handle
    
    Serial << "Tasks created" << endl;

    vTaskStartScheduler();

    #endif //TEST_CONTROL_PATH
}


/** @brief   Arduino's low-priority loop function, which we don't use.
 *  @details A non-RTOS Arduino program runs all of its continuously running
 *           code in this function after @c setup() has finished. When using
 *           FreeRTOS, @c loop() implements a low priority task on most
 *           microcontrollers, and crashes on some others, so we'll not use it.
 */
void loop()
{
}