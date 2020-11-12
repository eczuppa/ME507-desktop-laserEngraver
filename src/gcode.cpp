/** @file gcode.cpp
 *      This file defines functions for to interpret lines of gcode into commands 
 *      that control the laser cutter. 
 *
 *  @author  Niko Banks
 *  @date    Nov 10 2020 
 * 
*/
#include "libraries&constants.h"


//This function will interpret a single line of Gcode to enter into the main command array. 
void interpret_gcode_line(char *line) 
{
    //Define constants for use in actual function
    uint8_t char_counter = 0;  
    char letter;
    float value;
    uint16_t int_value = 0;
    uint16_t mantissa = 0;
    String str_line = line;     //Convert line into a string for use of string functions

    //Start looping through the line, character by character. Check each character to match 
    //it with certain commands. 
    while(line[char_counter] != '\0')
    {
        //Get letter of line position
        letter = line[char_counter];

        //Check if letter signifies a comment in the gcode
        if (letter == GCODE_COMMENT)
        {
            //Find a comment, print it!
            Serial << "Comment Found: " << str_line.substring(char_counter+1) << endl;
            //Once comment has been found, skip to the end of the line to end the line loop. 
            char_counter = str_line.length();
        }

        //If we hit a space, move past it.
        else if (letter == ' ')
        {
            char_counter++;
        }
        //If not a comment or a space, check for a letter command. 
        else
        {
            //If not a letter...
            if((letter < 'A') || (letter > 'Z')) 
            { 
                //REPLACE WITH RTOS PRINTER FUNCTION
                Serial << letter << "Error in Gcode: Not starting with letter" << endl;
                return;
            }
            //Otherwise, it is a letter, all good! Not a comment or an invalid command. 
            //Move on to next character to read the number now. 
            char_counter++;

            //This function both reads the float of the number AND moves the count up to 
            //the first value after the number from Gcode. Returns false if error. 
            if ((!read_float(line, &char_counter, &value) ))
            {
                //REPLACE WITH RTOS PRINTER FUNCTION
                Serial << "Error in Gcode: Letter not followed by number" << endl;
                return;
            }
            //At this point, we have a letter and a floating point number. Now, lets get
            //the float into terms we can understand.

            // FROM GRBL: 
            // Convert values to smaller uint8 significant and mantissa values for parsing this word.
            // NOTE: Mantissa is multiplied by 100 to catch non-integer command values. This is more 
            // accurate than the NIST gcode requirement of x10 when used for commands, but not quite
            // accurate enough for value words that require integers to within 0.0001. This should be
            // a good enough comprimise and catch most all non-integer errors. To make it compliant, 
            // we would simply need to change the mantissa to int16, but this add compiled flash space.
            int_value = trunc(value);
            mantissa =  round(100*(value - int_value)); // Compute mantissa for Gxx.xx commands.
            // NOTE: Rounding must be used to catch small floating point errors. 

            //How to interpret commands: take information and send to main code array.s
            switch (letter)
            {
                case 'G':
                    //Interpret G codes
                    switch(int_value)
                    {
                        case 0:
                            //Rapid movement (travel)
                            break;
                        case 1: 
                            //Linear Interpolation
                            break;
                        case 20:
                            //Unit conversion to in
                            break;
                        case 21:
                            //Unit conversion to mm
                            break;
                        case 28:
                            //Home machine
                            break;
                        case 90:
                            //Set absolute positioning
                            break;
                        case 91:
                            //Set incremental positioning
                        default:
                            //Error: Unsupported Gcode
                        
                    }
                    break;
                case 'M':
                    //Interpret M-codes
                    switch(int_value)
                    {
                        case 2:
                            //End program
                            break;
                        case 3:
                            //Enable Laser
                            break;
                        case 5:
                            //Disable laser
                            break;
                        default:
                            //ERROR: Unsupported Mcode
                    }
                    break;
                case 'X':
                    //Change X position
    
                    break;
                case 'Y':
                    //Change Y position
 
                    break;
                case 'S':
                    //Change laser PWM power
                    break;
                case 'F':
                    //Change speed settings
                    break;
                default:
                    //ERROR: Unsupported command

            }
        }
    }
    return;
}
