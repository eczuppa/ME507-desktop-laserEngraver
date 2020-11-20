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


//This function will interpret a single line of Gcode to enter into the main command array. 
void interpret_gcode_line(char *line) 
{
    //Define variables for use in function: Decoding Gcode
    uint8_t char_counter = 0;  
    char letter;
    float value;
    int16_t int_value = 0;
    int16_t mantissa = 0;
    String str_line = line;     //Convert line into a string for use of string functions

    //Define variables for use in function: Using decoded Gcode
    float X = 0;
    float Y = 0;
    uint8_t S = 0;
    float F = 0;

    //Define state variables
    uint8_t move_type = NONE;
    bool units = MILLIMETERS;

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
            print_serial("Comment Found: ");
            print_serial(str_line.substring(char_counter+1));
            print_serial('\n');
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
                print_serial("Error in Gcode: Not starting with letter");
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
                print_serial("Error in Gcode: Letter not followed by number");
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

            //Test commands to see that we're interpreting right
            // Serial << "Command is: " << letter << int_value << "." << mantissa << endl;


            
            //How to interpret commands: take information and send to main code array.s
            switch (letter)
            {
                case 'G':
                    //Interpret G codes
                    switch(int_value)
                    {
                        case 0:
                            //Rapid movement (travel)
                            move_type = TRAVEL;
                            break;
                        case 1: 
                            //Linear Interpolation
                            move_type = LIN_INTERP;
                            break;
                        case 20:
                            //Unit conversion to in
                            break;
                        case 21:
                            //Unit conversion to mm (default)
                            break;
                        case 28:
                            //Home machine
                            break;
                        case 90:
                            //Set absolute positioning: NOT sent to matrix
                            break;
                        case 91:
                            //Set incremental positioning: NOT sent to matrix
                        default:
                            //Error: Unsupported Gcode
                            print_serial("ERROR: Unsupported G Command in line __");
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
                            print_serial("ERROR: Unsupported M Command in line __");
                    }
                    break;

                case 'X':
                    // Change/set X position if indicated correctly
                    if (move_type == LIN_INTERP or move_type == TRAVEL)
                    {
                        X = value;
                        //Send value to output?
                    }
                    else
                    {
                        print_serial("Error in line __: No G0 or G1 indicator");
                    }
                    break;

                case 'Y':
                    // Change/set Y position
                    if (move_type == LIN_INTERP or move_type == TRAVEL)
                    {
                        Y = value;
                        //Send value to output?
                    }
                    else
                    {
                        print_serial("Error in line __: No G0 or G1 indicator");
                    }
                    break;

                case 'S':
                    // Change/set laser PWM power
                    if (move_type == LIN_INTERP or move_type == TRAVEL)
                    {
                        S = int_value*100 + mantissa;     //Power interpreted as pct (0 to 100)
                        //Send value to output?
                    }
                    else
                    {
                        print_serial("Error in line __: No G0 or G1 indicator");
                    }
 
                    break;

                case 'F':
                    //Change speed settings
                    if (move_type == LIN_INTERP or move_type == TRAVEL)
                    {
                        F = value;
                        //Send value to output?
                    }
                    else
                    {
                        print_serial("Error in line __: No G0 or G1 indicator");
                    }
                    break;

                default:
                    //ERROR: Unsupported command
                     print_serial("ERROR: Unsupported Letter Command in line __");
                    
            }//switch (letter)
        }//else (Not a comment or space)
    }

    return;
}
