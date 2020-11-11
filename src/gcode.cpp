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
void interpret_gcode_line(char *line) 
{
    //Define constants for use in actual function
    uint8_t char_counter = -1;  
    char letter;
    float value;


    while(line[++char_counter] != '\0')
    {
        //Get letter of line position
        letter = line[char_counter];

        //Check if letter signifies a comment in the gcode
        if (letter == GCODE_COMMENT)
        {
            //Find a comment, print it!
            // Serial << "Comment Found: " << line.substring(char_counter+1) << endl;

            //Once comment has been found, skip to the end of the line to end the line loop. 
            // char_counter = line.length();
        }

        //If not a comment, check for the letter commands.
        else 
        {
            //If not a letter...
            if((letter < 'A') || (letter > 'Z')) 
            { 
                Serial << letter << "Error in Gcode: Not starting with letter" << endl;
            }
            //Otherwise, it is a letter, all good! Not a comment or an invalid command. 
            //Move on and read the number now. 
            char_counter++;

            if ((!read_float(line, &char_counter, &value) )) // [Expected word value]
            {
                Serial << "Error in Gcode: Letter not followed by number" << endl;
            }

            Serial << letter;
        }
    }
    return;
}
