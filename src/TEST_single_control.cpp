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
    float DC = 100;

    //Set up variables for encoder
    float B_pos;
    float B_vel;
    float B_time;

    //Set the duty cycle
    Motor_B.setDutyCycle(DC);

    print_serial("Motor B Initialized\n");

    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity
        encoder_B_pos.get(B_pos);
        encoder_B_velocity.get(B_vel);
        encoder_B_time.get(B_time);

        //Print the encoder positions and velocity
        print_serial("Position:  ");    print_serial(B_pos);
        print_serial("  Velocity:  ");  print_serial(B_vel);
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
    float DC = 100;

    //Set up variables for encoder
    float A_pos;
    float A_vel;
    float A_time;

     Motor_A.setDutyCycle(DC);

    // //Set up variables for control loop
    // float error = 0;

    // //Set desired position
    // float desired_pos = 0.5;

    print_serial("Motor A Initialized\n");

    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity
        encoder_A_pos.get(A_pos);
        encoder_A_velocity.get(A_vel);
        encoder_A_time.get(A_time);

        // //Run Control Loop
        // error = desired_pos - A_pos;

        // DC = error*CONTROLLER_A_KP;

        // Motor_A.setDutyCycle(DC);

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