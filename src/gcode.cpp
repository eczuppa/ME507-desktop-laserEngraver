/** @file gcode.cpp
 *      This file defines functions for to interpret lines of gcode into commands 
 *      that control the laser cutter. 
 *
 *  @author  Niko Banks
 *  @date    Nov 10 2020 
 * 
*/


#include "libraries&constants.h"

#define GCODE_COMMENT ';'


//interpret gcode
void interpret_gcode_line(String line) 
{
    //Define constants for use in actual function
    uint8_t char_counter = -1;  
    char letter;


    while(line[++char_counter] != '\0')
    {
        //Get letter of line position
        letter = line[char_counter];

        //Check if letter signifies a comment in the gcode
        if (letter == GCODE_COMMENT)
        {
            Serial << "Comment Found: " << line.substring(char_counter+1) << endl;
            //Once comment has been found, skip to the end of the line to end the loop. 
            char_counter = line.length();
        }
        //If not a comment, check for the letter command
        else 
        {
            if((letter < 'A') || (letter > 'Z')) 
            { 
            Serial << letter << " is not a letter" << endl;
            } 
            // [Expected word letter]
            // if (!read_float(line, &char_counter, &value)) { FAIL(STATUS_BAD_NUMBER_FORMAT); } // [Expected word value]
        }
    }
    return;
}
