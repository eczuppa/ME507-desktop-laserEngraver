/** @file       control.cpp
 *  @brief      File containing a class that implements a PID position control to drive the motors.
 * 
 *  @author Niko Banks
 *
 *  @date  Aug 13 2022 Original file
 */

#include "libraries&constants.h"

// ============================================================================================================
// =========================================== PID Controller Class ===========================================
// ============================================================================================================


/** @brief   Contrsuctor for PID controller class
 *  @details Creates a PID controller class object.
 * 
 *  @param KP Proportional gain
 *  @param KI Integral gain
 *  @param KD Derivative gain
 *  @param deadband Min value of output signal that produces a response in the system
 *  @param high_sat_limit Max value allowed for output signal
 *  @param low_sat_limit Min value allowed for output signal
 */

 PID_Controller::PID_Controller(float KP, float KI, float KD, float deadband, float high_sat_limit, float low_sat_limit, float drv_filt_tau)
 {
    //Store gains to internal values
    _KP = KP;     _KI = KI;     _KD = KD;

    //Store deadband
    _deadband = deadband;

    //Store derivative filter time constant
    _drv_filt_tau = drv_filt_tau;

    //Initialize saturation limit and current saturation
    _high_sat_limit = high_sat_limit;
    _low_sat_limit = low_sat_limit; 
    _controller_saturated = false;

    //Set last positions, time, and error to 0
    _last_pos = 0;    _last_time = 0;   _last_error = 0;

    //Reset errors
    _derivative_error = 0;
    _integral_error = 0;
    _error = 0;

 }


// ============================================================================================================
// ============================================ Main Control Loop =============================================
// ============================================================================================================

/** @brief   Set up the PID control loop
 *  @details Runs one loop through the PID controller and outputs the resulting signal to send. This controller
 *           also includes deadband adjustments and integral clamping.
 *  @param meas_pos     Measured Position at the current time
 *  @param setpoint_pos Desired position at current time
 *  @param time         The current time
 *  
 *  @returns _output_signal The signal output from the PID controller
 */
 float PID_Controller::Run_Control_Loop(float meas_pos, float setpoint_pos, float time)
 {
    //Calculate raw error
    _error = setpoint_pos - meas_pos;

    //Calculate time delta
    _dt = time - _last_time;

    

    //Calculate integral error, with clamping if signal is saturated
    if (!_controller_saturated)
    {
        _integral_error += _dt*_error;
    }

    //Calculate the derivative error with a filter
    _drv_filt_coeff = _drv_filt_tau/(_drv_filt_tau + _dt);
    _raw_drv_error = (_error - _last_error )/_dt;
    _derivative_error = _raw_drv_error*(1-_drv_filt_coeff) + _derivative_error*_drv_filt_coeff;


    //Create signal from proportional, integral, and derivative signals
    _output_signal = _error*_KP + _integral_error*_KI  + _derivative_error*_KD;

    // Serial << _raw_drv_error*(1-_drv_filt_coeff) << endl;

    //Account for motor deadband: Makes minimum magnitude of signal sent out equal to the motor deadband, 
    if(_output_signal>=0)  {  _output_signal = _output_signal*( 100 - _deadband)/100 + _deadband;  }
    else                   {  _output_signal = _output_signal*( 100 - _deadband)/100 - _deadband;  }



    //Check for saturation and limit signal if saturated
    if     (_output_signal >  _high_sat_limit)  //Positive saturation
    {
        _output_signal =  _high_sat_limit; 
        _controller_saturated = true;
    }   
    else if(_output_signal < _low_sat_limit) //Negative saturation
    {
        _output_signal = _low_sat_limit;
        _controller_saturated = true;
    }    
    else //Not saturated
    {
        _controller_saturated = false;
    }


    //Reset variables
    _last_time = time;
    _last_error = _error;

    //Return DC signal as output
    return _output_signal;
 }




// ============================================================================================================
// ============================================= Setter functions =============================================
// ============================================================================================================


/** @brief   Set the controller's gain coefficient KP
 *  @param   KP A new value for the controller's gain coefficient, KP
 */
void PID_Controller::set_KP(float KP)
{
    _KP = KP;
}

/** @brief   Set the controller's gain coefficient KI
 *  @param   KI A new value for the controller's gain coefficient, KI
 */
void PID_Controller::set_KI(float KI)
{
    _KI = KI;
}

/** @brief   Set the controller's gain coefficient KD
 *  @param   KD A new value for the controller's gain coefficient, KD
 */
void PID_Controller::set_KD(float KD)
{
    _KD = KD;
}

/** @brief   Set the controller deadband
 *  @param   deadband A new value for the controller deadband
 */
void PID_Controller::set_deadband(float deadband)
{
    _deadband = deadband;
}

/** @brief   Set the controller derivative filter time constant
 *  @param   deadband A new value for the controller derivative filter time constant
 */
void PID_Controller::set_drv_filt_tau(float drv_filt_tau)
{
    _drv_filt_tau = drv_filt_tau;
}



// ============================================================================================================
// ============================================= Getter functions =============================================
// ============================================================================================================


/** @brief   Get the controller gain coefficient KP */
float PID_Controller::get_KP(void)
{
    return _KP;
}

/** @brief   Get the controller gain coefficient KI */
float PID_Controller::get_KI(void)
{
    return _KI;
}

/** @brief   Get the controller gain coefficient KD */
float PID_Controller::get_KD(void)
{
    return _KD;
}

/** @brief   Get the controller deadband */
float PID_Controller::get_deadband(void)
{
    return _deadband;
}

/** @brief   Get the controller derivative filter time constant */
float PID_Controller::get_drv_filt_tau(void)
{
    return _drv_filt_tau;
}

/** @brief   Get the controller error */
float PID_Controller::get_error(void)
{
    return _error;
}

/** @brief   Get the controller derivative error */
float PID_Controller::get_drv_error(void)
{
    return _derivative_error;
}

/** @brief   Get the controller integral error */
float PID_Controller::get_int_error(void)
{
    return _integral_error;
}