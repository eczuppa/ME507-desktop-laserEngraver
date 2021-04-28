// /** @file       kinematics.h 
//  *  @brief      File containing the interface for the kinematics class that wil translate GCODE inputs into a usable 
//  *              type for the control loop.
//  *  @details    This file contains a class which uses the appropriate kinematic equations for CoreXY to relate Gcode X,Y 
//  *              positions and F feedrate to values of the same type as the motor encoders output. 

//  *  
//  *  @author Ethan A Czuppa
//  *  @author Niko Banks
//  *  @author Matthew Carlson
//  * 
//  *  @date 16 Nov 2020 wrote skeleton of module for kinematics_CoreXY class, added doxygen comments for documentation
//  * 
//  */


// /** @brief      Class which contains functions to convert from cartesian global coordinates to motor commands.
//  *  @details    This class allows us to convert positions in X and Y and absolute speed (Feedrate) F
//  *              into motor-specific (A and B) positions and feedrates for controling the laser head, assuming a 
//  *              CoreXY belt system. The CoreXY transformation significantly increases the ease of controlling the 
//  *              motors and only requires the proper transformation of the desired target positions, velocities, etc. 
//  *              from the Gcode. CoreXY concepts used from Ilan E. Moyer - one of the earlier authors of CoreXY 
//  *              implementation and theory his website is available here: https://corexy.com/theory.html
//  */
// class Kinematics_coreXY
// {
//     protected:
//     // Class member data 
//     // variable to store the last GCODE X and Y positions, and Feedrate F
//     float _X_last;
//     float _Y_last;
//     float _F_last;          //Only necessary if we do acceleration control; not available at this time. 
    
//     // variables to store the currentGCODE X and Y positions, and Feedrate F
//     float _X_current;
//     float _Y_current;
//     float _F_current;

//     // variables to hold transformed result of X,Y position FOR CURRENT GCODE COMMAND specific to a motor 
//     float _A_setpoint;
//     float _B_setpoint;

//     //Feed rate variables for both motors
//     float _A_feed;
//     float _B_feed;

//     // Method specific variables that do not need to be saved 
//     // variable to hold cartesian x-componet of feed rate based on direction of path between the current command and last command

//     // variable to hold cartesian y-componet of feed rate based on direction of path between the current command and last command

//     // reference:
//     // A and B directions are with counterclockwise positive motion
//     // delta_A =  delta_X - delta_Y
//     // delta_B = -delta_X - delta_Y
//     // delta_X = 1/2*( delta_A - delta_B)
//     // delta_Y = 1/2*(-delta_A - delta_B)
//     // it also holds that 
//     // A = x - y;  B = -x - y;

//     public:
//     // Constructor
//     Kinematics_coreXY();

//     // Methods
    
//     // Route inputted floats from array to popper class member data
//     void update_XYF(decode decoder);                                // takes in new _current X Y and F values and sends the old ones to _last
//     void reset_XYF(void);                                           // sets the _last X,Y,F data to 0 for the home position in preparation to run another file


//     // Kinematic transformation calculation methods
//     void transform_A(void);        // Takes X and Y position commands and returns Motor A setpoint
//     void transform_B(void);        // Takes X and Y position commands and creates Motor B setpoint
//     void transform_F(void);      // Takes F, and X,Y current and Last and returns Motor A_F

//     //Run all the necessary functions to take new X Y and F commands and turn them into updated A and B commands
//     void calculate_kinematics(decode decoder);


//     // Get-er methods for quickly accessing saved class member data
//     float get_A_setpoint(void);
//     float get_B_setpoint(void);
//     float get_F_A(void);
//     float get_F_B(void);

//     // save space and variables by making MotionPlanning a friend of Kinematic_CoreXY
//     friend class MotionPlanning;


// };
    
