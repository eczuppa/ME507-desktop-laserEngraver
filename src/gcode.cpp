/** @file gcode.cpp
 *      This file defines functions for to interpret lines of gcode into commands 
 *      that control the laser cutter. 
 *
 *  @author  Niko Banks
 *  @date    Nov 10 2020 
 * 
*/


#include "libraries&constants.h"

#define GCODE_COMMENT ";"


//interpret gcode
void interpret_gcode_line(String line) 
{
    //If the line doesn't NOT contain a comment (so it does contain a comment):
    if (line.indexOf(GCODE_COMMENT) != -1)
    {
        Serial << "Comment Found" << endl;
    }
            

    return;
}
