/** @file TB6612FNG_Driver.h
 *            This file sets up the interface for the class to drive brushed DC motors using Toshiba's TB6612FNG
 *            motor driver chip. 
 *  
 *  @author   Ethan A. Czuppa
 *  
 *  @date     6 Nov 2020 Created Project and this file in parallel with doxygen comments for future documentation
 * 
 * 
 */


#include <stdint.h>

#include <HardwareTimer.h>
#include <pins_arduino.h>

class TB6612FNG 
{
    protected:
    // Create Variables to save as class member data that are only accessible by this class
    
    // Motor Direction Pins (Enables for which direciton the motor will rotate in)
    uint8_t _motor_dir_pin_1;
    uint8_t _motor_dir_pin_2;

    // Motor Driver PWM input pin (Sets speed at which motor is driven)
    uint8_t _pwm_input_pin;

    // Motor Driver Standby Pin (Enable Pin for H-Bridge)
    uint8_t _standby_pin; 

    // Timer Number and Timer Channel Number 
    uint8_t _pwm_tim_chan_num;

    // Setup PWM timer
    // Timer Number and Timer Channel Number 
    uint8_t _pwm_tim_chan_num;
    TIM_TypeDef * _p_timer;
    HardwareTimer *MotorTmr;

    public:
    // Constructor and class methods here

    // Constructor
    TB6612FNG::TB6612FNG(uint8_t stby_pin, uint8_t mot_pin_1, uint8_t mot_pin_2, uint8_t a_pwm_pin, uint8_t a_tim_chan_num, TIM_TypeDef * _p_timer);

    // Class Methods
    void setDutyCycle(int8_t duty_cycle);    // Sets motor speed and direction based on sign of duty cycle input           
    // void brake(void);                        // Makes motor hold in place?? (should be accomplished by PID position control) 
    void disable(void);                      // Software Emergency stop
    void enable(void);                       // Software reset of motor driver chip


};