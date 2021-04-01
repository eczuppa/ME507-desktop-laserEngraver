/** @file       TB6612FNG_Driver.h
 *  @brief      This file contains the header for the class that contains necessary functions
 *              needed to use the Toshiba dual H-bridge motor driver chip.
 *              
 *  
 *  @author Ethan A Czuppa
 *  @date   Nov 6 2020 Original file
 */


//  *  @date 6 Nov 2020  Created Project and this file in parallel with doxygen comments for future documentation
//  *  @date 7 Nov 2020  finished first version of motor driver, needs to be updated to accept more references 
//  *                    since many inputs are predefined objects (pin names, other classes, etc. ) 
//  *                    Also I need to figure out how to pass the instantiation of the hardware timer class to
//  *                    through this class (e.g. TIM1, TIM2, TIM3, etc) so that the timer can be changed as needed
//  *                    instead of being set in the header file as it is currently. 
//  *  @date 10 Nov 2020 Got help from Dr. Ridgely for how to correctly pass timer objects into this class and was able
//  *                    to get it to the level of portability I desired. Motors work, though they are not particularly quiet
//  *                    due to being contained in what amounts to 3D printed echo chambers, but the slight increase in
//  *                    complexity for more capability than @c analogWrite() was worth the effort. Especially given the added
//  *                    guarantee that pins are correctly being put in their alternate function modes.
//  *  @date 15 Nov 2020 Added brake method to try to do power and brake vs power and coast driving style for the motors. This was 
//  *                    taken from sparkfun's arduino Library for the TB6612FNG from: 
//  *                    https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library/blob/master/src/SparkFun_TB6612.cpp 
//  *                    However, I am not sure how to implement this method into our PID controller... perhaps something
//  *                    that is watching for the error to drop to a certain threshold and then instead of settting the 
//  *                    PWM singal to 0 and allowing the motor to coast, the short brake method is called. Also, given its
//  *                    name - short brake - in the truth table I am skeptical of how effective it would be in the first place
//  * 


#ifndef TB6612FNGDRIVER_H
#define TB6612FNGDRIVER_H

#include "libraries&constants.h"

/** @brief   a motor driver class that is specific to Toshiba's TB6612FNG motor driver IC
 *  @details makes use of arduino commands to create a simple driver that interfaces with toshiba's 
 *           TB6612FNG dual H-bridge motor driver IC. The PWM signal generation is handeled by the
 *           Arduino @c analogWrite() and the direction pin settings for changing the direction of 
 *           the motor is done with Arduino's @c digitalWrite.
 */ 

class TB6612FNG 
{
    protected:
    // Create Variables to save as class member data that are only accessible by this class
    uint8_t _standby_pin;         ///< Motor Driver Standby Pin (Enable Pin for H-Bridge)
    uint8_t _motor_dir_pin_1;     ///< Motor Direction Pin 1 (Enables for which direction the motor will rotate in)
    uint8_t _motor_dir_pin_2;     ///< Motor Direction Pin 2 (Enables for which direction the motor will rotate in)
    uint8_t _pwm_input_pin;       ///< Motor Driver PWM input pin (Sets speed at which motor is driven)

    // Setup PWM timer
    // Timer Number and Timer Channel Number 
    // TIM_TypeDef * _p_timer;
    // HardwareTimer *MotorTmr;

    public:
    // Constructor and class methods here

    // Constructor
    TB6612FNG(uint8_t stby_pin, uint8_t mot_pin_1, uint8_t mot_pin_2, uint8_t a_pwm_pin);

    // Class Methods
    void setDutyCycle(float duty_cycle);    // Sets motor speed and direction based on sign of duty cycle input  
    void brake(void);                        // Makes motor hold in place?? (should be accomplished by PID position control) 
    void disable(void);                      // Software Emergency stop
    void enable(void);                       // Software reset of motor driver chip


};

#endif //TB6612FNGDRIVER_H