/** @file gcode.h
 *    This file contains the header for a gcode interpreting function.
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


//Function to interpret gcode
void interpret_gcode_line(char *line);


#endif //GCODE_H