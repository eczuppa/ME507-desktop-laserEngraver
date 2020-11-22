/** @file motionplanning.cpp linearly interpolates between two points to create a ramp input to the controller
 *  @details this class generates ramp inputs for a classical postion PID control loop and is intented to
 *           maintain that control loop's linearity by limiting the size of the disturbances it is subjected to.
 *           First the time in microseconds it takes (at the current feed rate) for the laser to travel between
 *           the current and last setpoint are calculated. If the total travel period is less than the constant
 *           delta t for each ramp section no ramp is generated - the pointer to an array containing the current setpoint
 *           and current feed rate are returned. If it is the case the total travel period exceeds the ramp section time 
 *           differential, the number of ramp sections is calculated and array of length # of ramp sections + 2 is generated.
 *           The extra two spots hold the 
 * 
 * 
 */

// create the ramp inputs from the kinematics module to send to the contorller
// needs to caluclate the time it takes go to the posiiton and will use the current task timing to
// divide the total move into smaller linear movements and speeds so deviations from set point will be
// small which is better suited for the classical controls style controller we are using.

#include "libraries&constants.h"

MotionPlanning::MotionPlanning(float setpoint_current, float setpoint_last, float feed_setpoint, uint32_t ramp_dt_period)
{
    // Save inputs into class member data and scale everything by 100 so we can do integer division with remainders 
    _setpoint_current = setpoint_current;
    _setpoint_last = setpoint_last;
    _feed_setpoint = feed_setpoint;
    _ramp_dt_period = ramp_dt_period;

}

void MotionPlanning::ramp_generator(void)
{
    // calculates the time required to move from last to current point in microseconds
    float dist; 
    uint32_t ramp_sections = 0;
    uint32_t no_ramp;
    uint32_t index;
    bool even = false;
    int8_t aug_index = 0;

    dist = abs(_setpoint_current - _setpoint_last);
    _tot_travel_period = (dist*1000000)/_feed_setpoint; // travel period in microseconds
    
    // The time the movement takes between the current line of Gcode and the last is greater than the ramp section period
    // thus, we want to make the ramp input so our controller continues to behave in a linear manner and is not subjected to
    // large disturbances
    
    
    if (_tot_travel_period >= _ramp_dt_period)
    {
        // calculate number of ramp sections (close enough approx of euclid algorithm to get gcf using floating point version of %)
        ramp_sections = (_tot_travel_period - (fmodf(_tot_travel_period, _ramp_dt_period))) /_ramp_dt_period;   
        // calculate the time where the ramp section of specified length would not fit
        no_ramp = _tot_travel_period - (_ramp_dt_period*ramp_sections);  
        if (fmodf(_tot_travel_period, _ramp_dt_period) == 0)
        {
            // even division of ramp_dt_period into the total travel time, just make room for the current feed rate because the current setpoint will be included in the ramp
            ramp_sections ++;
            even = true;
        }
        else 
        {
            // generated ramp stops short of current setpoint becasue remaining time less than ramp_dt_period
            // make room for current setpoint and feed rate
            ramp_sections +=2;
        }

        float ramp[ramp_sections];   // static arrays can't be variable in length, wrapping with a struct is proving quite complicated,
                                     // figure out an easier way to do this or make it calculate just one piece at a time and send that to both controllers




           // array to hold ramp input to controller
        // setup for generation of ramp

        // if even division only change index by one to make room for current feed 
        if (even)
        {
            aug_index = 1;
        }
        // for un-even division change index by two to make room for current setpoint and current feed
        else
        {
            aug_index = 2;
        }
        
        // generate the ramp by iteration in a for loop
        for (index=0; (index=(ramp_sections - aug_index)); index++)
        {
            ramp[index] = ((_setpoint_current - _setpoint_last) * index * _ramp_dt_period) / _tot_travel_period;
        }
        
        // Add desired final value or values to the ramp 
        if(even)
        {
            memset(ramp,_feed_setpoint, ramp[sizeof(ramp)/sizeof(*ramp)]);           // set the last value of the array to the value of the feed_setpoint 
        }
        else
        {
            memset(ramp,_setpoint_current, ramp[(sizeof(ramp)/sizeof(*ramp))-1]);    // set penultimate value of the array to the value of the current setpoint
            memset(ramp,_feed_setpoint, ramp[sizeof(ramp)/sizeof(*ramp)]);           // set the last value of the array to the value of the feed_setpoint 
        
        }
        
        _output_ramp = &ramp[ramp_sections];                                    // set the class member data pointer output_ramp point to the generated ramp
    }
    
    // For very short or fast movements over sufficiently small distances - like the linear interpolation of arcs below a certain size
    // do not make a ramp. Instead put the current position setpoint and the feed setpoint into the array and make the class member data pointer
    // output ramp point to it.
    else if (_tot_travel_period < _ramp_dt_period)
    {
        int8_t ramp_length = 1;
        float ramp[ramp_length];
        ramp[0] = _setpoint_current;
        ramp[1] = _feed_setpoint;
        _output_ramp = &ramp[ramp_length];
    }

}

float * MotionPlanning::get_ramp(void)
{
    return _output_ramp;
}
