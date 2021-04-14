/** @file       encoder_task.cpp 
 *  @brief      File that contains encoder task function file implementation.
 *  
 *  @author Ethan A Czuppa
 *  @author Matthew Carlson
 * 
 *  @date 22 Nov 2020 Created these files and started outlining the code structure.
 * 
 */


#include "libraries&constants.h"

///@cond
// Set up shares and queues

// // Queues for Encoder A
// // MOTOR A ENCODER DATA
// extern Queue<float> encoder_A_pos; 
// extern Queue<float> encoder_A_velocity;
// extern Queue<uint16_t> encoder_A_dt;

// // MOTOR B ENCODER DATA
// extern Queue<float> encoder_B_pos;
// extern Queue<float> encoder_B_velocity;
// extern Queue<uint16_t> encoder_B_dt;

// shares for Encoder A
extern Share<float> encoder_A_pos;
extern Share<float> encoder_A_velocity;
extern Share<uint32_t> encoder_A_dt;

// Shares for Encoder B
extern Share<float> encoder_B_pos;
extern Share<float> encoder_B_velocity;
extern Share<float> encoder_B_time;

///@endcond

// Encoder A Task

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
    StopWatch velTmrA(TIM5,PF_6);      // change to        

    // Create an instance of Quad_Encoder for Encoder A
    uint8_t enc_sigpin_AA = A_C1;           // PC6 = 34 on TIM8
    uint8_t enc_sigpin_AB = A_C2;           // PC7 = 9
    uint8_t enc_chan_AA = 1;
    uint8_t enc_chan_AB = 2;
    // TIM_TypeDef *a_p_eTIM = TIM8;        // for Encoder on Motor A
    int32_t bound_A = 1000;                 // default not changed 
    bool invert_A = false;                  // encoder pins are flipped on board, so inversion of read values is needed
    Quad_Encoder encoder_A (enc_sigpin_AA, enc_sigpin_AB, enc_chan_AA, enc_chan_AB, TIM8, bound_A, invert_A);        
    
    // Initialize motor encoder and timer
    encoder_A.enc_zero();
    //velTmrA.restart();
    
    // temporary variables for share data to control task
    float position_A;
    float velocity_A;
    uint32_t delta_time_A;
    float delta_position_A;
    

    for (;;)
    {
        // velTmrA.now_time();
        // get position, change in position, and change in time
        position_A = encoder_A.enc_read_pos();
        delta_time_A = velTmrA.lap();
        delta_position_A = encoder_A.enc_read_pos();
        
        // position pre-multiplied by the number of microseconds in one second to produce
        // velocity in mm/second
        velocity_A = (delta_position_A*1000000) /delta_time_A;         

        // put all those values into their respective queues. Used by the controller and for printing out (parameterization purposes)
        encoder_A_pos.put(position_A);
        // encoder_A_velocity.put(delta_position_A);
        encoder_A_velocity.put(velocity_A);
        encoder_A_dt.put(delta_time_A);
        
        // reset the stopwatch so the counter register is less likely to overflow when calculating velocity
        // velTmrA.temp_stop();
        
        vTaskDelayUntil(&xLastWakeTime, encoder_period_A);
    }

}




// Encoder B Task

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

    // Uses the StopWatch class to find out how much time elapses between encoder reads
    StopWatch velTmrB(TIM4,PD_12);     // DO NOT CHANGE!!!!!!!        

    // Create an instance of Quad_Encoder for Encoder B
    uint8_t enc_sigpin_BA = B_C1;           // PA8 = 7 on TIM1
    uint8_t enc_sigpin_BB = B_C2;           // PA9 = 8
    uint8_t enc_chan_BA = 2;
    uint8_t enc_chan_BB = 1;
    int32_t bound_B = 1000;                 // default not changed 
    bool invert_B = true;                   // encoder pins are flipped on board, so inversion of read values is needed
    // TIM_TypeDef *a_p_eTIM = TIM1         // Hardware Timer for Encoder on Motor B


    Quad_Encoder encoder_B (enc_sigpin_BA, enc_sigpin_BB, enc_chan_BA, enc_chan_BB, TIM1, bound_B, invert_B); 

    // Initialize motor encoder and timer
    encoder_B.enc_zero();

    // temporary variables collecting data from encoder and timer
    float position_B;
    uint32_t delta_time_B;
    int32_t delta_position_B;

    // temporary variables used to for calculations
    float velocity_B = 0;
    float raw_vel_B = 0;
    float total_time = 0;

    //Output variables for position and velocity
    float pos_B_out = 0;
    float vel_B_out = 0;

    print_serial("Encoder B initialized\n");
    
    for (;;)
    {        
        // get position, change in position, and change in time
        position_B = encoder_B.enc_read();
        delta_time_B = velTmrB.lap();
        delta_position_B = encoder_B.get_delta();

        //Update the total time (in microsec)
        total_time += delta_time_B;

        //Calculate velocity (encoder ticks/sec)
        raw_vel_B = (float)delta_position_B / (float)delta_time_B *   1000000;
                        // ticks            /       microsec      * microsec/sec 

        //Filter the raw velocity to get meaningful output
        //Why do we have to convert them to int32_t? No idea. But for whatever reason if I add these two
        //parts of the filter as floats, they lose all of their precision and add to 0 every time. Super frustrating.
        velocity_B = (float)( (int32_t)(raw_vel_B*FILTER_B_ALPHA*10000) + (int32_t)(velocity_B*(1-FILTER_B_ALPHA)*10000) )/10000; 

        //Convert values into desired units
        pos_B_out = convert_units(position_B,ENC_POSITION_MODE_BELT_MM);
        vel_B_out = convert_units(velocity_B,ENC_VELOCITY_MODE_RPMOUT);

        // Put all those values into their respective shares to be used in other functions
        encoder_B_pos.put(pos_B_out);
        encoder_B_velocity.put(vel_B_out);
        encoder_B_time.put(total_time/1000000);   //Converted to seconds

        // Delay until reset        
        vTaskDelayUntil(&xLastWakeTime, encoder_period_B);
    }

}


// ============================= SUBFUNCTIONS =============================

/** @brief   Convert units out of ticks or ticks/sec
 *  @details This function 
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