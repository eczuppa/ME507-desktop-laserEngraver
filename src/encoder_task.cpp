/** @file       encoder_task.cpp 
 *  @brief      File that contains encoder task function file implementation.
 *  
 *  @author Ethan A Czuppa
 *  @author Matthew Carlson
 *  @author Niko Banks
 * 
 *  @date 22 Nov 2020 Created these files and started outlining the code structure.
 *  @date 13 Apr 2021 Updated and fully functional
 * 
 */


#include "libraries&constants.h"

///@cond 
//Set up shares and queues

// Shares for Encoder A and B
extern Share<encoder_output> enc_A_output_share;
extern Share<encoder_output> enc_B_output_share;

// Shares for timing mode
extern Share<uint8_t> timing_mode_share;

///@endcond





// ========================================= Encoder A Task ========================================= 


/** @brief   Task to run the left encoder, Encoder A (Motor 1 Encoder) 
 *  @details This task will interface with the Quad_Encoder class in order
 *           to read values from Encoder A. It will use methods established in
 *           the Quad_Encoder class to send values, via queues, of position, 
 *           velocity, and delta time to the controller.
 * 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_encoder_A (void* p_params)
{
    // Account for the length of time it takes to run the task in the task timing requirement
    TickType_t xLastWakeTime = xTaskGetTickCount();

    // Uses the StopWatch class to find out how much time elapses between reads
    StopWatch velTmrA(TIM7,NC);      // change to        

    // Create an instance of Quad_Encoder for Encoder A
    uint8_t enc_sigpin_AA = A_C1;           // PC6 = 33 on TIM8
    uint8_t enc_sigpin_AB = A_C2;           // PC7 = 9
    uint8_t enc_chan_AA = 1;
    uint8_t enc_chan_AB = 2;
    int32_t bound_A = 1000;                 // default not changed 
    bool invert_A = false;                  // encoder pins are flipped on board, so inversion of read values is needed
    // TIM_TypeDef *a_p_eTIM = TIM8;        // for Encoder on Motor A
    
    Quad_Encoder encoder_A (enc_sigpin_AA, enc_sigpin_AB, enc_chan_AA, enc_chan_AB, TIM3, bound_A, invert_A);        
    
    // Initialize motor encoder and timer
    encoder_A.enc_zero();

    // temporary variables collecting data from encoder and timer
    float position_A;
    uint32_t delta_time_A;
    int32_t delta_position_A;

    // temporary variables used to for calculations
    float velocity_A = 0;
    float raw_vel_A = 0;

    //Output variables for position and velocity
    float pos_A_out = 0;
    float vel_A_out = 0;
    float total_time = 0;
    encoder_output enc_A;

    print_serial("Encoder A initialized\n");

    for (;;)
    {
        // get position, change in position, and change in time
        position_A = encoder_A.enc_read();
        delta_time_A = velTmrA.lap();
        delta_position_A = encoder_A.get_delta();

        //Calculate velocity (encoder ticks/sec)
        raw_vel_A = (float)delta_position_A / (float)delta_time_A *   1000000;
                        // ticks            /       microsec      * microsec/sec 

        //Filter the raw velocity to get meaningful output
        //Why do we have to convert them to int32_t? No idea. But for whatever reason if I add these two
        //parts of the filter as floats, they lose all of their precision and add to 0 every time. Super frustrating.
        velocity_A = (float)( (int32_t)(raw_vel_A*FILTER_A_ALPHA*10000) + (int32_t)(velocity_A*(1-FILTER_A_ALPHA)*10000) )/10000; 

        //Convert values into desired units
        pos_A_out = convert_units(position_A,ENC_POSITION_MODE_TICKS);
        vel_A_out = convert_units(velocity_A,ENC_VELOCITY_MODE_TICKS_PER_SEC);

        //Update encoder timing (affects how ramps are interpreted)
        total_time = update_total_time(total_time,delta_time_A);

        // Put all those values into their respective shares to be used in other functions
        enc_A.pos  = pos_A_out;
        enc_A.vel  = vel_A_out;
        enc_A.time = total_time;

        // print_serial(position_A); print_serial('\n');
        enc_A_output_share.put(enc_A);


        // Delay until reset        
        vTaskDelayUntil(&xLastWakeTime, ENCODER_PERIOD_A);
    }

}







// ========================================= Encoder B Task ========================================= 


/** @brief   Task to run the right encoder, Encoder B (Motor 2 Encoder) 
 *  @details This task will interface with the Quad_Encoder class in order
 *           to read values from Encoder B. It will use methods established in
 *           the Quad_Encoder class to send values, via shares, of position, 
 *           velocity, and delta time to the controller.
 * 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_encoder_B (void* p_params)
{
    // Account for the length of time it takes to run the task in the task timing requirement
    TickType_t xLastWakeTime = xTaskGetTickCount();

    // Create a StopWatch class to find out how much time elapses between encoder reads
    // StopWatch velTmrB(TIM4,PD_12);
    StopWatch velTmrB(TIM6,NC);
    // StopWatch velTmrB(TIM16,PE_0);


    // Create an instance of Quad_Encoder for Encoder B

    uint8_t enc_sigpin_BA = B_C1;           // PA8 = 7 on TIM1
    uint8_t enc_sigpin_BB = B_C2;           // PA9 = 8
    uint8_t enc_chan_BA = 2;
    uint8_t enc_chan_BB = 1;
    int32_t bound_B = 1000;                 // default not changed 
    bool invert_B = true;                   // encoder pins are flipped on board, so inversion of read values is needed
    // TIM_TypeDef *a_p_eTIM = TIM1         // Hardware Timer for Encoder on Motor B


    Quad_Encoder encoder_B (enc_sigpin_BA, enc_sigpin_BB, enc_chan_BA, enc_chan_BB, TIM1, bound_B, invert_B); 

    // Initialize motor encoder
    encoder_B.enc_zero();

    // temporary variables collecting data from encoder and timer
    float position_B;
    uint32_t delta_time_B;
    int32_t delta_position_B;

    // temporary variables used to for calculations
    float velocity_B = 0;
    float raw_vel_B = 0;

    //Output variables for position and velocity
    float pos_B_out = 0;
    float vel_B_out = 0;
    float total_time = 0;
    encoder_output enc_B;

    print_serial("Encoder B initialized\n");
    
    for (;;)
    {        
        // get position, change in position, and change in time
        position_B = encoder_B.enc_read();
        delta_time_B = velTmrB.lap();
        delta_position_B = encoder_B.get_delta();

        //Calculate velocity (encoder ticks/sec)
        raw_vel_B = (float)delta_position_B / (float)delta_time_B *   1000000;
                        // ticks            /       microsec      * microsec/sec 

        //Filter the raw velocity to get meaningful output
        //Why do we have to convert them to int32_t? No idea. But for whatever reason if I add these two
        //parts of the filter as floats, they lose all of their precision and add to 0 every time. Super frustrating.
        velocity_B = (float)( (int32_t)(raw_vel_B*FILTER_B_ALPHA*10000) + (int32_t)(velocity_B*(1-FILTER_B_ALPHA)*10000) )/10000; 

        //Convert values into desired units
        pos_B_out = convert_units(position_B,ENC_POSITION_MODE_TICKS);
        vel_B_out = convert_units(velocity_B,ENC_VELOCITY_MODE_TICKS_PER_SEC);

        //Update encoder timing (affects how ramps are interpreted)
        total_time = update_total_time(total_time,delta_time_B);

        // Put all those values into their respective shares to be used in other functions
        enc_B.pos  = pos_B_out;
        enc_B.vel  = vel_B_out;
        enc_B.time = total_time;

        enc_B_output_share.put(enc_B);

        // Delay until reset        
        vTaskDelayUntil(&xLastWakeTime, ENCODER_PERIOD_B);
    }

}




// ========================================= SUBFUNCTIONS ========================================= 


/** @brief   Convert units out of ticks or ticks/sec
 *  @details This function converts the units for the encoder into the desired units of choice. It is always assumed that the 
 *           initial value is in units of encoder ticks.
 * 
 *  @param   value The value whose units are to be converted
 *  @param   convert_mode The mode in which to convert the units
 * 
 *  @returns converted value
 */
float convert_units(float value, uint8_t convert_mode)
{
    switch (convert_mode)
    {
        case ENC_POSITION_MODE_TICKS:           //starting with ticks
        case ENC_VELOCITY_MODE_TICKS_PER_SEC:   //starting with ticks/sec
            break; //No conversion necessary
        case ENC_POSITION_MODE_REVOUT:
            //      enc ticks / (      (ticks/pulse)      *    (pulse/enc rev) )    / (enc rev/output rev)
            value =   value   / (ENCODER_COUNTS_PER_PULSE * ENCODER_PULSES_PER_REV) / REV_ENC_PER_REVOUT_MOTOR;
            break;

        case ENC_VELOCITY_MODE_RPMOUT:
            //      enc ticks/sec / (      (ticks/pulse)      *    (pulse/enc rev) )    / (enc rev/output rev)     * sec/min
            value =   value       / (ENCODER_COUNTS_PER_PULSE * ENCODER_PULSES_PER_REV) / REV_ENC_PER_REVOUT_MOTOR * 60;
            break;

        case ENC_POSITION_MODE_DEGOUT:         //starting with ticks
        case ENC_VELOCITY_MODE_DEGOUT_PER_SEC: //starting with ticks/sec
            //      enc ticks / (      (ticks/pulse)      *    (pulse/enc rev) )    / (enc rev/output rev)      * 360deg/rev
            value =   value   / (ENCODER_COUNTS_PER_PULSE * ENCODER_PULSES_PER_REV) / REV_ENC_PER_REVOUT_MOTOR  * 360;
            break;

        case ENC_POSITION_MODE_BELT_MM:             //starting with ticks
        case ENC_VELOCITY_MODE_BELT_MM_PER_SEC:     //starting with ticks/sec
            // Convert to output revolutions fist:
            //      enc ticks / (      (ticks/pulse)      *    (pulse/enc rev) )    / (enc rev/output rev)
            value =   value   / (ENCODER_COUNTS_PER_PULSE * ENCODER_PULSES_PER_REV) / REV_ENC_PER_REVOUT_MOTOR;

            //Convert from output rev to belt distance: s = r*theta (theta in radians)
            //       rev *     2pi rad/rev     *         r (mm)
            value = value*(2*3.141592653589793)*OUTPUT_WHEEL_RADIUS_MM;
            break;

        default:
            break; //No conversion on default
    }
    
    return value;
}



/** @brief   Update the total time based on timing mode
 *  @details This function updates the total time based on what timing mode we're in. 
 * 
 *  @param   total_time     previous total time
 *  @param   delta_time     time between calls
 * 
 *  @returns updated total_time
 */
float update_total_time(float total_time, uint32_t delta_time)
{
    // Define variables to hold share values
    uint8_t timing_mode = TIMING_MODE_PAUSED;

    timing_mode_share.get(timing_mode);
    switch(timing_mode)
    {
        case TIMING_MODE_PAUSED:
            //Don't add to the total time; total time stays constant
            break;
        
        case TIMING_MODE_RESET:
            total_time = 0;     //Reset total time
            break;

        case TIMING_MODE_RUNNING:
        default:
            //Update the total time (in seconds)
            total_time += (float)delta_time/1000000;
            break;

    }
    return total_time;
}