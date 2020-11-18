// /** @file    share_testing.cpp Creates a place for the shares and queues to be tested
//  *  @details This file will be deleted once the GCode and Encoder codes are up and running
//  * @author   Matthew
//  * 
//  * @date     15 Nov 2020 Created file for testing
//  * 
//  */

// #include "share_testing.h"
// #include "libraries&constants.h"

// //declare the shares again as an "extern" so this file can read them
// extern Share<float> GCode_share_pos;
// extern Share<float> GCode_share_vel;
// extern Share<float> encoder_share_pos;
// extern Share<float> encoder_share_vel;


// void share_testing_send (void* p_params)
// {
//     (void)p_params;            // Does nothing but shut up a compiler warning

//     Serial << "PRINT ME share_testing.cpp file" << endl;

//     // Set up a singular line of GCode that will go into the control loop for motor A
//     float GCode_A_pos = 4;              // this will go into a share GCode_share that will be the "desired" input for position
//     float GCode_A_vel = 10;             // this will go into a share GCode_share that will be the "desired" input for velocity

//     // Set up a singular line of encoder code that will go into the control loop for motor A
//     float encoder_A_pos = 40.2;          // this will go into a share Encoder_share that will be the "actual" input for position
//     float encoder_A_vel = 100.2;         // this will go into a share Encoder_share that will be the "actual" input for velocity


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

//         // Timing accuracy isn't extremely important, so use the simpler delay
//         vTaskDelay (100);
//     }

//     // Serial << "PRINT ME share_testing.cpp file" << endl;

    
    
// }




