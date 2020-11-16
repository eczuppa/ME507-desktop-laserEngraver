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

    #define LINE_BUFFER_SIZE 80

    static char line[LINE_BUFFER_SIZE]; // Line to be executed. Zero-terminated.

    // Values in line are set one by one when data is recieved from the serial port. 
    // Here, we need to simulate what line will look like to use for testing.  

    String test_line = "; Testing code text file";
    test_line = "G21; Set units to mm";
    test_line = "G1 X46.12 Y39.20 S1.00 F600";

    ///Set each character in static char @c line to match our test string, just like it 
    ///would in the parser (once that's finished)
    for (uint8_t char_counter = 0; char_counter<=test_line.length();  char_counter++)
    {
        line[char_counter] = test_line[char_counter];
    }

    interpret_gcode_line(line);

    #endif //NIKO_TESTING

    //======================================================================================

    
    //MATTHEW test section
    #ifdef MATTHEW_TESTING

    // Set up a singular line of GCode that will go into the control loop for motor A
    float GCode_A_pos = 4;              // this will go into a share GCode_share that will be the "desired" input for position
    float GCode_A_vel = 10;             // this will go into a share GCode_share that will be the "desired" input for velocity

    // Set up a singular line of encoder code that will go into the control loop for motor A
    float encoder_A_pos = 4.2;          // this will go into a share Encoder_share that will be the "actual" input for position
    float encoder_A_vel = 10.2;         // this will go into a share Encoder_share that will be the "actual" input for velocity

    // Set up the gain's for PID controller
    float kP = 10;
    float kI = 0.1;
    float kD = 0.01;

    // Now it's time to set up the share's so I can set up an instance of the class
    // Initial set up of shares
    Share<float> GCode_share_pos ("Desired Position");
    Share<float> GCode_share_vel ("Desired Velocity");
    Share<float> encoder_share_pos ("Actual Position");
    Share<float> encoder_share_vel ("Actual Velocity");

    // Putting values into shares... like a boss
    GCode_share_pos.put (GCode_A_pos);          // put data into share
    GCode_share_vel.put (GCode_A_vel);          // put data into share
    encoder_share_pos.put (encoder_A_pos);      // put data into share
    encoder_share_vel.put (encoder_A_vel);      // put data into share    

    // workflow: now we must go over to the control.cpp file and declare an "extern" in order 
    // to be able to use the my_share.get functionality



    // Set up an instance of class "controller_PID" to see if the shares worked! (and to see if everything else works)
    // Start by setting up a class for just motor A





    // Then set up the a task... and maybe plot something to see how the response is? 
    // This one might get a little complicated... but should be a grand time



    #endif //MATTHEW_TESTING

    //======================================================================================
    
    //Ethan test section
    #ifdef ETHAN_TESTING

    xTaskCreate(motor_task, 
                "test motor", 
                1024, 
                NULL, 
                4, 
                NULL);
    Serial << "motor task init done" << endl;
    xTaskCreate(encoder_task,
                "test encoder",
                4096,
                NULL,
                4,
                NULL);
    Serial << "encoder task init done" << endl;

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