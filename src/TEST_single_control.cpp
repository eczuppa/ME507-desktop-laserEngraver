/** @file TEST_single_control.cpp
 *      This file is a test controller that will control a single motor.
 *
 *  @author  Niko Banks
 *  @date    3-23-2021 File Created
 *  @date    3-24-2021 Last Updated
 * 
*/

#include "libraries&constants.h"



// Shares for Encoder A
extern Share<float> encoder_A_pos;
extern Share<float> encoder_A_velocity;
extern Share<float> encoder_A_time;

// Shares for Encoder B
extern Share<float> encoder_B_pos;
extern Share<float> encoder_B_velocity;
extern Share<float> encoder_B_time;

extern Queue<ramp_segment_coefficients> ramp_segment_coefficient_queue;



/** @brief      Task which moves a single motor.
 *  @details    Fill in later
 *  @param      p_params A pointer to function parameters which we don't use.
 */
void task_single_control_B(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning

    // Set up instance of the motor driver class
    TB6612FNG Motor_B(STBY,BIN2,BIN1,PWM_B);

    //Set up motors and set duty cycle
    Motor_B.enable();
    float DC = 9;

    //Set up variables for encoder
    float B_pos;
    float B_vel;
    float B_time;

    //Set the duty cycle
    Motor_B.setDutyCycle(DC);

    //Average SS velocity for easy characterization
    float avg_vel = 0;
    uint16_t avg_count = 0;

    print_serial("Motor B Initialized\n");

    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity
        encoder_B_pos.get(B_pos);
        encoder_B_velocity.get(B_vel);
        encoder_B_time.get(B_time);

        if(B_time >=3)
        {
            avg_count += 1;

            avg_vel = (avg_vel*(avg_count-1) + B_vel) / (avg_count);
        }

        //Print the encoder positions and velocity
        print_serial("Position:  ");    print_serial(B_pos);
        print_serial("  Velocity:  ");  print_serial(avg_vel);
        print_serial("  Time:  ");      print_serial(B_time);
        print_serial("                                \r");
        // print_serial("                                \n");

        vTaskDelay(500);
    }
}



/** @brief      Task which moves a single motor.
 *  @details    Fill in later
 *  @param      p_params A pointer to function parameters which we don't use.
 */
void task_single_control_A(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning

    // Set up instance of the motor driver class
    TB6612FNG Motor_A(STBY,AIN2,AIN1,PWM_A);

    //Set up motor and set duty cycle
    Motor_A.enable();
    float DC = 0;

    //Set up variables for encoder
    float A_pos;
    float A_vel;
    float A_time;

    Motor_A.setDutyCycle(DC);

    // //Set up variables for control loop
    float error = 0;
    float integral = 0;
    float last_time = 0;

    // //Set desired position
    motor_setpoint setpoint;
    setpoint_of_time xyoft;

    print_serial("Motor A Initialized\n");

    //Create ramp coefficients for simulation
    ramp_segment_coefficients ramp1;
    ramp_segment_coefficients ramp2;
    ramp_segment_coefficients ramp3;

    //                            sec                rotations                 rps
    // ramp1.t0 = 0;    ramp1.t_end = 5;       ramp1.pos_A0 =  0;    ramp1.vel_A =   2;    
    // ramp2.t0 = 5;    ramp2.t_end = 10;      ramp2.pos_A0 = 10;    ramp2.vel_A =   0;    
    // ramp3.t0 = 10;   ramp3.t_end = 15;      ramp3.pos_A0 = 10;    ramp3.vel_A =  -2;    

    ramp1.t0 = 0;    ramp1.t_end = 5;       ramp1.pos_A0 =  0;    ramp1.vel_A =   5;    
    ramp2.t0 = 5;    ramp2.t_end = 10;      ramp2.pos_A0 = 25;    ramp2.vel_A =   0;    
    ramp3.t0 = 10;   ramp3.t_end = 15;      ramp3.pos_A0 = 25;    ramp3.vel_A =  -5;  

    ramp_segment_coefficient_queue.put(ramp1);
    ramp_segment_coefficient_queue.put(ramp2);
    ramp_segment_coefficient_queue.put(ramp3);

    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity
        encoder_A_pos.get(A_pos);
        encoder_A_velocity.get(A_vel);
        encoder_A_time.get(A_time);


        
        setpoint = xyoft.get_desired_pos_vel(A_time);

        //Run Control Loop
        error = setpoint.A_pos - A_pos;

        integral += (A_time - last_time)*error;

        DC = error*CONTROLLER_A_KP + integral*CONTROLLER_A_KI;

        last_time = A_time;

        //Account for motor deadband
        if(DC>=0)  {  DC = DC*( 100 - MOTOR_DEADBAND)/100 - MOTOR_DEADBAND;  }
        else       {  DC = DC*( 100 - MOTOR_DEADBAND)/100 + MOTOR_DEADBAND;  }
        
        Motor_A.setDutyCycle(DC);

        //Print the encoder positions and velocity
        print_serial("Position:  ");    print_serial(A_pos);
        print_serial("  Velocity:  ");  print_serial(A_vel);
        print_serial("  Time:  ");      print_serial(A_time);
        print_serial("                                \r");
        // print_serial("                                \n");

        vTaskDelay(500);
    }
}


/** @brief      Task which moves a single motor.
 *  @details    Fill in later
 *  @param      p_params A pointer to function parameters which we don't use.
 */
void task_single_control_both(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning

    // Set up instance of the motor driver class
    TB6612FNG Motor_A(STBY,AIN2,AIN1,PWM_A);
    TB6612FNG Motor_B(STBY,BIN2,BIN1,PWM_B);

    //Set up motor and set duty cycle
    Motor_A.enable();
    Motor_B.enable();
    float DC_A = 100;
    float DC_B = 100;

    //Set up variables for encoder
    float A_pos;
    float A_vel;
    float A_time;
    float B_pos;
    float B_vel;
    float B_time;

    //Set the duty cycle
    Motor_A.setDutyCycle(DC_A);
    Motor_B.setDutyCycle(DC_B);


    print_serial("Motors Initialized\n");
    print_serial("Pos: A     B    Vel: A     B   Time: A     B\n");

    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity
        encoder_A_pos.get(A_pos);
        encoder_A_velocity.get(A_vel);
        encoder_A_time.get(A_time);

        encoder_B_pos.get(B_pos);
        encoder_B_velocity.get(B_vel);
        encoder_B_time.get(B_time);


        //Print the encoder positions and velocity
        print_serial("     ");      print_serial(A_pos);
        print_serial("  ");         print_serial(B_pos);
        print_serial("      ");     print_serial(A_vel);
        print_serial("  ");         print_serial(B_vel);
        print_serial("      ");     print_serial(A_time);
        print_serial("  ");         print_serial(B_time);
        print_serial("                              \r");
        // print_serial("                                \n");

        vTaskDelay(500);
    }
}





// ============================= SUBFUNCTIONS =============================

setpoint_of_time::setpoint_of_time(void)
{
    //Don't do anything with the constructor (yet)
}


motor_setpoint setpoint_of_time::get_desired_pos_vel(float time)
{
    //Figure out which segment of the desired curve that we're looking at based on the time we're given; 
    //that will determine what the constants we can use are
    
    //Create instance of struct containing setpoints
    motor_setpoint setpoint;

    bool checking_coefficients = true;

    //Find out which segment we're looking at
    while(checking_coefficients)    //Loop used to continuously check the coefficients until we find a set that is within our time range
    {
        if(time > _seg_coeff.t_end)     //We've passed the end of the current ramp segment; we may need to update coefficients
        {
            if(ramp_segment_coefficient_queue.is_empty())
            {
                //If there are no new coefficients available, set positions to the final desired position and then 
                //set velocities to 0 to keep the position steady
                _seg_coeff.pos_A0 = _seg_coeff.vel_A * (_seg_coeff.t_end - _seg_coeff.t0)  + _seg_coeff.pos_A0;
                _seg_coeff.pos_B0 = _seg_coeff.vel_B * (_seg_coeff.t_end - _seg_coeff.t0)  + _seg_coeff.pos_B0;

                _seg_coeff.vel_A = 0;
                _seg_coeff.vel_B = 0;

                //Get us out of the checking loop
                checking_coefficients = false;
            }
            else //We have new coefficients in the queue; get it and replace the old _seg_coeff with this new set.
            {
                ramp_segment_coefficient_queue.get(_seg_coeff);
            }
        }
        else //If time <= t_end of the current segment, we don't have to change the segment coefficients. 
        {
            checking_coefficients = false;
        }
    } //while(checking_coefficients)

    //Caclulate desired position
    // A(t)  =  vel_A*(t-t0) + A0
    setpoint.A_pos = _seg_coeff.vel_A * (time - _seg_coeff.t0)  + _seg_coeff.pos_A0;
    setpoint.B_pos = _seg_coeff.vel_B * (time - _seg_coeff.t0)  + _seg_coeff.pos_B0;

    //Send desired velocity to the output struct
    // setpoint.A_vel = _seg_coeff.vel_A;
    setpoint.A_vel = _seg_coeff.vel_A;
    setpoint.B_vel = _seg_coeff.vel_B;

    return setpoint;
}







// Testing task function
void task_test_script(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning

    for(;;)
    {
       
        vTaskDelay(1000);
    }
}

