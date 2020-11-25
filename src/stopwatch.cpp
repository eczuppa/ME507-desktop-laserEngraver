
#include "stopwatch.h"

// StopWatch Constructor
StopWatch::StopWatch(TIM_TypeDef* p_Stpwtch, uint8_t tmrpin)
{
    // save inputs to class member data 
    _p_Stpwtch = p_Stpwtch;
    _tmrpin = tmrpin;
    // create a new HardwareTimer instance
    a_Tmr = new HardwareTimer(_p_Stpwtch);
    // Initialize the timer into OUTPUT_COMPARE with on output
    // and set it to count at 1 MHz
    a_Tmr -> pause();
    a_Tmr -> setMode(1,TIMER_OUTPUT_COMPARE, _tmrpin);
    a_Tmr -> setOverflow(1000000, MICROSEC_FORMAT);
    a_Tmr -> setCount(0, MICROSEC_FORMAT);
    _last = 0;

}

// StopWatch class method to overwrite the last timer value with current measured one 
uint16_t StopWatch::now_time(void)
{
    
    a_Tmr -> resume();
    _now = a_Tmr -> getCount();
    return _now;

}

// method to get the elapsed time since the last time point was measured
uint16_t StopWatch::elapsed_time(void)
{
    a_Tmr -> resume();
    _elpsd = (a_Tmr ->getCount() - _now);
    return _elpsd;

}

uint32_t StopWatch::lap(void)
{
    a_Tmr -> resume();
    _now = a_Tmr -> getCount();
    _lap = _now - _last;
    _last = _now;
    return _lap;

}

void StopWatch::restart(void)
{
    a_Tmr -> pause();
    a_Tmr -> setCount(0, MICROSEC_FORMAT);
    _last = 0;
    a_Tmr -> resume();
}

void StopWatch::temp_stop(void)
{
    a_Tmr -> pause();
}
