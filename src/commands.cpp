/** @file commands.cpp
 *      This file defines functions that are used to translate a line of incoming data into specific motor 
 *      commands that are sent to the control task. 
 *
 *  @author  Niko Banks
 *  @date    Nov 10 2020 
 * 
*/
#include "libraries&constants.h"

//Shares and queues should go here
extern Queue<char[LINE_BUFFER_SIZE]> read_chars;

Queue<float> A_pos_desired(POS_Q_SIZE,"A Position Desired");
Queue<float> B_pos_desired(POS_Q_SIZE,"B Position Desired");
// Queue<float> A_feed_desired();
// Queue<float> B_feed_desired();


/** @brief      Task which reads data from the serial port, translates it, and sends it where it needs to go.
 *  @details    This task function reads a line from the read_chars queue of gcode or other commands 
 *              and splits it up into the separate commands. Commands are then sent to the control task via queues
 *              and shares. Gcode is translated from @c X @c Y and @c F values into desired positions and feedrates 
 *              for both motors; @c _A_setpoint, @c _A_feed, @c _B_setpoint and @c _B_feed, which are class member 
 *              data of the class @c Kinematics_coreXY. Regular machine commands such as homing are also translated 
 *              to motor commands, which are then sent to the control task so it may run them. 
 */
void task_translate(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning
    // Set the timeout for reading from the serial port to the maximum
    // possible value, essentially forever for a real-time control program
    Serial.setTimeout (0xFFFFFFFF);

    //Initialize translator and decoder class members 
    Kinematics_coreXY translator;
    decode decoder;

    ///Create line char array to hold incoming data from the @c read_chars queue
    char line[LINE_BUFFER_SIZE];

    for(;;)
    {   
        //If there is a read line of gcode/commands in the queue...
        if (read_chars.any())
        {
            //Read the line
            read_chars.get(line);
            //Interpret the line (data is stored in decoder class)
            decoder.interpret_gcode_line(line);
            //update kinematic translator with new values
            translator.calculate_kinematics(decoder);

            //Put translated items into queues
            A_pos_desired.put(translator.get_A_setpoint());
            B_pos_desired.put(translator.get_B_setpoint());

            // float A = translator.get_A_setpoint();
            // float B = translator.get_B_setpoint();
            // float FA = translator.get_F_A();
            // float FB = translator.get_F_B();
            // print_serial(A);
            // print_serial(" ");
            // print_serial(B);
            // print_serial(" ");
            // print_serial(FA);
            // print_serial(" ");
            // print_serial(FB);
            // print_serial("\n");
        }
        vTaskDelay(100);
    }
}



void interpret_machinecmd_line(char *line)
{

}