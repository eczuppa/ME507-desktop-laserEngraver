/** @file       TB6612FNG_Driver.cpp
 *  @brief      This file implements the TB6612FNG class for the Toshiba dual H-bridge motor driver chip
 *  
 *  @author Ethan A Czuppa
 *  @date   Nov 6 2020 Original file
 */


#include "TB6612FNG_Driver.h"
#include <Arduino.h>

/** @brief   TB6612FNG Constructor
 *  @details Saves instances of class member data for all required input pins to run one H-bridge on the
 *           motor driver chip and brings the h-bridge into the enabled, LOW state were all outputs are 
 *           zeroed out to ensure nothing starts moving unexpectedly. Also sets up the hardware timer 
 *           to send out PWM pulses at 20kHz (so motors are quiet) and sets the initial duty_cylce to zero.
 *  @param   _standby_pin the digital pin that send the enable/disable singal to the dual H-bridge
 *  @param   _motor_dir_pin_1 the digital pin that maps to the IN1 motor direction input on 1 of the H-bridges
 *  @param   _motor_dir_pin_2 the digital pin that maps to the IN2 motor direction input on 1 of the H-bridges
 *  @param   _pwm_input_pin the pin from the microcontroller that is then configured to be attached to the specifed hardware timer
 *  @param   _pwm_tim_chan_num the channel number (1,2,3, or 4) of the timer specified that is attached to the pwm input pin 
 */

TB6612FNG::TB6612FNG(uint8_t stby_pin, uint8_t mot_pin_1, uint8_t mot_pin_2, uint8_t a_pwm_pin)
{
    // Save inputs to class member data
    _standby_pin = stby_pin;
    _motor_dir_pin_1 = mot_pin_1;
    _motor_dir_pin_2 = mot_pin_2;
    _pwm_input_pin = a_pwm_pin;
    

    // Initialize Standby and Motor Direction digital pins
    pinMode(_standby_pin, OUTPUT);
    pinMode(_motor_dir_pin_1, OUTPUT);
    pinMode(_motor_dir_pin_2, OUTPUT);

    // Initialize One of the H-bridge drivers on the chip
    digitalWrite(_standby_pin, HIGH);      // Enable H-bridge
    digitalWrite(_motor_dir_pin_1, LOW);   // Disable Motor Direction Pin 1
    digitalWrite(_motor_dir_pin_2, LOW);   // Disable Motor Direction Pin 2

    analogWrite(_pwm_input_pin, 0);

}

/** @brief Class method for setting the duty cycle of the attached motor via the H-bridge output
 *  @details takes in the duty cycle as a precentage (-100% to 100%) and sets the motor direction
 *           and speed.
 *  @param duty_cycle provided by a separate class or the user directly, a signed 8 bit integer 
 *                    that is the precentage representation of the desired duty cycle: -100% to 100%
 */

void TB6612FNG::setDutyCycle(float duty_cycle)
{ 
    float u_duty_cycle;
    // +/- Saturation Protection for SetPWM method of HardwareTimer class
    if (duty_cycle > 100)
    {
        duty_cycle = 100;
    }

    if (duty_cycle < -100)
    {
        duty_cycle = -100;
    }

    // sets motor direction based on input duty cycle and u_dutycycle for HardwareTimer API (expects 0 to 100%)
    
    // Spin Clockwise
    if (duty_cycle < 0)
    {
        digitalWrite(_motor_dir_pin_1, HIGH);
        digitalWrite(_motor_dir_pin_2, LOW);
        u_duty_cycle = duty_cycle* -1;   
    }
    
    // Spin Counter Clockwise
    else if (duty_cycle > 0)
    {
        digitalWrite(_motor_dir_pin_1, LOW);   
        digitalWrite(_motor_dir_pin_2, HIGH);
        u_duty_cycle = duty_cycle; 
    }
    
    // Something went horribly wrong, just stop what you are doing
    else
    {
        digitalWrite(_motor_dir_pin_1, LOW);   
        digitalWrite(_motor_dir_pin_2, LOW);
        u_duty_cycle = 0; 
    }
    
    // Updates duty cycle to input or saturated input without 
    // reinitializing the PWM mode of MotorTmr
    
    u_duty_cycle = (float)u_duty_cycle*255/100;

    analogWrite(_pwm_input_pin,(uint8_t)u_duty_cycle);
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

/** @brief method to brake motors
 *  @details based on the truth table for the TB6612FNG driver chip and sparkfun's arduino library for this IC
 */
void TB6612FNG::brake(void)
{
    // Places chip in short brake mode instead of coasting to a stop
    digitalWrite(_motor_dir_pin_1, HIGH);
    digitalWrite(_motor_dir_pin_2, HIGH);
    setDutyCycle(0);
}