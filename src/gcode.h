/** @file gcode.h
 *    This file contains the header for a file comtaining gcode interpreting functions.
 * 
 *    The full Doxygen header for the function is in the .cpp file, so there is
 *    just a brief description of the function here. 
 *
 *  @author Niko Banks
 *  @date Nov 10 2020
 */

// This code prevents errors if this file is #included more than once
#ifndef GCODE_H
#define GCODE_H


#define GCODE_COMMENT ';'

#define TRAVEL_SPEED 600    // mm/min

//Define travel types
#define TRAVEL 0
#define LIN_INTERP 1
#define NONE 2

//Define unit systems
#define MILLIMETERS 0
#define INCHES 1


//Function to interpret gcode
void interpret_gcode_line(char *line);


#endif //GCODE_H