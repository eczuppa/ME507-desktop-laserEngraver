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
Queue<char[LINE_BUFFER_SIZE]> chars_to_print(32,"char array printer");
Queue<char[LINE_BUFFER_SIZE]> read_chars(32,"read_val");


void setup() 
{
    // Start the serial port, wait a short time, then say hello. Use the
    // non-RTOS delay() function because the RTOS hasn't been started yet
    Serial.begin (115200);
    delay (2000);
    Serial << endl << "Laser Program Initializing" << endl;
    

    

    //======================================================================================

    //Choose your testing section:
    #define NIKO_TESTING 0
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

    // General notes: Now 17, 2020
    // Shares seem to be working pretty nicely!
    // The values are not entering into the control loop code... namely the Class specific 
    // parameters are not getting redefined by the inputs to the Controller_PID constructor


    // ˇˇˇˇˇ Paste this code above Setup()  ˇˇˇˇˇ

    #include "share_testing.h"

    // Now it's time to set up the share's so I can set up an instance of the class
    // Initial set up of shares
    Share<float> GCode_share_pos ("Desired Position");
    Share<float> GCode_share_vel ("Desired Velocity");
    Share<float> encoder_share_pos ("Actual Position");
    Share<float> encoder_share_vel ("Actual Velocity");



    void share_testing_receive (void* p_params)
    {
        (void)p_params;                             // Shuts up a compiler warning
        

        // make some constants that will receive the values that are in the shares
        float position_desired;
        float position_actual;
        float velocity_desired;
        float velocity_actual;

        // make some constants that will receive the outputs from the control loop constructor
        float testing_output = 0;
        float testing_position_desired = 0;
        float testing_pos_error = 0;

        // Set up the gain's for PID controller
        float kP = 1;
        float kI = 1;
        float kD = 1;


        Serial << "PRINT ME, main.cpp file" << endl;

        
        for (;;)
        {



            // ".get" the values from the shares
            GCode_share_pos.get (position_desired);          // get data out of the share
            encoder_share_pos.get (position_actual);         // get data out of the share
            GCode_share_vel.get (velocity_desired);          // get data out of the share
            encoder_share_vel.get (velocity_actual);         // get data out of the share   

            // Set up an instance of class "controller_PID" to see if the shares worked! (and to see if everything else works)
            // Start by setting up a class for just motor A



            // Create instance called motor_A with some fun inputs
            Controller_PID motor_A (kP, kI, kD, position_desired, position_actual, velocity_desired, velocity_actual);

            // // Create instance called motor_A with some number inputs
            // Controller_PID motor_A (10, 0.1, 0.01, 4, 4.2, 10, 10.2);


            testing_output = motor_A.get_output();
            testing_position_desired = motor_A.get_pos_desired_PID();       // Position desired from control loop
            testing_pos_error = motor_A.get_pos_error();                    // Get position error

            // Get a bunch of values to test things

            Serial << "Desired Position (Shared)" << position_desired << endl;               // Position desired from share
            Serial << "Desired Position (Control Loop)" << testing_position_desired << endl;       // Position desired from control loop
            Serial << "Current Position" << position_actual << endl;
            Serial << "Desired Velocity" << velocity_desired << endl;
            Serial << "Current Velocity" << velocity_actual << endl;
            
            Serial << "Output" << testing_output << endl;
            Serial << "Error, Position: " << testing_pos_error << endl;

            // Serial << "PRINT ME" << endl;
            // Then set up the a task... and maybe plot something to see how the response is? 
            // This one might get a little complicated... but should be a grand time
            
            vTaskDelay (1000);
        }
        Serial << "PRINT ME, main.cpp file" << endl;
        // Timing accuracy isn't extremely important, so use the simpler delay
        
    }

    // ^^^^^ Paste this code above Setup()  ^^^^^^




    // Set up share's in "share_testing.h and share_testing.cpp"
    // #include "share_testing.h"


    // workflow: now we must go over to the control.cpp file and declare an "extern" in order 
    // to be able to use the my_share.get functionality
    // ... Now I don't know if I need to do this actually...
    // Ask about this


    // //declare the shares again as an "extern" so this file can read them
    // extern Share<float> GCode_share_pos;
    // extern Share<float> GCode_share_vel;
    // extern Share<float> encoder_share_pos;
    // extern Share<float> encoder_share_vel;




    // We still gotta ".get" those fine values in "share_testing" now don't we?

    xTaskCreate (share_testing_send,
                "test sharing",
                4096,
                NULL,
                4,
                NULL);
    Serial << "Sharing task has run successfully" << endl;


    xTaskCreate (share_testing_receive,
                "test receiving",
                4096,
                NULL,
                3,
                NULL);
    Serial << "Receiving task has run successfully" << endl;


    // Start running the tasks, sir!
    vTaskStartScheduler ();



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