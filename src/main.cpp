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