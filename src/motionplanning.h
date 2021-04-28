// /** @file       motionplanning.h 
//  *  @brief      This file contains the header file for the motion planning class
//  * 
//  *  @author Ethan A Czuppa
//  *  @date Nov 19 2020 Original file
//  *  
//  */

// //  * 
// //  *  @date    20 Nov 2020 Mapped out header file and needed functions as pseudocode
// //  *  @date    22 Nov 2020 Completed first revision of class, need to fix the memory allocation so the dynamic array does not point to
// //  *                       nothing instead of the intended ouput ramp element. 
// //  *  @date    23 Nov 2020 Completed second revision of class, dynamic allocation of array memory is now done in a separate method and 
// //  *                       required to have succeeded for the ramp generator method to run. Uses the trick Dr. Ridgely mentioned in 
// //  *                       lecture of checking that the pointer to the dynamic array points to something instead of zero as an indicator
// //  *                       that the memory allocation was successful
// //  *  @date    30 Nov 2020 Updated doxygen documentation and added an important class method. The @c update_setpoints() was added and this
// //  *                       method that allows the current position and feed rate setpoints to be changed without reinstantiating the class. 
// //  * 

 


// #ifndef MOTIONPLANNING_H
// #define MOTIONPLANNING_H

// #include "libraries&constants.h"

//  /** @brief  a class that implements a linearly interpolated ramp input generator for a classical control loop  
//  *   @details Generates a linearly interpolated ramp input for use in P, PI, and PID control loop between two lines of parsed GCODE.
//  *            This was implemented in the form of a class in order to avoid having to make a large number of static variables and to
//  *            do cleaner dynamic memory allocation for the array which holds the generated ramp input. By providing our classical 
//  *            control loops with a ramp input instead of a step input we are able to control both position and velocity with a PID 
//  *            postion control loop. Additionally, the classical control loop behaves in a linear manner since it is subjected to several smaller
//  *            disturbances instead of one large step disturbance. Maintaining linear control loop behavior eases parameterization of 
//  *            the closed loop system as well as reduces the likelyhood for eratic controller behavior due to non-linearities. 
//  *   
//  *           @section MotionPlanning_usage Usage
//  *              
//  *      
//  */

// class MotionPlanning
// {
//     protected:
//     // NOTE: make motion planning a friend of the kinematics class to get the transformed struct A_curr, A_last, A_feed and B_curr, B_last, B_feed
//     // variables to store array individual values and/or the whole XYF array
    
//     // a Motor 
//     float _setpoint_current;        /// Stores current target position (in mm)
//     float  _setpoint_last;          /// stores last target postion (in mm)
//     float _feed_setpoint;           /// store the target feed rate for the motor
//     float _delta_dist;              /// the distance between the current and last setpoints
//     float _tot_travel_period;       /// the amount of time required to travel from last to current point based on the current feedrate
//     uint32_t _ramp_dt_period;       /// the dt for each section of the ramp input
//     uint32_t _ramp_sections;        /// the number of discrete sections of the ramp input 
//     float *_output_ramp;            /// the pointer to the float array of linearly interpollated portions of the overall movement specified between the lines of G-code
//     int16_t *_output_ramp_time;       /// the pointer to the int_16 array of ramp_dt_periods for each _ramp_section in _output_ramp
//     bool _even = false;             /// boolean for how to adjust the index of the dynamically allocated array which stores the ramp input



//     public:
//     // a constructor that probably takes in the given array or individual variables X,Y,F current and last as well as the discretization period
//     MotionPlanning(float setpoint_current, float setpoint_last, float feed_setpoint, uint32_t ramp_dt_period);
//     // a method to dynamically allocate memory for the ramp generator method
//     void init_ramp(void);
//     // a method to calculate the discrete sections of the ramp input to the controller based on the current and last lines of GCODE (from a completion standpoint - last is just completed, current is what will be)
//     void ramp_generator(void);
//     // a getter method to get the discrete movements 
//     float * get_ramp(void);
//     // a method to delete the dynamically allocated memory when all of it has been queued up
//     void deinit_ramp(void);
//     // get-er method for number of ramp sections in ramp
//     uint32_t get_ramp_len(void);
//     // a method to change the current setpoint and feedrate after deiniting the ramp in preparation for generating a new ramp
//     void update_setpoints(float current_setpoint, float current_feedrate);
    
//     // tells the queuing task which sends the control loops their next input at a fixed time rate to
//     // take the descritized _output_ramp array elements and make them into an actual ramp input to the control loops in control
//     int16_t * time_to_wait(void); 

// };

// #endif //MOTIONPLANNING_H