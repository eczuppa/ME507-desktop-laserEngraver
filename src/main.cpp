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
Queue<char[LINE_BUFFER_SIZE]> chars_to_print(WRITE_Q_SIZE,"char array printer");
Queue<char[LINE_BUFFER_SIZE]> read_chars(READ_Q_SIZE,"read_val");

/**
 * In order to achieve sub-millisecond task timing, the default value of configTick_RATE_HZ was changed to (SysClock)/10000 instead of (SysClock)/1000. 
 * This means every RTOS tick is 1/10th of a millisecond instead of 1 millisecond. This comes at the cost of reducing the time slice each task can be 
 * given before the CPU is overloaded by how fast the RTOS is trying to run. 
 * Use this version with a lower powered microcontroller at your own risk. This fork was created solely for this config change for a grbl-like 
 * (but not compatible ... yet) desktop laser cutter than uses a STM32 Nucleo L476RG. Also, delay quantities in vTaskDelay or vTaskDelayUntil can be updated
 *  to account for the change in TICK_RATE_HZ by using the pdMS_TO_TICKS() macro as follows:
 *         const TickType_t xBlockTime = pdMS_TO_TICKS( 100 );
 *         vTaskDelay( xBlockTime ); 
 * from https://arduino.stackexchange.com/questions/25746/changing-the-tick-time-in-freertos 
 * answered by FreeRTOS.org and rebatoma. This approach is significantly more convenient than 
 * going through and updating all the delay periods for each of your tasks.
 */ 




void setup() 
{
    // Start the serial port, wait a short time, then say hello. Use the
    // non-RTOS delay() function because the RTOS hasn't been started yet
    Serial.begin (115200);
    delay (2000);
    Serial << endl << "Laser Program Initializing" << endl;
    



    //======================================================================================

    //Choose your testing section:
    // #define NIKO_TESTING 0
    // #define MATTHEW_TESTING 1
    // #define ETHAN_TESTING 2

    //======================================================================================
    
    //Niko test section
    #ifdef NIKO_TESTING

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

    // Create a task to translate command codes to the contorller
    xTaskCreate(task_translate,                 //Task Function name
                 "Translating",                 // Name for printouts
                 1000,                          // Stack size
                 NULL,                          // Parameters for task fn.
                 9,                             // Priority
                 NULL);                         // Task handle


    // static char line[LINE_BUFFER_SIZE]; // Line to be executed. Zero-terminated.

    // // Values in line are set one by one when data is recieved from the serial port. 
    // // Here, we need to simulate what line will look like to use for testing.  

    // String test_line = "; Testing code text file";
    // test_line = "G21; Set units to mm";
    // test_line = "G1 X46.12 Y39.20 S1.00 F600";

    // ///Set each character in static char @c line to match our test string, just like it 
    // ///would in the parser (once that's finished)
    // for (uint8_t char_counter = 0; char_counter<=test_line.length();  char_counter++)
    // {
    //     line[char_counter] = test_line[char_counter];
    // }

    // interpret_gcode_line(line);

    vTaskStartScheduler();

    #endif //NIKO_TESTING

    //======================================================================================

    
    //MATTHEW test section
    #ifdef MATTHEW_TESTING


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

    // xTaskCreate(motor_task, 
    //             "test motor", 
    //             1024, 
    //             NULL, 
    //             4, 
    //             NULL);
    // Serial << "motor task init done" << endl;
    xTaskCreate(encoder_task,
                "test encoder",
                4096,
                NULL,
                4,
                NULL);
    // Serial << "encoder task init done" << endl;

    xTaskCreate(task_ui,
                "user", 
                4096, 
                NULL,
                7,
                NULL);
    Serial << "ui task init done" << endl;


    vTaskStartScheduler ();




    #endif //ETHAN_TESTING


}

void loop()
{
}