/** @file Quad_Encoder.h 
 *           sets up interface for using the MCU's hardware timer counting of pulses from a
 *           quadrature encoder to read and store encoder position values without using interrupts.
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

#define TMR_COUNT_MAX 65535             //the overflow value for the encoder timer, in this case for a 16 bit timer so, 65535.


/** @brief  Sets up the user specified timer into encoder mode and handles read values from it
 *  @details This is a class which uses the Hardware Timer API and some low level register setting to read a quadrature encoder on a motor
 *           and then do various useful tasks with that read value. This class has debugging methods which are documented in the .cpp file. 
 *           The primary methods of this class are @c enc_zero() , @c enc_read() , and its variants @c enc_read_pos() and @c enc_d_pos(). 
 *           In general the class works by counting pulses from the quadrature encoder on a brushed DC motor by leveraging the STM32 Generic
 *           hardware timer API with a bound based over/underflow detection scheme for determining direction.
 *           This approach gives maximum performance and minimal CPU load as all counting is done with hardware giving nanosecond
 *           latencies - based on your main clock speed that is (80 MHz in the case of the Nucleo L476RG).Methods which zero the encoder, 
 *           keep track of the absolute as well as the incremental postion with input from the read method are present as this is intended 
 *           to be used with a desktop CNC machine.
 */
class Quad_Encoder
{
    protected:
    // Class Member data here

    // Encoder Signal Channel Pins
    uint8_t _enc_sigpin_A;     ///< the pin that is receiving the A output signal of the encoder and is tied to the first timer channel
    uint8_t _enc_sigpin_B;     ///< the pin that is receiving the B output signal of the encoder and is tied to the second timer channel
    uint8_t _enc_chan_A;       ///< the timer channel number the encoder A signal pin is connected to.
    uint8_t _enc_chan_B;       ///< the timer channel number the encoder B signal pin is connected to.

    // Over/Underflow checking threshold 
    int32_t _bound;            ///< user specified overflow threshold parameter

    // Hardware timer that will be counting encoder pulses
    // NOTE: Please ensure that the hardware timer you have selected has at least 2 timer channels!
    TIM_TypeDef *_p_eTIM;      ///< pointer that contains user passed-in timer object (TIM1, TIM2, etc)
    HardwareTimer *EncTmr;     ///< Instance of Hardware Timer class used for an instance of the encoder class
    
    // Count Variables:
    uint16_t _lastcount;       ///< the previous count from the timer counter register
    uint16_t _count;           ///< the current count from the timer counter register
    
    // Position Variables  
    int32_t _abspos;           ///< position in the global frame 
    int32_t _last_abspos;      ///< the previous postion in the global frame


    
    bool _invert;              ///<  Software correction for encoder wiring

    public:
    // Constructor and method declarations here

    // Constructor for quadrature encoder class
    Quad_Encoder(uint8_t enc_sigpin_A, uint8_t enc_sigpin_B, uint8_t enc_chan_A, uint8_t enc_chan_B, TIM_TypeDef *p_eTIM, int32_t bound = 1000, bool invert = false);

    int32_t enc_read(void);                 // encoder read method to get current position in encoder ticks with direction and under/over flow checking
    int32_t get_delta(void);                // get the delta position value in ticks between the last two times enc_read was run
    float enc_read_angle_pos(void);         // relies on encoder read method but the ouput is the angular displacement output shaft of the motor
    float enc_read_pos(void);               // relies on encoder read method but the ouput is the linear displacement of the belt due to the motor's rotation
    // float enc_read_d_angle_pos (void);      // relies on the encoder read method but the output in the change in angluar displacement motor output since the last measurement
    // float enc_read_d_pos (void);            // relies on the encoder read method but the output in the change in linear displacement of the belt due to the motor's rotation since the last measurement
    void enc_zero(void);                    // resets the encoder value - will likely be called after the home command given by the user is executed to ensure the encoder is properly reset.
    
    // Debugging Methods
    uint16_t enc_test(void);       // returns the encoder counter register value to see if the timer is counting properly
    uint32_t get_overflow (void);  // returns what the overflow value for the counter register has been set to
    uint32_t get_prescale (void);  // returns what the prescale value for the timer counter in use has been set to

};

#endif //QUADENCODER_H