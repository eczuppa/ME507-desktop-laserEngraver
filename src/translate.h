/** @file       translate.h
 *  @brief      This file contains the header for the translate.cpp file. 
 *              
 *  
 *  @author Niko Banks
 *  @date    4-26-2021 File Created
 *  @date    4-26-2021 Last Updated
 * 
 */

#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "libraries&constants.h"

// ========================================== Constants ========================================== 

// Queue size for ramp segment coefficient structs
#define RAMP_COEFFICIENT_Q_SIZE 32

#define MACHINE_CMD_NULL 0
#define MACHINE_CMD_HOME 1



// =========================================== Structs =========================================== 


/// This struct is what is returned from @c setpoint_of_time::get_desired_pos_vel(). It contains the position
/// and velocity setpoints for each motor, to be used in the control loop. 
struct motor_setpoint
{
    float A_pos = 0;
    float B_pos = 0;
    float A_vel = 0;
    float B_vel = 0;
};


//Struct to contain all desired values to be sent from coreXY_to_AB to the ramp creator. 
struct ramp_segment_coefficients
{
    float t0     = 0;   //Initial time of ramp segment
    float t_end  = 0;   //End time of ramp segment
    float pos_A0 = 0;   //Initial A position
    float pos_B0 = 0;   //Initial B position
    float vel_A  = 0;   //Velocity of A
    float vel_B  = 0;   //Velocity of B
    uint8_t S    = 0;   //Laser PWM signal
};





// =========================================== Classes ===========================================



/** @brief      Different way to motion plan- give any desired position for the time specified. 
 *  @details    Defined as a class to have a constant set of segment coefficients (_seg_coeff) that is only updated when 
 *              necessary; otherwise, this probably didn't have to be a class. Maybe could have used 
 *              "const ramp_segment_coeff" as the type on this variable instead? Oh well. 
 */ 
class setpoint_of_time
{
    protected:
    ramp_segment_coefficients _seg_coeff;       //Saved segment coefficients (all values initialized as 0)

    public: 
    //Contstuctor of the class
    setpoint_of_time(void);

    motor_setpoint get_desired_pos_vel(float time);                 //Get desired position and velocity
};






/** @brief      Class which contains functions to convert from cartesian global coordinates to motor commands.
 *  @details    This class allows us to convert positions in X and Y and absolute speed (Feedrate) F
 *              into motor-specific (A and B) positions and velocities for controling the laser head, assuming a 
 *              CoreXY belt system. More info on coreXY: https://corexy.com/theory.html
 */
class coreXY_to_AB
{
    protected:  // Class member data 
    
    XYSFvalues _last_XYSF;                  // XYSF values from the previous set of coordinates

    ramp_segment_coefficients _ramp_coeff;  // Struct of ramp coefficients to transform to

    public:
    // Constructor:
    coreXY_to_AB(void);

    // Run all the necessary functions to take new X Y and F commands, turn them into updated A and B commands, and put 
    // them into the corret queue. 
    void translate_to_queue(decode decoder);

     // Take XYSF values and create desired ramp coefficient struct
    ramp_segment_coefficients calc_ramp_coeff(XYSFvalues XYSF_input);     

    // Reset class data
    void reset(void);  

};


// =========================================== Functions ===========================================


//Task function to translate and send out necessary control data
void task_translate(void* p_params);

//function which interprets a machine command
uint8_t interpret_machinecmd_line(char *line);


#endif //TRANSLATE_H