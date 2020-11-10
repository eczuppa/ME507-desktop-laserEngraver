
#include "Quad_Encoder.h"

Quad_Encoder::Quad_Encoder(uint8_t enc_sigpin_A, uint8_t enc_sigpin_B, TIM_TypeDef *p_eTIM, uint32_t bound = 1000)
{
    // Save instance specific inputs into class member data
    _enc_sigpin_A = enc_sigpin_A;
    _enc_sigpin_B = enc_sigpin_B;
    _p_eTIM = p_eTIM;
    _bound = bound;

    // Instantiate and intialize hardware timer for counting encoder pulses
    EncTmr = new HardwareTimer(_p_eTIM);




}

uint64_t Quad_Encoder::enc_read(void)
{

}

uint64_t Quad_Encoder::get_abspos(void)
{

}

uint64_t Quad_Encoder::get_incpos(void)
{

}

void Quad_Encoder::enc_zero(void)
{
    
}