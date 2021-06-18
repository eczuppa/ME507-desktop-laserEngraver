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
extern Share<encoder_output> enc_A_output_share;
extern Share<encoder_output> enc_B_output_share;

// Queue for Ramp Coefficients
extern Queue<ramp_segment_coefficients> ramp_segment_coefficient_queue;

//Queue that holds read character arrays (not necessary here except for testing)
extern Queue<char[LINE_BUFFER_SIZE]> read_chars_queue;



/** @brief      Task which runs a motor at a set duty cycle and reads average speed.
 *  @details    This function runs a motor at a set duty cycle, reads the output from the encoder for the motor, 
 *              and outputs position, average or instantaneous velocity, and time of operation. The motor, duty cycle,
 *              and test mode can be specified in @c motor_test_tasks.h.
 *  @param      p_params A pointer to function parameters which we don't use.
 */
void task_test_const_velocity(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning

    char motor_id = CONST_VELOCITY_TEST_MOTOR;

    //Set up default pins
    uint8_t stby_pin = STBY;        uint8_t mot_pin_1 = AIN2;
    uint8_t pwm_pin = PWM_A;        uint8_t mot_pin_2 = AIN1;
    
    //Change pins for selected motor
    if(motor_id == 'A' || motor_id == 'a')
    {
        stby_pin = STBY;        mot_pin_1 = AIN2;       mot_pin_2 = AIN1;       pwm_pin = PWM_A;
    }
    else if(motor_id == 'B' || motor_id == 'b')
    {
        stby_pin = STBY;        mot_pin_1 = BIN2;       mot_pin_2 = BIN1;       pwm_pin = PWM_B;
    }
    else
    {
        print_serial("Warning: motor_id does match options; motor A chosen by default\n");
    }
    
    // Set up instance of the motor driver class
    TB6612FNG Motor(stby_pin,mot_pin_1,mot_pin_2,pwm_pin);

    //Set up motors and set duty cycle
    Motor.enable();
    float DC = CONST_VELOCITY_TEST_DC;
    Motor.setDutyCycle(DC);

    //Set up variables for encoder
    encoder_output enc_read;
    
    //Create intermediate variables for calculations
    float avg_vel = 0;
    uint16_t avg_count = 0;
    float vel_out = 0;

    //Create printouts for specification
    print_serial("\nRunning Motor: ");   print_serial(motor_id);
    if     (CONST_VELOCITY_TEST_MODE == 1){ print_serial("\nMode: Average Velocity\n");      }
    else if(CONST_VELOCITY_TEST_MODE == 0){ print_serial("\nMode: Instantaneous Velocity\n");}
    print_serial("Duty Cycle: ");   print_serial(DC);   print_serial("\n\n");



    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity
        if(motor_id == 'B' || motor_id == 'b')
        {
            enc_B_output_share.get(enc_read);
        }
        else
        {
            enc_A_output_share.get(enc_read);
        }
        

        // Calculate average velocity if required (start calculation after 3 seconds to skip motor startup)
        if(CONST_VELOCITY_TEST_MODE == 1)
        {
            if(enc_read.time >=3)
            {
                avg_count += 1;

                avg_vel = (avg_vel*(avg_count-1) + enc_read.vel) / (avg_count);
            }
            vel_out = avg_vel;
        }
        else if(CONST_VELOCITY_TEST_MODE == 0)
        {
            vel_out = enc_read.vel;
        }
        

        //Print the encoder positions and velocity
        print_serial("Position:  ");    print_serial(enc_read.pos);
        print_serial("  Velocity:  ");  print_serial(vel_out);
        print_serial("  Time:  ");      print_serial(enc_read.time);
        print_serial("                                \r");
        // print_serial("                                \n");

        vTaskDelay(50);
    }
}



/** @brief      Task which runs a motor in a set path via a control loop.
 *  @details    This function runs a motor to follow a pre-determined path using a PID control loop. Each of the PID
 *              constants, the motor deadband, and the selected motor the test can be specified in @c motor_test_tasks.h.
 *  @param      p_params A pointer to function parameters which we don't use.
 */
void task_test_control_path(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning

    char motor_id = PATH_TEST_MOTOR;

    //Set up default pins
    uint8_t stby_pin = STBY;        uint8_t mot_pin_1 = AIN2;
    uint8_t pwm_pin = PWM_A;        uint8_t mot_pin_2 = AIN1;
    
    //Change pins for selected motor
    if(motor_id == 'A' || motor_id == 'a')
    {
        stby_pin = STBY;        mot_pin_1 = AIN2;       mot_pin_2 = AIN1;       pwm_pin = PWM_A;
    }
    else if(motor_id == 'B' || motor_id == 'b')
    {
        stby_pin = STBY;        mot_pin_1 = BIN2;       mot_pin_2 = BIN1;       pwm_pin = PWM_B;
    }
    else
    {
        print_serial("Warning: motor_id does match options; motor A chosen by default\n");
    }

    // Set up instance of the motor driver class
    TB6612FNG Motor(stby_pin,mot_pin_1,mot_pin_2,pwm_pin);

    //Set up motor and set duty cycle
    Motor.enable();
    float DC = 0;
    Motor.setDutyCycle(DC);

    //Set up variables for encoder
    encoder_output enc_read;

    // Set up variables for control loop
    float error = 0;
    float integral = 0;
    float last_time = 0;

    // Initialize structs
    motor_setpoint setpoint;

    //Initialize classes
    setpoint_of_time xyoft;


    // Printouts for reference
    print_serial("\nRunning Motor: ");   print_serial(motor_id);
    print_serial("\nKP: ");   print_serial((float)PATH_TEST_CONTROL_KP);
    print_serial("\nKI: ");   print_serial((float)PATH_TEST_CONTROL_KI);
    print_serial("\nKD: ");   print_serial((float)PATH_TEST_CONTROL_KD);
    print_serial("\nMotor Deadband: ");   print_serial((float)PATH_TEST_MOTOR_DEADBAND); print_serial("\n\n");


    // Create ramp coefficients for simulation
    //Method 1: Raw Ramps
    ramp_segment_coefficients ramp1;
    ramp_segment_coefficients ramp2;
    ramp_segment_coefficients ramp3;

    //                            sec                rotations                 rps 
    ramp1.t0 = 0;    ramp1.t_end = 5;       ramp1.pos_A0 =  0;    ramp1.vel_A =   5;    ramp1.pos_B0 =  0;      ramp1.vel_B =   5;
    ramp2.t0 = 5;    ramp2.t_end = 10;      ramp2.pos_A0 = 25;    ramp2.vel_A =   0;    ramp2.pos_B0 = 25;      ramp2.vel_B =   0; 
    ramp3.t0 = 10;   ramp3.t_end = 15;      ramp3.pos_A0 = 25;    ramp3.vel_A =  -5;    ramp3.pos_B0 = 25;      ramp3.vel_B =  -5;

    ramp_segment_coefficient_queue.put(ramp1);
    ramp_segment_coefficient_queue.put(ramp2);
    ramp_segment_coefficient_queue.put(ramp3);

    // //Method 2: XYSF
    // coreXY_to_AB translator;
    // XYSFvalues point1;      XYSFvalues point2;
    
    // point1.X = 25;       point1.Y = 0;       point1.F = 5;
    // point2.X =  0;       point2.Y = 0;       point2.F = 5;

    // translator.translate_to_queue(point1);
    // translator.translate_to_queue(point2);


    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity
        if(motor_id == 'B' || motor_id == 'b')
        {
            enc_B_output_share.get(enc_read);
        }
        else
        {
            enc_A_output_share.get(enc_read);
        }

        //Get the setpoint for this moment in time
        setpoint = xyoft.get_desired_pos_vel(enc_read.time);

        //Run Control Loop
        if     (motor_id == 'A' || motor_id == 'a'){ error = setpoint.A_pos - enc_read.pos; }
        else if(motor_id == 'B' || motor_id == 'b'){ error = setpoint.B_pos - enc_read.pos; }
        
        integral += (enc_read.time - last_time)*error;

        DC = error*PATH_TEST_CONTROL_KP + integral*PATH_TEST_CONTROL_KI;

        last_time = enc_read.time;

        //Account for motor deadband
        if(DC>=0)  {  DC = DC*( 100 - PATH_TEST_MOTOR_DEADBAND)/100 + PATH_TEST_MOTOR_DEADBAND;  }
        else       {  DC = DC*( 100 - PATH_TEST_MOTOR_DEADBAND)/100 - PATH_TEST_MOTOR_DEADBAND;  }
        
        Motor.setDutyCycle(DC);

        //Print the encoder positions and velocity
        print_serial("Position:  ");    print_serial(enc_read.pos);
        print_serial("  Velocity:  ");  print_serial(enc_read.vel);
        print_serial("  Time:  ");      print_serial(enc_read.time);
        print_serial("  Error:  ");     print_serial(error);
        print_serial("                                \r");
        // print_serial("                                \n");

        vTaskDelay(50);
    }
}




/** @brief      Task which moves a single motor.
 *  @details    Work in progress; test running both motors at the same time. 
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
        enc_A_output_share.get(enc_A_read);
        enc_B_output_share.get(enc_B_read);


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




/** @brief      Task which moves a single motor.
 *  @details    Work in progress; test running both motors at the same time. 
 *  @param      p_params A pointer to function parameters which we don't use.
 */
void task_test_gcode_response(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning

    // Set up instance of the motor driver class
    TB6612FNG Motor(STBY,AIN2,AIN1,PWM_A);

    //Set up motor and set duty cycle
    Motor.enable();
    float DC = 0;
    Motor.setDutyCycle(DC);

    //Set up variables for encoder
    encoder_output enc_read;

    // Set up variables for control loop
    float error = 0;
    float integral = 0;
    float last_time = 0;

    // Initialize structs
    motor_setpoint setpoint;

    //Initialize classes
    setpoint_of_time xyoft;


    //Add lines of gcode to be interpreted
    char line[LINE_BUFFER_SIZE] = "G1 X46.18 Y-51.74 S1.00 F600";
    read_chars_queue.put(line);

    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity
        enc_A_output_share.get(enc_read);

        //Get the setpoint for this moment in time
        setpoint = xyoft.get_desired_pos_vel(enc_read.time);

        //Run Control Loop
        error = setpoint.A_pos - enc_read.pos;
        
        integral += (enc_read.time - last_time)*error;

        DC = error*PATH_TEST_CONTROL_KP + integral*PATH_TEST_CONTROL_KI;

        last_time = enc_read.time;

        //Account for motor deadband
        if(DC>=0)  {  DC = DC*( 100 - PATH_TEST_MOTOR_DEADBAND)/100 - PATH_TEST_MOTOR_DEADBAND;  }
        else       {  DC = DC*( 100 - PATH_TEST_MOTOR_DEADBAND)/100 + PATH_TEST_MOTOR_DEADBAND;  }
        
        Motor.setDutyCycle(DC);

        //Print the encoder positions and velocity
        print_serial("Position:  ");    print_serial(enc_read.pos);
        print_serial("  Velocity:  ");  print_serial(enc_read.vel);
        print_serial("  Time:  ");      print_serial(enc_read.time);
        print_serial("                                \r");
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

