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
 *  @author Ethan A Czuppa
 *  @date   19 Nov 2020 Original file
 *  
 * 
 */

#include "libraries&constants.h"

/**  @brief   Motionplanning Class constructor which takes in decoded GCODE inputs and a user set discrete ramp section period
 *   @details Save all inputted arguments to class member data from the current and last lines of queued GCODE. Make the 
 *            pointer to the dynamically allocated array @c _output_ramp point to NULL in preparation for memory allocation. 
 *   @param   _setpoint_current
 *   @param   _setpoint_last
 *   @param   _feed_setpoint
 *   @param   _ramp_dt_period
 *   @param   _output_ramp
 */

MotionPlanning::MotionPlanning(float setpoint_current, float setpoint_last, float feed_setpoint, uint32_t ramp_dt_period)
{
    // Save inputs into class member data and setup dynamic array pointer 
    _setpoint_current = setpoint_current;
    _setpoint_last = setpoint_last;
    _feed_setpoint = feed_setpoint;
    _ramp_dt_period = ramp_dt_period;
    _output_ramp = NULL;

}

/**  @brief  allocates the memory for the ramp input and initalizes output_pointer
 *   @details calculates the required size of the dynamic array @c _output_ramp by 
 *            from the time required to traverse the position change specified by the
 *            current and last lines of GCODE and then creates the appropriate sized array
 *            to hold the ramp input to the control loop. 
 * 
 * 
 */

void MotionPlanning::init_ramp(void)
{
    float dist; 
    //uint32_t no_ramp
    dist = abs(_setpoint_current - _setpoint_last);
    _tot_travel_period = (dist*1000000)/_feed_setpoint; // travel period in microseconds
    
    // The time the movement takes between the current line of Gcode and the last is greater than the ramp section period
    // thus, we want to make the ramp input so our controller continues to behave in a linear manner and is not subjected to
    // large disturbances

    // calculate number of ramp sections (close enough approx of euclid algorithm to get gcf using floating point version of %)
    _ramp_sections = (_tot_travel_period - (fmodf(_tot_travel_period, _ramp_dt_period))) /_ramp_dt_period;   
    // calculate the time where the ramp section of specified length would not fit
    //no_ramp = _tot_travel_period - (_ramp_dt_period*ramp_sections);  
    if (_tot_travel_period <= _ramp_dt_period)
    {
        if (fmodf(_tot_travel_period, _ramp_dt_period) == 0)
        {
            // even division of ramp_dt_period into the total travel time, just make room for the current feed rate because the current setpoint will be included in the ramp
            _ramp_sections ++;
            _even = true;
        }
        else 
        {
            // generated ramp stops short of current setpoint becasue remaining time less than ramp_dt_period
            // make room for current setpoint and feed rate
            _ramp_sections +=2;
        }

        _output_ramp = new float[_ramp_sections];   // the pointer to the dynamically allocated array can now be used like a normal array
    }
    
    else if (_tot_travel_period < _ramp_dt_period)
    {
        _ramp_sections = 2;
        _output_ramp = new float[_ramp_sections];
    }


}

/**  @brief  generates the ramp input to the control loop as a dyanmic array
 *   @details
 * 
 * 
 */

void MotionPlanning::ramp_generator(void)
{
    // the _output_ramp array is only populated if the previous memory allocation was successful in the init_ramp method
    
    // calculates the time required to move from last to current point in microseconds
    if ((_tot_travel_period <= _ramp_dt_period) && _output_ramp)
    {
        int32_t index = 0;
        int8_t aug_index = 1;

           // array to hold ramp input to controller
        // setup for generation of ramp

        // if even division only change index by one to make room for current feed 
        if (_even)
        {
            aug_index = 1;
        }
        // for un-even division change index by two to make room for current setpoint and current feed
        else
        {
            aug_index = 2;
        }
        
        // generate the ramp by iteration in a for loop
        for (index=0; (index=(_ramp_sections - aug_index)); index++)
        {
            _output_ramp[index] = ((_setpoint_current - _setpoint_last) * index * _ramp_dt_period) / _tot_travel_period;
        }
        
        // Add desired final value or values to the ramp 
        if(_even)
        {
            memset(_output_ramp,_feed_setpoint, _output_ramp[_ramp_sections]);           // set the last value of the array to the value of the feed_setpoint 
        }
        else
        {
            memset(_output_ramp,_setpoint_current, _output_ramp[_ramp_sections-1]);    // set penultimate value of the array to the value of the current setpoint
            memset(_output_ramp,_feed_setpoint, _output_ramp[_ramp_sections]);           // set the last value of the array to the value of the feed_setpoint 
        
        }

    }

    // For very short or fast movements over sufficiently small distances - like the linear interpolation of arcs below a certain size
    // do not make a ramp. Instead put the current position setpoint and the feed setpoint into the array and make the class member data pointer
    // output ramp point to it.
    else if ((_tot_travel_period < _ramp_dt_period) && _output_ramp)
    {
        _output_ramp[0] = _setpoint_current;
        _output_ramp[1] = _feed_setpoint;
    }
    else
    {
        _output_ramp = NULL;
    }
    

}

/**  @brief returns the pointer to the first element of the ramp input to the controller
 *   @details
 * 
 * 
 */

float * MotionPlanning::get_ramp(void)
{
    return _output_ramp;   // returns the persistent reference to the dynamically allocated array _output_ramp
}

/**  @brief   deletes the dynamically allocated ramp input array and sets up for the next ramp
 *   @details   
 * 
 * 
 */

void MotionPlanning::deinit_ramp(void)
{
    delete [] _output_ramp;   // deallocate memory pointed to by *_output_ramp
    _output_ramp = NULL;      // make _ouput_ramp point back to NULL
    _ramp_sections = 0;       // zero out ramp sections so it is ready for the next ramp input
}

/**  @brief   allows the user to update the current position and feed rate setpoints
 *   @details
 * 
 * 
 */

void MotionPlanning::update_setpoints(float current_setpoint, float current_feedrate)
{
    _setpoint_last = _setpoint_current;     // save what was the current setpoint to the last one
    _setpoint_current = current_setpoint;   // save the new current setpoint to class member data
    _feed_setpoint = current_feedrate;      // save the new current feed rate to class member data
}