/** @file       control.h
 *  @brief      File containing the header for the .cpp file that creates PID controllers.
 *  @details    This file contains the header for a class that implements a PID motor position control
 *              code to drive the motors given inputs of position, velocity, and delta time.
 * 
 *  @author Niko Banks
 *  @date  Aug 13 2022 Original file
 */

#ifndef CONTROLLER_2_H
#define CONTROLLER_2_H

class PID_Controller
{
protected:

    //PID Gains
    float _KP;    float _KI;    float _KD;

    // Controller deadband and saturation
    float _deadband;   
    float _high_sat_limit;     
    float _low_sat_limit;
    bool _controller_saturated;

    
    //Variable storage
    float _last_pos;    float _last_time;       
    float _dt;          float _drv_filt_tau;    float _drv_filt_coeff;

    //Error variables
    float _error = 0;       float _integral_error = 0;   
    float _last_error = 0;  float _raw_drv_error = 0;   float _derivative_error = 0;

    //Signal output
    float _output_signal;
 
public:
    //Constructor function: Initialize the controller
    PID_Controller(float KP, float KI, float KD, float deadband,float pos_sat_limit, float neg_sat_limit, float drv_filt_tau);

    //Run a main control loop
    float Run_Control_Loop(float pos, float pos_setpoint, float time);

    //Set-er functions
    void set_KP(float KP);
    void set_KI(float KI);
    void set_KD(float KD);
    void set_deadband(float deadband);
    void set_drv_filt_tau(float drv_filt_tau);

    //Get-er functions
    float get_KP(void);
    float get_KI(void);
    float get_KD(void);
    float get_deadband(void);
    float get_drv_filt_tau(void);
    float get_error(void);
    float get_drv_error(void);
    float get_int_error(void);

};

#endif //CONTROLLER_2_H