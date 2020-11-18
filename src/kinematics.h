/** @file   kinematics.h the interface for the kinematics class that wil translate GCODE inputs into a usable type 
 *          for the control loop
 *  @details    Uses the appropriate kinematic equations to relate Gcode X,Y positions and F feedrate to values of 
 *              the same type as the motor encoders output. This transfomration significantly increases the ease of 
 *              controlling the motors and only requires the proper transformation of the desired target positions, 
 *              velocities, etc. from the Gcode. CoreXY concepts used from Ilan E. Moyer - one of the earlier authors 
 *              of CoreXY implementation and theory his website is available here: https://corexy.com/theory.html

 *  
 *  @author Ethan A Czuppa
 *  @author Niko Banks
 *  @author Matthew Carlson
 * 
 *  @date 16 Nov 2020 wrote skeleton of module for kinematics_CoreXY class, added doxygen comments for documentation
 * 
 */

class kinematics_coreXY
{
    protected:
    // Class member data 
    // FROM LAST GCODE COMMAND
    // variable to store the GCODE X position 100s, 10s, and 1s place value and decimal portion (aka the Mantissa)
    
    // variable to store the GCODE Y position 100s, 10s, and 1s place value and decimal portion (aka the Mantissa)

    // variable to store the GCODE F feed rate 100s, 10s, and 1s place value and decimal portion (aka the Mantissa)
    
    
    // FROM CURRENT GCODE COMMAND
    // variable to store the GCODE X position 100s, 10s, and 1s place value and decimal portion (aka the Mantissa)
    
    // variable to store the GCODE Y position 100s, 10s, and 1s place value and decimal portion (aka the Mantissa)

    // variable to store the GCODE F feed rate 100s, 10s, and 1s place value and decimal portion (aka the Mantissa)

    // variable to hold transformed result of X,Y position FOR CURRENT GCODE COMMAND specific to a motor 

    // variable to hold transfromed result of X,Y position FOR CURRENT GCODE COMMAND specific to a motor 

    // Method specific variables that do not need to be saved 
    // variable to hold cartesian x-componet of feed rate based on direction of path between the current command and last command

    // variable to hold cartesian y-componet of feed rate based on direction of path between the current command and last command

    // reference:
    // delta_A = delta_X + delta_Y
    // delta_B = delta_X - delta_Y
    // delta_X = 1/2*(delta_A + delta_B)
    // delta_Y = 1/2*(delta_A - delta_B)
    // it also holds that 
    // A = x + y;  B = x - y;

    public:
    kinematics_coreXY();

};
    
