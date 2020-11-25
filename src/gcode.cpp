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

//Shares and queues should go here


/** @brief   Create a decoding class object to decode gcode.
 *  @details This constructor function doesn't need to do anything else except construct the class!
 */
decode::decode(void)
{
    //Constructor with nothing to construct!
}


/** @brief      Function which reads a single line of gcode and begins to decode it. 
 *  @details    This function reads a line of gcode and splits it up into the separate commands. 
 *              It then sends out the appropriate data to finish translation.
 *  @param      line A line of gcode to be interpreted. 
 */
void decode::interpret_gcode_line(char *line) 
{

    //Define variables for use in function: Decoding Gcode
    uint8_t char_counter = 0;  
    char letter;
    float value;
    int16_t int_value = 0;
    int16_t mantissa = 0;
    String str_line = line;     //Convert line into a string for use of string functions


    //Define state variables
    uint8_t move_type = NONE;

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
            // print_serial("Comment Found: ");
            // print_serial(str_line.substring(char_counter+1));
            // print_serial('\n'); //end transmission

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
                print_serial("Error in Gcode: Not starting with letter");
                _error_signal = SYNTAX_ERROR_LETTER;
                return; 
            }
            //Otherwise, it is a letter, all good! Not a comment or an invalid command. 
            //Move on to next character to read the number now. 
            char_counter++;

            //This function both reads the float of the number AND moves the count up to 
            //the first value after the number from Gcode. Returns false if error. 
            if ((!read_float(line, &char_counter, &value) ))
            {
                print_serial("Error in Gcode: Letter not followed by number");
                _error_signal = SYNTAX_ERROR_NUMBER;
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
                            //set feedrate for traveling
                            _XYSFval.F = TRAVEL_SPEED;
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
                            _go_home = 1;
                            break;
                        case 90:
                            //Set absolute positioning: NOT sent to matrix
                            break;
                        case 91:
                            //Set incremental positioning: NOT sent to matrix
                            break;
                        default:
                            //Error: Unsupported Gcode
                            print_serial("ERROR: Unsupported G Command in line __");
                            _error_signal = G_COMMAND_ERROR;
                    }
                    break;
                case 'M':
                    //Interpret M-codes
                    switch(int_value)
                    {
                        case 2:
                            //End program
                            _gcode_running = 0;
                            break;
                        case 3:
                            //Enable Laser
                            _laser_enable = 1;
                            break;
                        case 5:
                            //Disable laser
                            _laser_enable = 0;
                            break;
                        default:
                            //ERROR: Unsupported Mcode
                            print_serial("ERROR: Unsupported M Command in line __");
                            _error_signal = M_COMMAND_ERROR;
                    }
                    break;

                case 'X':
                    // Change/set X position if indicated correctly
                    // if (move_type == LIN_INTERP or move_type == TRAVEL)
                    // {
                        _XYSFval.X = value;
                    // }
                    // else
                    // {
                    //     print_serial("Error in line __: No G0 or G1 indicator");
                    //     _error_signal = MOVE_ERROR;
                    // }
                    // break;

                case 'Y':
                    // Change/set Y position
                    // if (move_type == LIN_INTERP or move_type == TRAVEL)
                    // {
                        _XYSFval.Y = value;
                    // }
                    // else
                    // {
                    //     print_serial("Error in line __: No G0 or G1 indicator");
                    //     _error_signal = MOVE_ERROR;
                    // }
                    // break;

                case 'S':
                    // // Change/set laser PWM power
                    // if (move_type == LIN_INTERP or move_type == TRAVEL)
                    // {
                        _XYSFval.S = int_value*100 + mantissa;     //Power interpreted as pct (0 to 100)
                    // }
                    // else
                    // {
                    //     print_serial("Error in line __: No G0 or G1 indicator");
                    //     _error_signal = MOVE_ERROR;
                    // }
 
                    // break;

                case 'F':
                    // //Change speed settings
                    // if (move_type == LIN_INTERP)
                    // {
                        _XYSFval.F = value;
                    // }
                    // else
                    // {
                    //     print_serial("Error in line __: No G1 indicator");
                    //     _error_signal = MOVE_ERROR;
                    // }
                    // break;

                default:
                    //ERROR: Unsupported command
                     print_serial("ERROR: Unsupported Letter Command in line __");
                     _error_signal = LETTER_CMD_ERROR;
                    
            }//switch (letter)
        }//else (Not a comment or space)
    }
    
    // ------------------------- And the meek shall inherit the earth... ----------------------------

    return;
}



void decode::gcode_initialize(void)
{
    _gcode_running = 1;
}


//Get-er functions:

XYSFvalues decode::get_XYSF(void)
{
    return _XYSFval;
}