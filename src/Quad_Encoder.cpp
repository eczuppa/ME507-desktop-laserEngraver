/** @file Quad_Encoder.cpp 
 *           class implementation that uses hardware timers to count pulses from a quadrature rotary encoder WITHOUT INTERRUPTS
 *  
 *  @author Ethan A Czuppa
 *  @author JR Ridgely - wrote the C++ class that I used the constructor from in order to get my timer to work with the encoder
 *  @author Josh Anderson  - taught me how to write an encoder reading method in python that I am using in C++ because I am still new to this whole type
 *  @author Alexandre Bourdiol - wrote the HardwareTimer Library that this class depends on
 * 
 *  @date 9 Nov 2020 wrote Quad_Encoder module - mostly pseudocode with some doxygen comments
 *  @date 11 Nov 2020 finished Quad_Encoder class implementation and prepped for testing
 *  @date 15 Nov 2020 Added several debugging methods to try to figure out what was wrong with the timer setup
 *  @date 16 Nov 2020 Using JR Ridgely's STM32Encoder's constructor got timer to count quadrature encoder pulses. Added other needed doxygen stuff
 *  @date 16 Nov 2020 (later that day) added methods that output the belt's displacement and velocity based on enc_read(). Added more doxygen comments
 */

#include "Quad_Encoder.h"

// ================================================
// ||                                            ||
// ||            Class  CONSTANTS                ||
// ||                                            ||
// ================================================
const uint16_t TMR_COUNT_MAX = 65535;
const int32_t TMR_PRESCL = 0;        // Prescaler must be set to zero for Quadrature decoding
// Physical Characteristics of Timing Belt Pulley - set to match your own.
const float MOMENT_ARM = 5.95; // radius of timing pulley on motor in [mm]
    
// The number of radians in 1 revolution of the motor output shaft divided
// by the number of ticks accrued in the specified timer counter register. 
// this value was empirically measured by polling the encoder with the
// enc_read() method while slowly rotating the motor output shaft through 1 rotation
// several times to ensure that the this conversion is correct.
const float TICK_TO_RAD = 0.022520; // radians per encoder tick




// ================================================
// ||                                            ||
// ||            Primary Methods                 ||
// ||                                            ||
// ================================================

/** @brief  Constructor for Quad Encoder Class 
 *  @details takes the two pins from the encoder that will tie to specific pins for timer functionality
 *           in encoder mode as well as the desired timer to be used (TIM1, TIM2, etc.) and has default values for 
 *           the bound and invert parameters. Puts the timer into a specific encoder mode - the count register value is 
 *           incremented on the rising and falling edges of each encoder singal input. And saves input parameters
 *           and other useful, state-determining values into class member data
 *  @param   enc_sigpin_A is the 8-bit unsigned integer representation of the timer channel (pin) the first encoder output signal is tied to (e.g PC6 or PC7 for TIM8)
 *  @param   enc_sigpin_B is the 8-bit unsigned integer representation of the timer channel (pin) the second encoder output signal is tied to (e.g PC6 or PC7 for TIM8)
 *  @param   enc_chan_A  
 *  @param   enc_chan_B
 *  @param   p_eTIM the pointer of type @c TIM_TypeDef which matches the type of the predefined TIMx objects (e.g. TIM1, ..., TIM16) 
 *  @param   bound the value which determines the area where the enc_read method begins checking for an under/overflow condition
 *  @param   invert a boolean to compensate for hardware issues - if you switch the encoder signal outputs on the timer channel pins then setting
 *                 invert to true allows you to read the correct encoder values without switching the motor leads, which can sometimes be problematic
 * 
 */

Quad_Encoder::Quad_Encoder(uint8_t enc_sigpin_A, uint8_t enc_sigpin_B, uint8_t enc_chan_A, uint8_t enc_chan_B, TIM_TypeDef *p_eTIM, int32_t bound, bool invert)
{
    // Save instance specific inputs into class member data
    _enc_sigpin_A = enc_sigpin_A;
    _enc_sigpin_B = enc_sigpin_B;
    _enc_chan_A = enc_chan_A;
    _enc_chan_B = enc_chan_B;
    _p_eTIM = p_eTIM;
    _bound = bound;
    _abspos = 0;
    _last_abspos = 0;
    _invert = invert;

    
    // Instantiate and initialize hardware timer for counting encoder pulses based on:
    //           JR Ridgely's implementation of the Hardware Timer API specifically
    //           for a Quadratrue Rotary encoders in his STM32Encoder class. 
    //           Available from - https://github.com/spluttflob/ME507-Support/blob/master/src/encoder_counter.cpp
    //           Read the Documentation here -  https://spluttflob.github.io/ME507-Support/classSTM32Encoder.html
    
    // new hardware timer class instance using user-inputted TIMx.
    EncTmr = new HardwareTimer(_p_eTIM);
    
    // Pause the timer to keep it from counting while we are trying to set it up.
    EncTmr -> pause();

    // Place both of the encoder signal pins connected to the timer in a compatible mode to 
    // the timer being in encoder mode. This may or may not work for boards besides the STM32L476RG Nucleo 
    EncTmr -> setMode(_enc_chan_A, (TimerModes_t)((1UL << 8) | (1UL << 0)), _enc_sigpin_A);  // Puts the timer channel 1 into input capture falling and rising edge detection mode
    EncTmr -> setMode(_enc_chan_B, (TimerModes_t)((1UL << 8) | (1UL << 0)), _enc_sigpin_B);  // Puts the timer channel 2 into input capture falling and rising edge detection mode
    
    // Reset the value of the timer count register to 0 so the coutner doesn't start with something weird in it
    EncTmr -> setCount(0);
    
    // Low level mode setting of the specified timer's control register
    // For the encoder mode we are interested in. 
    _p_eTIM -> SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; // both of the timer channels used enabled to increment the count in the timer counter register
    _p_eTIM -> CR1 |= TIM_CR1_CEN;  // start the actual timer
    
    // Attempting to program the bare metal so to speak - put the given timer in encoder mode based on: 
    //       Edwin Farchild's Interface a Rotary Encoder the Right Way.
    //       Available from - https://www.edwinfairchild.com/2019/04/interface-rotary-encoder-right-way.html
    //       Did not work, possible that the encoder signal pins were not set up correctly
    // _p_eTIM -> PSC = 0x0000;
    // _p_eTIM -> ARR = 0xFFFF;
    // _p_eTIM -> CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC1S_1);
    // _p_eTIM -> CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);

    // Attempting to use the Hardware Timer Library API and HAL_TIM functions to put the given timer in encoder mode based on:
    //         Hardware Timer API Wiki
    //         Available from - https://github.com/stm32duino/wiki/wiki/HardwareTimer-library#API
    //         Did not work, possible encoder signals were not setup correctly 
    // EncTmr -> setPreloadEnable(false);
    // EncTmr -> setOverflow(TMR_COUNT_MAX, TICK_FORMAT);
    //EncTmr -> setMode(1, TIMER_INPUT_CAPTURE_RISING, _enc_sigpin_A);
    //EncTmr -> setMode(2, TIMER_INPUT_CAPTURE_RISING, _enc_sigpin_B);
    // HAL_TIM_Encoder_Init(EncTmr->getHandle(),(TIM_Encoder_InitTypeDef*)TIM_ENCODERMODE_TI12);
    // HAL_TIM_Encoder_Start(EncTmr->getHandle(),TIM_CHANNEL_ALL);
    // Initialize last count with current counter value in selected timer counter register
    //EncTmr -> setCount(0);
    
    // Initialize last count for the encoder read method to the current count, should be zero.
    _lastcount = EncTmr -> getCount();

}


/** @brief   handles the changing value in the timer count register and returns the total displacement in encoder ticks
 *  @details Keeps track of encoder position in ticks in the returned parameter by accumulating the change in timer 
 *           count register values using the HardwareTimer Library's @c getCount() method as well as positive and 
 *           negative overflow handling. 
 *  @param   delta the difference between the last timer register count value and the current one - checked for overflows.
 *  @param   count the method specific variable that holds the current timer register count value.   
 *  @param   TMR_COUNT_MAX the overflow value for the timer, in this case for a 16 bit timer so, 65535.
 *  @param   _abspos is the class member variable that holds the absolute position measured by the encoder 
 *                   it is an int32_t so it should be quite difficult to overflow unless your motor is spinning SUPER fast
 *  @param   _last_abspos a snap shot of _abspos before being incremented by the appropriate delta - used for instantaneous velocity calculations
 * 
 */
int32_t Quad_Encoder::enc_read(void)
{
    // This code is based on the python implementation of a read method for an encoder
    // I wrote based on the patient instructions from my coding wizard ME 405 lab partner Josh Anderson. 


    int32_t delta;    // The difference between the last encoder count and the current one 
    uint16_t count;    // The current encoder count, reinitialized every time this method is called
    
    // Get the current count
    count = EncTmr -> getCount();
    
    // positive overflow condition for counting up from 0 to 65535 
    if ((_lastcount > TMR_COUNT_MAX - _bound) && (count < _bound))
    {
        delta = (TMR_COUNT_MAX - _lastcount + count);
    }
    // negative overflow condition for counting down from 65535 to 0
    else if ((count > TMR_COUNT_MAX - _bound) && (_lastcount < _bound))
    {
        delta = -1*(TMR_COUNT_MAX - count + _lastcount);
    }
    // no overflow, delta is simply the difference between the current and last counts
    else 
    {
        delta = count - _lastcount;
    }

    // delta = count - _lastcount;

    // Handle encoder inversion if wiring does not match expected setup
    if (_invert)
    {
        delta *= -1;
    }
    
    _last_abspos = _abspos; // take a snap shot abspos before we add the new delta to it
    _abspos += delta;  // increment the absolute position variable by the amount delta from current encoder read
    _lastcount = count; // recycle the last lastcount to the current count used in this loop so the no data is lost when the next count is got 
    return _abspos;
}


/** @brief   converts the raw value in ticks from @c enc_read() to belt displacement in mm
 *  @details The radius of timing belt pulley and the conversion from ticks to radians (motor-specific)
 *           are used to convert the accrued position returned from @c enc_read to a value for the
 *           linear movement of the belt driven by the timing pulley in millimeters. 
 *  @param   MOMENT_ARM the radius of the drive mechanism translating rotational motion to linear motion
 *  @param   TICK_TO_RAD the number of radians per encoder tick, which is empirically determined by measuring
 *                       the number of ticks accumulated in the timer count register when moving the 
 *                       OUTPUT shaft of the motor through 1 rotation. Do this several times, record the result
 *                       and average it. Then compute by dividing 2*Pi by your result (ticks/revoltion).
 * @param    current_tick method specific storage of @c enc_read() return value 
 * 
 */
float Quad_Encoder::enc_read_pos(void)
{
    int32_t current_tick = enc_read();  // holds the encoder pos value read by the encoder
    return (MOMENT_ARM*current_tick*TICK_TO_RAD);
}

float Quad_Encoder::enc_d_pos(void)
{
    return (MOMENT_ARM*(_abspos - _last_abspos)*TICK_TO_RAD);
}


/** @brief   resets _abspos, the accumulated encoder ticks variable, to 0
 *  @details applications include prepping the encoder for use, trying to do
 *           things in an incremental positioning frame vs the global frame.
 *           Not to be confused with the Hardware Timer API's @c setCount()
 *           which changes the value of the timer counter register. 
 * 
 */
void Quad_Encoder::enc_zero(void)
{
    // Reset the value of the encoder to zero
    _abspos = 0;
}


// ================================================
// ||                                            ||
// ||            Debugging Methods               ||
// ||                                            ||
// ================================================

// If your encoder timer is behaving strangely these methods are at your disposal 
// to try and figure out what is going on.

/** @brief quick way to check if encoder is counting
 *  @details poll the timer with this method in a loop and print out the result
*            to see if the timer is counting the way you expect it to be   
*/
uint16_t Quad_Encoder::enc_test(void)
{
    return (EncTmr -> getCount());
}

/** @brief quick way to check if timer overflow value for counter register is what you expect
 *  @details print out the result somewhere convenient in your code, 
 *           but beware of having multiple things try to print simultaneously
*/
uint32_t Quad_Encoder::get_overflow (void)
{
    return (EncTmr ->getOverflow());
}

/** @brief quick way to check if timer prescaler is what you expect
 *  @details print out the result somewhere convenient in your code, 
 *           but beware of having multiple things try to print simultaneously
 */
uint32_t Quad_Encoder::get_prescale (void)
{
    return (EncTmr -> getPrescaleFactor());
}