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

    bool motor_selected = false;
    uint8_t motor_choice = CONST_VELOCITY_TEST_MOTOR; 

    //Set up default pins
    uint8_t stby_pin = STBY;  
    uint8_t pwmA_pin = PWM_A;           uint8_t pwmB_pin = PWM_B;          
    uint8_t motA_pin_1 = AIN2;          uint8_t motB_pin_1 = BIN2;
    uint8_t motA_pin_2 = AIN1;          uint8_t motB_pin_2 = BIN1;

    //Set up variables
    float                DC_A = 0;      float                DC_B = 0;
    float           avg_vel_A = 0;      float           avg_vel_B = 0;
    uint16_t      avg_count_A = 0;      uint16_t      avg_count_B = 0;
    float           vel_out_A = 0;      float           vel_out_B = 0;
    encoder_output enc_read_A;          encoder_output enc_read_B;

    //Create motor instances
    TB6612FNG Motor_A(stby_pin,motA_pin_1,motA_pin_2,pwmA_pin);
    TB6612FNG Motor_B(stby_pin,motB_pin_1,motB_pin_2,pwmB_pin);
    

    print_serial("Constant Velocity Test Initialized\n");

    // Choose which motors to run in the test
    while(motor_selected == false)
    {
        //Motor A setup:
        if(motor_choice == LASER_CUTTER_MOTOR_A || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            motor_selected = true;            

            //Set up motors and set duty cycle
            Motor_A.enable();
            DC_A = CONST_VELOCITY_TEST_DC_A;
            Motor_A.setDutyCycle(DC_A);            
        }

        //Motor B setup:
        if(motor_choice == LASER_CUTTER_MOTOR_B || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            motor_selected = true;
            
            //Set up motors and set duty cycle
            Motor_B.enable();
            DC_B = CONST_VELOCITY_TEST_DC_B;
            Motor_B.setDutyCycle(DC_B);
        }

        //If no motor is selected or the selection was done incorrectly, enable the override and run motor A.
        if(motor_selected == false)
        {
            print_serial("\nWarning: motor_id does match options; motor A chosen by default\n");
            motor_choice = LASER_CUTTER_MOTOR_A;
        }
    }

    //Create printouts for specification
    switch(motor_choice)
    {
        case LASER_CUTTER_MOTOR_A:
            print_serial("\nRunning Motor: A");
            print_serial("\nDuty Cycle: ");   print_serial(DC_A);
            break;

        case LASER_CUTTER_MOTOR_B:
            print_serial("\nRunning Motor: B");
            print_serial("\nDuty Cycle: ");   print_serial(DC_B);
            break;

        case LASER_CUTTER_MOTOR_BOTH:
        default:
            print_serial("\nRunning Motor: A & B");
            print_serial("\nDuty Cycle A: ");   print_serial(DC_A);
            print_serial("\nDuty Cycle B: ");   print_serial(DC_B);
            break;
    }
    if     (CONST_VELOCITY_TEST_MODE == CONST_VEL_MODE_AVERAGE)      { print_serial("\nMode: Average Velocity\n\n");      }
    else if(CONST_VELOCITY_TEST_MODE == CONST_VEL_MODE_INSTANTANEOUS){ print_serial("\nMode: Instantaneous Velocity\n\n");}



    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity and convert units to desired
        if(motor_choice == LASER_CUTTER_MOTOR_A || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            enc_A_output_share.get(enc_read_A);
            enc_read_A.pos = convert_units(enc_read_A.pos,ENC_POSITION_MODE_REVOUT);
            enc_read_A.vel = convert_units(enc_read_A.vel,ENC_VELOCITY_MODE_RPMOUT);
        }
        if(motor_choice == LASER_CUTTER_MOTOR_B || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            enc_B_output_share.get(enc_read_B);
            enc_read_B.pos = convert_units(enc_read_B.pos,ENC_POSITION_MODE_REVOUT);
            enc_read_B.vel = convert_units(enc_read_B.vel,ENC_VELOCITY_MODE_RPMOUT);
        }
        

        if(motor_choice == LASER_CUTTER_MOTOR_A || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            // Calculate average velocity if required (start calculation after 3 seconds to skip motor startup)
            if(CONST_VELOCITY_TEST_MODE == CONST_VEL_MODE_AVERAGE)
            {
                if(enc_read_A.time >=3)
                {
                    avg_count_A += 1;
                    avg_vel_A = (avg_vel_A*(avg_count_A-1) + enc_read_A.vel) / (avg_count_A);
                }
                vel_out_A = avg_vel_A;
            }
            //If instantaneous, just output the value
            else if(CONST_VELOCITY_TEST_MODE == CONST_VEL_MODE_INSTANTANEOUS)
            {
                vel_out_A = enc_read_A.vel;
            }
        }

        if(motor_choice == LASER_CUTTER_MOTOR_B || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            // Calculate average velocity if required (start calculation after 3 seconds to skip motor startup)
            if(CONST_VELOCITY_TEST_MODE == CONST_VEL_MODE_AVERAGE)
            {
                if(enc_read_B.time >=3)
                {
                    avg_count_B += 1;
                    avg_vel_B = (avg_vel_B*(avg_count_B-1) + enc_read_B.vel) / (avg_count_B);
                }
                vel_out_B = avg_vel_B;
            }
            //If instantaneous, just output the value
            else if(CONST_VELOCITY_TEST_MODE == CONST_VEL_MODE_INSTANTANEOUS)
            {
                vel_out_B = enc_read_B.vel;
            }
        }
            
        //Print the encoder positions and velocity
        if(motor_choice == LASER_CUTTER_MOTOR_A)
        {
            print_serial("Position:  ");    print_serial(enc_read_A.pos);
            print_serial("  Velocity:  ");  print_serial(vel_out_A);
            print_serial("  Time:  ");      print_serial(enc_read_A.time);
            print_serial("                                \r");
        }
        else if(motor_choice == LASER_CUTTER_MOTOR_B)
        {
            print_serial("Position:  ");    print_serial(enc_read_B.pos);
            print_serial("  Velocity:  ");  print_serial(vel_out_B);
            print_serial("  Time:  ");      print_serial(enc_read_B.time);
            print_serial("                                \r");
        }
        else if(motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            print_serial("Position: ");    print_serial(enc_read_A.pos); print_serial(" (A)  ");  print_serial(enc_read_B.pos);  print_serial(" (B)  ");
            print_serial("  Velocity: ");  print_serial(vel_out_A);      print_serial(" (A)  ");  print_serial(vel_out_B);       print_serial(" (B)  ");
            print_serial("  Time:  ");      print_serial(enc_read_A.time);
            print_serial("                                \r");
        }
        
        if(motor_choice == LASER_CUTTER_MOTOR_BOTH){vTaskDelay(200);}
        else{vTaskDelay(50);}
            
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

    //Set up motor parameters
    bool motor_selected = false;
    uint8_t motor_choice = PATH_TEST_MOTOR;
    
    //Set up default pins
    uint8_t stby_pin = STBY;  
    uint8_t pwmA_pin = PWM_A;           uint8_t pwmB_pin = PWM_B;          
    uint8_t motA_pin_1 = AIN2;          uint8_t motB_pin_1 = BIN2;
    uint8_t motA_pin_2 = AIN1;          uint8_t motB_pin_2 = BIN1;

    //Set up variables
    float DC_A;                         float DC_B;
    encoder_output enc_read_A;          encoder_output enc_read_B;

    //Create motor instances
    TB6612FNG Motor_A(stby_pin,motA_pin_1,motA_pin_2,pwmA_pin);
    TB6612FNG Motor_B(stby_pin,motB_pin_1,motB_pin_2,pwmB_pin);


    // Initialize struct containing position and velocity setpoint for motors A and B
    motor_setpoint setpoint;

    //Initialize setpoint of time class to calculate specific setpoints based on time
    setpoint_of_time xyoft;

    //Set up controllers
    PID_Controller control_A(PATH_TEST_CONTROL_KP, PATH_TEST_CONTROL_KI, PATH_TEST_CONTROL_KD, PATH_TEST_MOTOR_DEADBAND, 100, -100, PATH_TEST_DRV_FILTER_TAU);
    PID_Controller control_B(PATH_TEST_CONTROL_KP, PATH_TEST_CONTROL_KI, PATH_TEST_CONTROL_KD, PATH_TEST_MOTOR_DEADBAND, 100, -100, PATH_TEST_DRV_FILTER_TAU);

    print_serial("Control Path Test Initialized\n");

    //------------------------------------------------

    // Choose which motors to run in the test
    while(motor_selected == false)
    {
        //Motor A setup:
        if(motor_choice == LASER_CUTTER_MOTOR_A || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            motor_selected = true;            

            //Set up motors and set duty cycle
            Motor_A.enable();
            DC_A = CONST_VELOCITY_TEST_DC_A;
            Motor_A.setDutyCycle(DC_A);            
        }

        //Motor B setup:
        if(motor_choice == LASER_CUTTER_MOTOR_B || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            motor_selected = true;
            
            //Set up motors and set duty cycle
            Motor_B.enable();
            DC_B = CONST_VELOCITY_TEST_DC_B;
            Motor_B.setDutyCycle(DC_B);
        }

        //If no motor is selected or the selection was done incorrectly, enable the override and run motor A.
        if(motor_selected == false)
        {
            print_serial("\nWarning: motor_id does match options; motor A chosen by default\n");
            motor_choice = LASER_CUTTER_MOTOR_A;
        }
    }

    //Create printouts for specification
    switch(motor_choice)
    {
        case LASER_CUTTER_MOTOR_A:
            print_serial("\nRunning Motor: A");
            break;

        case LASER_CUTTER_MOTOR_B:
            print_serial("\nRunning Motor: B");
            break;

        case LASER_CUTTER_MOTOR_BOTH:
        default:
            print_serial("\nRunning Motor: A & B");
            break;
    }
    // Printouts for reference
    print_serial("\nKP: ");   print_serial((float)PATH_TEST_CONTROL_KP);
    print_serial("\nKI: ");   print_serial((float)PATH_TEST_CONTROL_KI);
    print_serial("\nKD: ");   print_serial((float)PATH_TEST_CONTROL_KD);
    print_serial("\nMotor Deadband: ");   print_serial((float)PATH_TEST_MOTOR_DEADBAND); print_serial("\n\n");


    // Create ramp coefficients for simulation
    //Method 1: Raw Ramps
    ramp_segment_coefficients ramp1;
    ramp_segment_coefficients ramp2;
    ramp_segment_coefficients ramp3;

    // //                            sec                rotations                 rps 
    // ramp1.t0 = 0;    ramp1.t_end = 5;       ramp1.pos_A0 =  0;    ramp1.vel_A =   5;    ramp1.pos_B0 =  0;      ramp1.vel_B =   5;
    // ramp2.t0 = 5;    ramp2.t_end = 10;      ramp2.pos_A0 = 25;    ramp2.vel_A =   0;    ramp2.pos_B0 = 25;      ramp2.vel_B =   0; 
    // ramp3.t0 = 10;   ramp3.t_end = 15;      ramp3.pos_A0 = 25;    ramp3.vel_A =  -5;    ramp3.pos_B0 = 25;      ramp3.vel_B =  -5;

    //                            sec                mm                 mm/s 
    // ramp1.t0 = 0;    ramp1.t_end = 1;      ramp1.pos_A0 =   0;    ramp1.vel_A =   350;    ramp1.pos_B0 =   0;      ramp1.vel_B =   350;
    // ramp2.t0 = 1;    ramp2.t_end = 6;      ramp2.pos_A0 = 350;    ramp2.vel_A =     0;    ramp2.pos_B0 = 350;      ramp2.vel_B =     0; 
    // ramp3.t0 = 6;    ramp3.t_end = 7;      ramp3.pos_A0 = 375;    ramp3.vel_A =  -350;    ramp3.pos_B0 = 350;      ramp3.vel_B =  -350;

    // ramp_segment_coefficient_queue.put(ramp1);
    // ramp_segment_coefficient_queue.put(ramp2);
    // ramp_segment_coefficient_queue.put(ramp3);

    // //Method 2: XYSF
    coreXY_to_AB translator;
    XYSFvalues point1;      XYSFvalues point2;
    
    point1.X = -250;       point1.Y = 0;       point1.F = 500;
    point2.X =    0;       point2.Y = 0;       point2.F = 500;

    translator.translate_to_queue(point1);
    translator.translate_to_queue(point2);

    vTaskDelay(50);

    //Task for loop
    for(;;)
    {
        //Read off encoder position and velocity
        if(motor_choice == LASER_CUTTER_MOTOR_A || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            enc_A_output_share.get(enc_read_A);
            enc_read_A.pos = convert_units(enc_read_A.pos,ENC_POSITION_MODE_BELT_MM);
            enc_read_A.vel = convert_units(enc_read_A.vel,ENC_VELOCITY_MODE_BELT_MM_PER_SEC);
            // enc_read_A.pos = convert_units(enc_read_A.pos,ENC_POSITION_MODE_REVOUT);
            // enc_read_A.vel = convert_units(enc_read_A.vel,ENC_VELOCITY_MODE_RPMOUT);
        }
        if(motor_choice == LASER_CUTTER_MOTOR_B || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            enc_B_output_share.get(enc_read_B);
            enc_read_B.pos = convert_units(enc_read_B.pos,ENC_POSITION_MODE_BELT_MM);
            enc_read_B.vel = convert_units(enc_read_B.vel,ENC_VELOCITY_MODE_BELT_MM_PER_SEC);
            // enc_read_B.pos = convert_units(enc_read_B.pos,ENC_POSITION_MODE_REVOUT);
            // enc_read_B.vel = convert_units(enc_read_B.vel,ENC_VELOCITY_MODE_RPMOUT);
        }

        // Get the setpoint for this moment in time. 
        if(motor_choice == LASER_CUTTER_MOTOR_A || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            setpoint = xyoft.get_desired_pos_vel(enc_read_A.time);
        }
        else
        {
            setpoint = xyoft.get_desired_pos_vel(enc_read_B.time);
        }


        if(motor_choice == LASER_CUTTER_MOTOR_A || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            //Run Control Loop
            DC_A = control_A.Run_Control_Loop(enc_read_A.pos,setpoint.A_pos,enc_read_A.time);

            //Send command to the motor
            Motor_A.setDutyCycle(DC_A);
        }
        if(motor_choice == LASER_CUTTER_MOTOR_B || motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            //Run Control Loop
            DC_B = control_B.Run_Control_Loop(enc_read_B.pos,setpoint.B_pos,enc_read_B.time);

            //Send command to the motor
            Motor_B.setDutyCycle(DC_B);
        }


        //Print the encoder positions and velocity
        if(motor_choice == LASER_CUTTER_MOTOR_A)
        {
            print_serial("Position:  ");    print_serial(enc_read_A.pos);
            print_serial("  Velocity:  ");  print_serial(enc_read_A.vel);
            print_serial("  Error:  ");     print_serial(control_A.get_error());
            print_serial("  Time:  ");      print_serial(enc_read_A.time);
            print_serial("                                \r");
        }
        else if(motor_choice == LASER_CUTTER_MOTOR_B)
        {
            print_serial("Position:  ");    print_serial(enc_read_B.pos);
            print_serial("  Velocity:  ");  print_serial(enc_read_B.vel);
            print_serial("  Error:  ");     print_serial(control_B.get_error());
            print_serial("  Time:  ");      print_serial(enc_read_B.time);
            print_serial("                                \r");
        }
        else if(motor_choice == LASER_CUTTER_MOTOR_BOTH)
        {
            print_serial("Position: ");    print_serial(enc_read_A.pos);            print_serial(" (A)  ");  print_serial(enc_read_B.pos);          print_serial(" (B)  ");
            print_serial("  Velocity: ");  print_serial(enc_read_A.vel);            print_serial(" (A)  ");  print_serial(enc_read_B.vel);          print_serial(" (B)  ");
            print_serial("  Error:  ");    print_serial(control_A.get_error());     print_serial(" (A)  ");  print_serial(control_B.get_error());   print_serial(" (B)  ");
            print_serial("  Time:  ");     print_serial(enc_read_A.time);
            print_serial("                                \r");
        }
        
        if(motor_choice == LASER_CUTTER_MOTOR_BOTH){vTaskDelay(200);}
        else{vTaskDelay(50);}
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
        print_serial("  Error:  ");     print_serial(error);
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

