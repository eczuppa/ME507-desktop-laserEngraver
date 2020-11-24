/** @file gcode.cpp
 *      This file defines functions for to interpret lines of gcode into commands 
 *      that control the laser cutter. Many of the methods used in this file were inspired
 *      by GRBL code.
 *
 *  @author  Niko Banks
 *  @date    Nov 10 2020 
 * 
*/
#include "libraries&constants.h"
#include <tuple>

//Shares and queues should go here
extern Queue<char[LINE_BUFFER_SIZE]> read_chars;


/** @brief      Task which reads gcode, translates it, and sends the data to where it needs to go.
 *  @details    This task function reads a line from the read_chars queue of gcode or other commands 
 *              and splits it up into the separate commands. 
 *              It then sends out the appropriate data to finish translation.
 *  @param      line A line of gcode to be interpreted. 
 */
void task_translate(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning
    // Set the timeout for reading from the serial port to the maximum
    // possible value, essentially forever for a real-time control program
    Serial.setTimeout (0xFFFFFFFF);

    Kinematics_coreXY translator;
    decode decoder;

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

            float A = translator.get_A_setpoint();
            float B = translator.get_B_setpoint();
            float FA = translator.get_F_A();
            float FB = translator.get_F_B();
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