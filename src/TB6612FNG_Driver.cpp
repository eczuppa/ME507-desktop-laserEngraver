/** @file TB6612FNG_Driver.cpp
 *          This file implements the TB6612FNG class for the Toshiba dual H-bridge motor driver chip
 *  
 *  @author Ethan A Czuppa
 * 
 *  @date 6 Nov 6:32PM  2020 Created file for class implementation of TB6612FNG motor driver IC with doxygen comments
 *  @date 7 Nov 12:23AM 2020 finished first version of motor driver, needs to be updated to accept more refrences 
 *                           since many inputs are predefined objects (pin names, other classes, etc. ) 
 *                           Also I need to figure out how to pass the instantiation of the hardware timer class to
 *                           through this class (e.g. TIM1, TIM2, TIM3, etc) so that the timer can be changed as needed
 *                           instead of being set in the header file as it is currently. 
 * 
 * 
 * 
 */

#include "TB6612FNG_Driver.h"
#include <Arduino.h>
#include <string.h>

/** @brief   TB6612FNG Constructor
 *  @details Saves instances of class memeber data for all required input pins to run one H-bridge on the
 *           motor driver chip and brings the h-bridge into the enabled, LOW state were all outputs are 
 *           zeroed out to ensure nothing starts moving unexpectedly. Also sets up the hardware timer 
 *           to send out PWM pulses at 20kHz (so motors are quiet) and sets the intial duty_cylce to zero.
 *  @param   _standby_pin the digital pin that send the enable/disable singal to the dual H-bridge
 *  @param   _motor_dir_pin_1 the digital pin that maps to the IN1 motor direction input on 1 of the H-bridges
 *  @param   _motor_dir_pin_2 the digital pin that maps to the IN2 motor direction input on 1 of the H-bridges
 *  @param   _pwm_input_pin the pin from the microcontroller that is then configured to be attached to the specifed hardware timer
 *  @param   _pwm_tim_chan_num the channel number (1,2,3, or 4) of the timer specified that is attached to the pwm input pin 
 */

TB6612FNG::TB6612FNG(uint8_t stby_pin, uint8_t mot_pin_1, uint8_t mot_pin_2, uint8_t a_pwm_pin, uint8_t a_tim_chan_num, TIM_TypeDef * _p_timer)
{
    // Save inputs to class member data
    _standby_pin = stby_pin;
    _motor_dir_pin_1 = mot_pin_1;
    _motor_dir_pin_2 = mot_pin_2;
    _pwm_input_pin = a_pwm_pin;
    _pwm_tim_chan_num = a_tim_chan_num;

    // Setup PWM timer
    MotorTmr = new HardwareTimer(_p_timer);

    // Initalize Standby and Motor Direction digital pins
    pinMode(_standby_pin, OUTPUT);
    pinMode(_motor_dir_pin_1, OUTPUT);
    pinMode(_motor_dir_pin_2, OUTPUT);

    // Intialize One of the H-bridge drivers on the chip
    digitalWrite(_standby_pin, HIGH);      // Enable H-bridge
    digitalWrite(_motor_dir_pin_1, LOW);   // Disable Motor Direction Pin 1
    digitalWrite(_motor_dir_pin_2, LOW);   // Disable Motor Direction Pin 2

    // Put Motor Tmr into PWM mode
    MotorTmr -> setPWM(_pwm_tim_chan_num, _pwm_input_pin, 20000, 0);
}

/** @brief Class method for setting the duty cycle of the attached motor via the H-bridge output
 *  @details takes in the duty cycle as a precentage (-100% to 100%) and sets the motor direction
 *           and speed.
 *  @param duty_cycle provided by a separate class or the user directly, a signed 8 bit integer 
 *                    that is the precentage representation of the desired duty cycle: -100% to 100%
 */

void TB6612FNG::setDutyCycle(int8_t duty_cycle)
{ 
    // +/- Saturation Protection for SetPWM method of HardwareTimer class
    if (duty_cycle > 100)
    {
        duty_cycle = 100;
    }

    if (duty_cycle < -100)
    {
        duty_cycle = -100;
    }
    
    // sets motor direction based on input dutycyle
    
    // Spin Clockwise
    if (duty_cycle < 0)
    {
        digitalWrite(_motor_dir_pin_1, LOW);   
        digitalWrite(_motor_dir_pin_2, LOW);
        digitalWrite(_motor_dir_pin_1, HIGH);   
    }
    
    // Spin Counter Clockwise
    else if (duty_cycle > 0)
    {
        digitalWrite(_motor_dir_pin_1, LOW);   
        digitalWrite(_motor_dir_pin_2, LOW);
        digitalWrite(_motor_dir_pin_2, HIGH); 
    }
    
    // Something went horribly wrong, just stop what you are doing
    else
    {
        digitalWrite(_motor_dir_pin_1, LOW);   
        digitalWrite(_motor_dir_pin_2, LOW); 
    }
    
    // Updates duty cycle to input or saturated input without 
    // reinitializing the PWM mode of MotorTmr
    MotorTmr -> setCaptureCompare(_pwm_tim_chan_num, duty_cycle);
}

/** @brief software E-Stop for dual H-bridge chip
 *  @details drives the ouput of the standby pin LOW, which disables the dual H-bridge IC entirely
 */ 

void TB6612FNG::disable(void)
{
    digitalWrite(_standby_pin, LOW);
}

/** @brief software reset for dual H-bridge chip after being disabled
 *  @details drives the output of the standby pin HIGH, which enables the dual H-bridge IC
 */

void TB6612FNG::enable(void)
{
    digitalWrite(_standby_pin, HIGH);
}