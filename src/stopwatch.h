
/** @file stopwatch.h 
 *          makes the specified onboard timer count in the up direction and adds other functionality so the
 *          user selected timer behaves as a typical stop watch would. The HardwareTimer Library API was used
 *          to do this, See the Hardware Timer library API and Wiki for more information on changing the clock parameters
 *          Here: https://github.com/stm32duino/wiki/wiki/HardwareTimer-library#API 
 * 
 *  @author Ethan A Czuppa
 *  
 *  @date 19 Nov 2020 Original File.  
 * 
 */


#ifndef STPWATCH_H
#define STPWATCH_H
#include <HardwareTimer.h>

/** @brief   header for a timer class based on the HardwareTimer library that behaves like an actual stopwatch
 *  @details Uses the hardware timer library API to create a timer that just counts but does not trigger any output events. 
 *           the counts and overflow are set to be in @c MICROSECOND_FORMAT to ensure that the sufficient resolution of timing 
 *           is achieved for whatever the user is trying to time.
 * 
 */

class StopWatch
{
    protected:
    
    // Note: make sure this timer is not being used by another class/function!
    //Timer initialization arugments
    TIM_TypeDef* _p_Stpwtch;   ///< pointer to user-passed hardware timer (TIM1,TIM2, etc) 
    HardwareTimer* a_Tmr;      ///< pointer to HardwareTimer class instance
    uint8_t _tmrpin;           ///< the channel pin specifically assigned to the chosen timer
    
    // Class member data
    uint16_t _now;            ///< the current time in microseconds - since the clock was restarted       
    uint16_t _last;           ///< the last time a measurement was taken
    uint16_t _lap;            ///< the difference between now and last - used when measuring a parameter of interest
    uint16_t _elpsd;          ///< the time elapsed between the last and current measurements with now_time();

    public:
    // constructor
    StopWatch(TIM_TypeDef* p_Stpwtch,uint8_t tmrpin);
    // all thigs now measured from current time and returns that value
    uint16_t now_time(void); 
    // returns time elapsed in microseconds
    uint16_t elapsed_time(void); 
    // returns the time elapsed between measurments of a parameter of interest
    uint32_t lap(void);          
    // sets the count back to 0;
    void restart(void); 
    // pauses the timer until we want it to start counting again
    void temp_stop(void); 

};
#endif //STPWATCH_H