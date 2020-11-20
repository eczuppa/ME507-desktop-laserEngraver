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

Kinematics_coreXY::Kinematics_coreXY()
{
    //Create Zeroth line for what the home 
    _X_last = 0;
    _Y_last = 0;
    _F_last = 0;
}
// Methods
    
// save current gcode X command to class member data
void Kinematics_coreXY::set_current_XYF (float g_X_c, float g_Y_c, float g_F_c)
{
    _X_current = g_X_c;
    _Y_current = g_Y_c;
    _F_current = g_F_c;
}   
  
void Kinematics_coreXY::update_XYF(void)
{
    _X_last = _X_current;
    _Y_last = _Y_current;
    _F_last = _F_current;
}
// Takes X and Y position commands and returns Motor A setpoint
void Kinematics_coreXY::transform_A(void)
{
    _A_setpoint = _X_current + _Y_current;
    // return (_A_setpoint);
}
// Takes X and Y position commands and creates Motor B setpoint
void Kinematics_coreXY::transfrom_B(void)
{
    _B_setpoint = _X_current - _Y_current;
    // return (_B_setpoint);
}  
// Takes F, and X,Y current and Last and returns Motor A_F and B_F
void Kinematics_coreXY::transform_F_A(void)
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
    _A_feed = dir_Vector(1,1) + dir_Vector(1,2);
    
    // return _A_feed; 

}  

void Kinematics_coreXY::transform_F_B(void)
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
    _B_feed = dir_Vector(1,1) - dir_Vector(1,2);
    
    return _B_feed;
}

void Kinematics_coreXY::reset_XYF(void)
{
    // Create Zeroth line for what the home point would be
    // Allows user to run multiple files and reset all last GCODE values to 0 
    // when the current file reaches the end and the laser is now ready for a new file
    _X_last = 0;
    _Y_last = 0;
    _F_last = 0;
}

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