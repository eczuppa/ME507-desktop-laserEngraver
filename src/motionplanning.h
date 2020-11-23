#ifndef MOTIONPLANNING_H
#define MOTIONPLANNING_H

#include "libraries&constants.h"


class MotionPlanning
{
    protected:
    // NOTE: make motion planning a freind of the kinematics class to get the transformed struct A_curr, A_last, A_feed and B_curr, B_last, B_feed
    // variables to store array individual values and/or the whole XYF array
    // class member data to store the discretization period
    
    // a Motor 
    float _setpoint_current;        // Stores current target position (in mm)
    float  _setpoint_last;          // stores last target postion (in mm)
    float _feed_setpoint;           // store the target feed rate for the motor
    float _delta_dist;              // the distance between the current and last setpoints
    float _tot_travel_period;       // the amount of time required to travel from last to current point based on the current feedrate
    uint32_t _ramp_dt_period;       // the dt for each section of the ramp input
    uint32_t _ramp_sections;        // the number of discrete sections of the ramp input 
    float *_output_ramp;            // the pointer to the float array of linearly interpollated portions of the overall movement specified between the lines of G-code
    bool _even = false;             // boolean for how to adjust the index of the dynamically allocated array which stores the ramp input



    public:
    // a constructor that probably takes in the given array or individual variables X,Y,F current and last as well as the discretization period
    MotionPlanning(float setpoint_current, float setpoint_last, float feed_setpoint, uint32_t ramp_dt_period);
    // a method to dynamically allocate memory for the ramp generator method
    void init_ramp(void);
    // a method to calculate the discrete sections of the ramp input to the contorller based on the current and last lines of GCODE (from a completion standpoint - last is just completed, current is what will be)
    void ramp_generator(void);
    // a getter method to get the discrete movements 
    float * get_ramp(void);
    // a method to delete the dynamically allocated memory when all of it has been queued up
    void deinit_ramp(void);


};

#endif //MOTIONPLANNING_H