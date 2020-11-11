/** @file nuts_bolts.h
 *      This file contains the header to the file nuts_bolts.cpp, which contains rudimentary
 *      coding subfunctions that are used in other functions. 
 *      
 *
 *  @author Niko Banks
 *  @date Nov 10 2020
 */

// This code prevents errors if this file is #included more than once
#ifndef NUTS_BOLTS_H
#define NUTS_BOLTS_H

#include "libraries&constants.h"

//Function to convert strings of numbers into floats (for gcode interpreting)
uint8_t read_float(char *line, uint8_t *char_counter, float *float_ptr);             

#endif //NUTS_BOLTS_H