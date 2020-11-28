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

//Define error messages
#define NO_ERROR 0
#define SYNTAX_ERROR_LETTER 1
#define SYNTAX_ERROR_NUMBER 2
#define G_COMMAND_ERROR 3
#define M_COMMAND_ERROR 4
#define MOVE_ERROR 5
#define LETTER_CMD_ERROR 6




///Create struct types for output data from @c interpret_gcode_line() 
    //Define struct of variables for use in function: Using decoded Gcode
    struct XYSFvalues
    {
        float X = 0;
        float Y = 0;
        uint8_t S = 0;
        float F = 0;
    };



/** @brief   Class which implements decoding object which contains functions for decoding
 *           gcode.
 *  @details This class allows us to read a line of gcode, decode it, and store the necessary information 
 *           into class member data. This is a convenient way to store and pass along information as it
 *           updates with each new command.
 */
class decode
{
protected:
    ///struct with values for X, Y, S, and F 
    XYSFvalues _XYSFval;

    ///Laser state
    bool _laser_enable = 0;

    ///Set units (default in millimeters)
    bool _units = MILLIMETERS;

    ///Error code signal
    uint8_t _error_signal = NO_ERROR;

    ///Homing Signal
    bool _go_home = 0;

    ///Signal when end of Gcode reached
    bool _gcode_running = 0;

public:
    ///Constructor
    decode(void);

    ///Function to interpret gcode
    void interpret_gcode_line(char *line);

    ///Initialize gcode reading
    void gcode_initialize(void);

    ///Get-er functions:
    XYSFvalues get_XYSF(void);


    ///Friend class Kinematics, so Kinematics can access the class member data:
    friend class Kinematics_coreXY;
};

#endif //GCODE_H