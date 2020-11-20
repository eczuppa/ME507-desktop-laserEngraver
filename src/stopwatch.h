
/** @file stopwatch.h is the header for a timer class based on the HardwareTimer library that behaves like an actual stopwatch
 *  @details Uses the hardware timer library API to create a timer that just counts but does not trigger any output events. 
 *           the counts and overflow are set to be in @c MICROSECOND_FORMAT to ensure that the sufficient resolution of timing 
 *           is achieved for whatever the user is trying to time.
 *           
 *           See the Hardware Timer library API and Wiki for more information on changing the clock parameters
 *           Here: https://github.com/stm32duino/wiki/wiki/HardwareTimer-library#API 
 * 
 *  @author Ethan A Czuppa
 *  
 *  @date 19 Nov 2020 Original File.  
 * 
 */


#ifndef STPWATCH_H
#define STPWATCH_H
#include <HardwareTimer.h>

class StopWatch
{
    protected:
    
    // Note: make sure this timer is not being used by another class/function!
    //Timer initialization arugments
    TIM_TypeDef* _p_Stpwtch;   // pointer to user-passed hardware timer (TIM1,TIM2, etc) 
    HardwareTimer* a_Tmr;      // pointer to HardwareTimer class instance
    uint8_t _tmrpin;           // the channel pin specifically assigned to the chosen timer
    
    // Class member data
    uint16_t _now;            // the current time in microseconds - since the clock was restarted       
    uint16_t _elpsd;          // the time elapsed between the last and current measurements with now_time();

    public:
    StopWatch(TIM_TypeDef* p_Stpwtch,uint8_t tmrpin);
    
    uint32_t now_time(void); // all thigs now measured from current time and returns that value
    uint32_t elapsed_time(void); // returns time elapsed in microseconds
    void restart(void); // sets the count back to 0;

};
#endif //STPWATCH_H