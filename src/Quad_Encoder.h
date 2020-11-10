/** @file    Quad_Encoder.h sets up interface for using the MCU's hardware timer counting of pulses from a
 *           quadrature encoder to read and store encoder position values.
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
 * 
 */

#include <stdint.h>
#include <HardwareTimer.h>

class Quad_Encoder
{
    protected:
    // Class Member data here

    // Encoder Signal Channel Pins
    uint8_t _enc_sigpin_A;
    uint8_t _enc_sigpin_B;

    // Over/Underflow checking threshold 
    uint32_t _bound;

    // Hardware timer that will be counting encoder pulses
    // NOTE: Please ensure that the hardware timer you have selected has at least 2 timer channels!
    TIM_TypeDef *_p_eTIM;       // pointer that contains user passed-in timer object (TIM1, TIM2, etc)
    HardwareTimer *EncTmr;      // Instance of Hardware Timer class used for an instance of the encoder class

    public:
    // Constructor and method declarations here

    // Constructor for quadrature encoder class
    Quad_Encoder(uint8_t enc_sigpin_A, uint8_t enc_sigpin_B, TIM_TypeDef *p_eTIM, uint32_t bound = 1000);

    uint64_t enc_read(void);    // encoder read method to get current position in encoder ticks with direction and under/over flow checking
    uint64_t get_abspos(void);  // gets absolute position from encoder for working from CNC machine home
    uint64_t get_incpos(void);  // gets incremental position if wokring from a refrence offset from CNC machine home
    void enc_zero(void);        // resets the encoder value - will likely be called after the home command given by the user is executed to ensure the encoder is properly reset.


};