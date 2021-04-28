/** @file TEST_single_control.cpp
 *      This file is a test controller that will control a single motor.
 *
 *  @author  Niko Banks
 *  @date    3-23-2021 File Created
 *  @date    4-26-2021 Last Updated
 * 
*/

#include "libraries&constants.h"


// Shares for Encoders A and B
extern Share<encoder_output> encoder_A_output;
extern Share<encoder_output> encoder_B_output;

// Queue for Ramp Coefficients
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
    // float B_pos;
    // float B_vel;
    // float B_time;
    encoder_output enc_B_read;

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
        encoder_B_output.get(enc_B_read);

        if(enc_B_read.time >=3)
        {
            avg_count += 1;

            avg_vel = (avg_vel*(avg_count-1) + enc_B_read.vel) / (avg_count);
        }

        //Print the encoder positions and velocity
        print_serial("Position:  ");    print_serial(enc_B_read.pos);
        print_serial("  Velocity:  ");  print_serial(avg_vel);
        print_serial("  Time:  ");      print_serial(enc_B_read.time);
        print_serial("                                \r");
        // print_serial("                                \n");

        vTaskDelay(50);
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
    float DC = 10;
    Motor_A.setDutyCycle(DC);

    //Set up variables for encoder
    // float A_pos;
    // float A_vel;
    // float A_time;
    encoder_output enc_A_read;

    // Set up variables for control loop
    float error = 0;
    float integral = 0;
    float last_time = 0;

    // Initialize structs
    motor_setpoint setpoint;

    //Initialize classes
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
        encoder_A_output.get(enc_A_read);
        
        setpoint = xyoft.get_desired_pos_vel(enc_A_read.time);

        //Run Control Loop
        error = setpoint.A_pos - enc_A_read.pos;

        integral += (enc_A_read.time - last_time)*error;

        DC = error*CONTROLLER_A_KP + integral*CONTROLLER_A_KI;

        last_time = enc_A_read.time;

        //Account for motor deadband
        if(DC>=0)  {  DC = DC*( 100 - MOTOR_DEADBAND)/100 - MOTOR_DEADBAND;  }
        else       {  DC = DC*( 100 - MOTOR_DEADBAND)/100 + MOTOR_DEADBAND;  }
        
        Motor_A.setDutyCycle(DC);

        //Print the encoder positions and velocity
        print_serial("Position:  ");    print_serial(enc_A_read.pos);
        print_serial("  Velocity:  ");  print_serial(enc_A_read.vel);
        print_serial("  Time:  ");      print_serial(enc_A_read.time);
        print_serial("                                \r");
        // print_serial("                                \n");

        vTaskDelay(50);
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

    // Enable motors and set duty cycle
    Motor_A.enable();
    Motor_B.enable();
    float DC_A = 50;
    float DC_B = 50;

    //Set up variables for encoder
    encoder_output enc_A_read;
    encoder_output enc_B_read;


    //Set the duty cycle
    Motor_A.setDutyCycle(DC_A);
    Motor_B.setDutyCycle(DC_B);


    print_serial("Motors Initialized\n");
    print_serial("Pos: A     B    Vel: A     B   Time: A     B\n");

    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity
        encoder_A_output.get(enc_A_read);
        encoder_B_output.get(enc_B_read);


        //Print the encoder positions and velocity
        print_serial("     ");      print_serial(enc_A_read.pos);
        print_serial("  ");         print_serial(enc_B_read.pos);
        print_serial("      ");     print_serial(enc_A_read.vel);
        print_serial("  ");         print_serial(enc_B_read.vel);
        print_serial("      ");     print_serial(enc_A_read.time);
        print_serial("  ");         print_serial(enc_B_read.time);
        print_serial("                              \r");
        // print_serial("                                \n");

        vTaskDelay(50);
    }
}





// ============================= SUBFUNCTIONS =============================


// Testing task function
void task_test_script(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning

    for(;;)
    {
       
        vTaskDelay(1000);
    }
}

