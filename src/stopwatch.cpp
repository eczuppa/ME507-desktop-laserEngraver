/** @file       stopwatch.cpp
 *  @brief      This file uses a class that implements the HardwareTimer Library API to 
 *              make a hardware timer behave like a stopwatch
 *  
 *  @author Ethan A Czuppa
 *  @date Nov 19 2020 Original File 
 * 
 */


#include "stopwatch.h"
#include "libraries&constants.h"

/** @brief      Creates a StopWatch constructor
 *  @details    Uses the HardwareTimer methods and user passed hardware timer (TIM1, TIM2, etc)
 *              in order to initialize and set up a constructor that will mimic a stopwatch
 * 
 *  @param      p_Stpwtch the pointer of type @c TIM_TypeDef which points to user-passed hardware timer (TIM1,TIM2, etc)
 *  @param      tmrpin the channel pin specifically assigned to the chosen timer
 */ 

//If the pin is set to the uint8_t pin name variable, use this function
StopWatch::StopWatch(TIM_TypeDef* p_Stpwtch, uint8_t tmrpin)
{
    StopWatch(p_Stpwtch, digitalPinToPinName(tmrpin));
}

//Otherwise, we want the true PinName of the pin we're using
StopWatch::StopWatch(TIM_TypeDef* p_Stpwtch, PinName tmrpin)
{
    // save inputs to class member data 
    _p_Stpwtch = p_Stpwtch;
    _tmrpin = tmrpin;
    // create a new HardwareTimer instance
    a_Tmr = new HardwareTimer(_p_Stpwtch);
    // Initialize the timer into OUTPUT_COMPARE with on output
    // and set it to count at 1 MHz
    a_Tmr -> pause();
    a_Tmr -> setMode(1,TIMER_DISABLED, _tmrpin);            //TIMER_DISABLED means that there is no pin that reflects the timer value. It is only an internal stopwatch timer.
    a_Tmr -> setPrescaleFactor(STOPWATCH_PRESCALE);

    // Serial << (a_Tmr -> getPrescaleFactor()) << endl;
    // Serial << (a_Tmr -> getOverflow()) << endl;
    // Serial << (a_Tmr -> getTimerClkFreq()) << endl;
    a_Tmr -> setCount(0);
    _last = 0;

}

// (pin)

/** @brief      StopWatch class method to overwrite the last timer value with current measured one
 * @returns     _now The current timer value
 */ 
uint16_t StopWatch::now_time(void)
{
    
    a_Tmr -> resume();
    _now = a_Tmr -> getCount();
    return _now;

}

/** @brief      Method to get the elapsed time since the last time point was measured
 *  @returns    _elpsd The elapsed time since the last measurement was taken
 */ 
uint16_t StopWatch::elapsed_time(void)
{
    a_Tmr -> resume();
    _elpsd = (a_Tmr ->getCount() - _now);
    return _elpsd;

}

/** @brief      Method to get the time between when this method was called last and now
 *  @returns    @c _lap measure of time since the last time that this funciton was called, in microseconds
 */ 
uint32_t StopWatch::lap(void)
{
    
    _now = a_Tmr -> getCount(TICK_FORMAT);
    _lap = _now - _last;

    // a_Tmr -> pause();
    // a_Tmr -> setCount(0);
    a_Tmr -> resume();

    _last = a_Tmr -> getCount(TICK_FORMAT);
    return _lap;

}



/** @brief      Restart the timer
 *  @details    This method will methods from the HardwareTimer class
 *              to clear the timer and set it back to zero
 */ 

void StopWatch::restart(void)
{
    a_Tmr -> pause();
    a_Tmr -> setCount(0, MICROSEC_FORMAT);
    _last = 0;
    a_Tmr -> resume();
}


/** @brief      Temporarily stop the timer
 *  @details    This method will use the HardwareTimer method "pause" to pause the timer
 * 
 */ 

void StopWatch::temp_stop(void)
{
    a_Tmr -> pause();
}
