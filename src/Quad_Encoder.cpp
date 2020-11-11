
// #include "Quad_Encoder.h"

// // Class constants
// const uint32_t TMR_COUNT_MAX = 65535;
// const uint32_t TMR_PRESCL = 0;

// Quad_Encoder::Quad_Encoder(uint8_t enc_sigpin_A, uint8_t enc_sigpin_B, TIM_TypeDef *p_eTIM, uint32_t bound = 1000, bool invert = false)
// {
//     // Save instance specific inputs into class member data
//     _enc_sigpin_A = enc_sigpin_A;
//     _enc_sigpin_B = enc_sigpin_B;
//     _p_eTIM = p_eTIM;
//     _bound = bound;
//     _abspos = 0;
//     _incpos = 0;
//     _invert = invert;

    

//     // Instantiate and intialize hardware timer for counting encoder pulses
//     EncTmr = new HardwareTimer(_p_eTIM);
//     EncTmr -> setPrescaleFactor(TMR_PRESCL);
//     EncTmr -> setOverflow(TMR_COUNT_MAX, TICK_FORMAT);
//     EncTmr -> setMode(1, TIMER_INPUT_CAPTURE_RISING, _enc_sigpin_A);
//     EncTmr -> setMode(2, TIMER_INPUT_CAPTURE_RISING, _enc_sigpin_B);
//     EncTmr -> resume();
//     // Initialize last count with current counter value in selected timer counter register
//     _lastcount = EncTmr -> getCount();
//     // if this doesn't work use hal_encoder_tim functions directly - HAL_TIM_ECODER_DEINIT, HAL_TIM_ENCODER_INIT, HAL_TIM_ENCODER_START (no interrupts)

// }

// uint64_t Quad_Encoder::enc_read(void)
// {
//     uint32_t delta;
//     _count = EncTmr -> getCount();
    
//     // positive overflow condition for counting up from 0 to 65535 
//     if ((_lastcount > TMR_COUNT_MAX - _bound) && (_count < _bound))
//     {
//         delta = (TMR_COUNT_MAX - _lastcount + _count);
//     }
//     // negative overflow conidtion for counting down from 65535 to 0
//     else if ((_count > TMR_COUNT_MAX - _bound) && (_lastcount < _bound))
//     {
//         delta = -1*(TMR_COUNT_MAX - _count + _lastcount);
//     }
//     // no overflow, delta is simply the difference between the current and last counts
//     else 
//     {
//         delta = _count - _lastcount;
//     }

//     // Handle encoder inversion if wiring does not match expected setup
//     if (_invert)
//     {
//         delta *= -1;
//     }
//     _abspos += delta;  // increment the absolute position variable by the amount delta from current encoder read
//     return _abspos;
// }


// uint64_t Quad_Encoder::get_incpos(void)
// {
//     // talk to ridgely about how this will ensure we don't crash... not sure we will need to do incremental positioning at all...
//     // probably implemented in the CNC safety class as a method


// }

// void Quad_Encoder::enc_zero(void)
// {
//     // Reset the value of the encoder to zero
//     _abspos = 0;
// }