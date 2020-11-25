/** @file    Quad_Encoder.h sets up interface for using the MCU's hardware timer counting of pulses from a
 *           quadrature encoder to read and store encoder position values without using interrupts.
 *  @details counts pulses from the qudarature encoder on a brushed DC motor by leveraging the STM32 Generic
 *           hardware timer API with a bound based over/underflow detection scheme for determining direction.
 *           maximum performance and minimal CPU load as all counting is done with hardware giving nanosecond
 *           latencies - based on your main clock speed that is (80 MHz in the case of the Nucleo L476RG). 
 *           Methods which zero the encoder, keep track of the absolute as well as the incremental postion 
 *           with input from the read method are present as this is intended to be used with a desktop CNC machine.
 * 
 * @author   Ethan A Czuppa
 * 
 * @date     9 Nov 6:29PM 2020 Created file in repo and added doxygen comments
 * @date     9 Nov 6:47PM 2020 finished header file constructor and method declarations
 * @date     
 * @date     16 Nov 2020 added new variables to support methods that return displacement and velocity based on encoder reading
 * 
 */



#ifndef QUADENCODER_H
#define QUADENCODER_H

#include "libraries&constants.h"

/** @brief  Write what the class does
 * 
 * @details More descriptive description of the class
 * 
 */
class Quad_Encoder
{
    protected:
    // Class Member data here

    // Encoder Signal Channel Pins
    uint8_t _enc_sigpin_A;
    uint8_t _enc_sigpin_B;
    uint8_t _enc_chan_A;
    uint8_t _enc_chan_B;

    // Over/Underflow checking threshold 
    int32_t _bound;

    // Hardware timer that will be counting encoder pulses
    // NOTE: Please ensure that the hardware timer you have selected has at least 2 timer channels!
    TIM_TypeDef *_p_eTIM;       // pointer that contains user passed-in timer object (TIM1, TIM2, etc)
    HardwareTimer *EncTmr;      // Instance of Hardware Timer class used for an instance of the encoder class
    // StopWatch * &_p_clock;
    // TIM_TypeDef *_p_vTIM;       // pointer that contains user passed-in timer object (TIM1, TIM2, etc)
    // HardwareTimer *VelTmr;      // Instance of Hardware Timer class used for an instance of the encoder class

    // Count Variables:
    uint16_t _lastcount;     // the previous count from the timer counter register
    uint16_t _count;         // the current count from the timer counter register
    // Position Variables
    int32_t _abspos;       // position in the global frame 
    int32_t _last_abspos;  // the previous postion in the gobal frame

    // Velocity variables
    //int32_t _absvel;
    //int32_t _lasttime;
    //int32_t _currtime;
    //int32_t _delta_time;
    

    // Software correction for encoder wiring
    bool _invert;

    public:
    // Constructor and method declarations here

    // Constructor for quadrature encoder class
    Quad_Encoder(uint8_t enc_sigpin_A, uint8_t enc_sigpin_B, uint8_t enc_chan_A, uint8_t enc_chan_B, TIM_TypeDef *p_eTIM, int32_t bound = 1000, bool invert = false);

    int32_t enc_read(void);        // encoder read method to get current position in encoder ticks with direction and under/over flow checking
    float enc_read_pos(void);      // relies on encoder read method but the oput is the linear displacement of the belt due to the motor's rotation
    float enc_d_pos (void);
    //float enc_read_vel(void);      // relies on encoder read position method but the output is the tangential velocity in the direction of the belt at the motor's drive pulley
    void enc_zero(void);           // resets the encoder value - will likely be called after the home command given by the user is executed to ensure the encoder is properly reset.

    // Get -er methods
    int32_t get_enc_dt(void);      // returns the delta time in sec associated with the calculate instanteous velocity
    

    // Debugging Methods
    uint16_t enc_test(void);       // returns the encoder counter register value to see if the timer is counting properly
    uint32_t get_overflow (void);  // returns what the overflow value for the counter register has been set to
    uint32_t get_prescale (void);  // returns what the prescale value for the timer counter in use has been set to



};

#endif //QUADENCODER_H