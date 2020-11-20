
#ifndef STPWATCH_H
#define STPWATCH_H
#include <HardwareTimer.h>

class StopWatch
{
    protected:
    TIM_TypeDef* _p_Stpwtch;
    HardwareTimer* a_Tmr;
    uint8_t _tmrpin;
    uint16_t _now;
    uint16_t _elpsd;

    public:
    StopWatch(TIM_TypeDef* p_Stpwtch,uint8_t tmrpin);
    
    uint32_t now_time(void); // all thigs now measured from current time and returns that value
    uint32_t elapsed_time(void); // returns time elapsed in microseconds
    void restart(void); // sets the count back to 0;

};
#endif //STPWATCH_H