#include "stopwatch.h"
// This class is an implementaiton of example code from 


// StopWatch Constructor
StopWatch::StopWatch(TIM_TypeDef* p_Stpwtch, uint8_t tmrpin)
{
    _p_Stpwtch = p_Stpwtch;
    _tmrpin = tmrpin;
    a_Tmr = new HardwareTimer(_p_Stpwtch);
    a_Tmr -> pause();
    a_Tmr -> setMode(1,TIMER_OUTPUT_COMPARE, _tmrpin);
    a_Tmr -> setOverflow(1000000, MICROSEC_FORMAT);
    a_Tmr -> setCount(0, MICROSEC_FORMAT);

}

// StopWatch class method to overwrite the last timer value with current measured one 
uint32_t StopWatch::now_time(void)
{
    
    a_Tmr -> resume();
    _now = a_Tmr -> getCount();
    return _now;

}

// method to get the elapsed time since the last time point was measured
uint32_t StopWatch::elapsed_time(void)
{
    a_Tmr -> resume();
    _elpsd = (a_Tmr ->getCount() - _now);
    return _elpsd;

}

void StopWatch::restart(void)
{
    a_Tmr -> pause();
    a_Tmr -> setCount(0, MICROSEC_FORMAT);
    a_Tmr -> resume();
}