/** @file kinemtatics.cpp
 *  @details    This file contains functions to convert desired X and Y positions and velocities
 *              into desired A and B positions and velocities for motors A and B in a CoreXY 
 *              model. 
 *
 *  @author  Niko Banks
 *  @author  Ethan A Czuppa
 *  @date    Nov 16 2020  Original File
 *  @date    Nov 18 2020 added basic functionality
 * 
 * 
*/
#include "libraries&constants.h"
using namespace BLA;

/** @brief   Create a kinematics class object to convert coordinates to motor commands. 
 *  @details This constructor function runs a reset on the _last values in the class member
 *           data, having the machine start at an effective homed position. 
 */
Kinematics_coreXY::Kinematics_coreXY()
{
    //Create Zeroth line for what the home 
    _X_last = 0;
    _Y_last = 0;
    _F_last = 0;
}
// Methods
    
// save current gcode X command to class member data
void Kinematics_coreXY::update_XYF(decode decoder)
{
    //Send current values to be the new last values
    _X_last = _X_current;
    _Y_last = _Y_current;
    _F_last = _F_current;

    //Get new current values from the gcode decoder 
    _X_current = decoder._XYSFval.X;
    _Y_current = decoder._XYSFval.Y;
    _F_current = decoder._XYSFval.F;
}


// Takes X and Y position commands and returns Motor A setpoint
void Kinematics_coreXY::transform_A(void)
{
    _A_setpoint = _X_current - _Y_current;
    // return (_A_setpoint);
}


// Takes X and Y position commands and creates Motor B setpoint
void Kinematics_coreXY::transform_B(void)
{
    _B_setpoint = -_X_current - _Y_current;
    // return (_B_setpoint);
}  


// Takes F, and X,Y current and Last and returns Motor A_F and B_F
void Kinematics_coreXY::transform_F(void)
{
    // Gets the vector components of the laser velocity between the current and last points
    // and returns the transformed A and B feed rates.
    Matrix<1,2> dir_Vector;
    //Matrix<1,2> feed_Vector;
    float magnitude;
    dir_Vector.Fill(0);
    //feed_Vector.Fill(0);
    magnitude = sqrt(pow((_X_current - _X_last),2) + pow((_Y_current - _Y_last),2));  // Find distance between points
    // Make unit vector and apply the magnitude of the feed rate to it (Cartesian coordinates)
    dir_Vector(1,1) = ((_X_current - _X_last)*_F_current)/magnitude;
    dir_Vector(1,2) = ((_Y_current - _Y_last)*_F_current)/magnitude;
    // Transform X and Y components of the dir_vector scaled by F to velocity setpoints for motor A and B.
    _A_feed =   dir_Vector(1,1) - dir_Vector(1,2);
    _B_feed = - dir_Vector(1,1) - dir_Vector(1,2);
    // return _A_feed; 
}  


// void Kinematics_coreXY::transform_F_B(void)
// {
//     // Gets the vector components of the laser velocity between the current and last points
//     // and returns the transformed A and B feed rates.
//     Matrix<1,2> dir_Vector;
//     //Matrix<1,2> feed_Vector;
//     float magnitude;
//     dir_Vector.Fill(0);
//     //feed_Vector.Fill(0);
//     magnitude = sqrt(pow((_X_current - _X_last),2) + pow((_Y_current - _Y_last),2));  // Find distance between points
//     // Make unit vector and apply the magnitude of the feed rate to it (Cartesian coordinates)
//     dir_Vector(1,1) = ((_X_current - _X_last)*_F_current)/magnitude;
//     dir_Vector(1,2) = ((_Y_current - _Y_last)*_F_current)/magnitude;
//     // Transform X and Y components of the dir_vector scaled by F to velocity setpoints for motor A and B.
    
    
//     // return _B_feed;
// }


/** @brief      Function which runs the kinematics functions in succession
 *  @details    This function runs the kinematics functions to take a new X Y and F value from the 
 *              gcode interpreter and convert them into the motor A and B positions and feedrates. 
 */
void Kinematics_coreXY::calculate_kinematics(decode decoder)
{
    update_XYF(decoder);
    transform_A();
    transform_B();
    transform_F();
}



//Reset X Y and F last values back to the origin
void Kinematics_coreXY::reset_XYF(void)
{
    // Create Zeroth line for what the home point would be
    // Allows user to run multiple files and reset all last GCODE values to 0 
    // when the current file reaches the end and the laser is now ready for a new file
    _X_last = 0;
    _Y_last = 0;
    _F_last = 0;
}


//Get-er functions
float Kinematics_coreXY::get_A_setpoint(void)
{
    return _A_setpoint;
}

float Kinematics_coreXY::get_B_setpoint(void)
{
    return _B_setpoint;
}

float Kinematics_coreXY::get_F_A(void)
{
    return _A_feed;
}

float Kinematics_coreXY::get_F_B(void)
{
    return _B_feed;
}