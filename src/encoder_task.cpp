/** @file encoder_task.cpp is the encoder task function file implementation.
 *        It reads values from the encoder class and sends a position, velocity
 *        delta time to the controller via queues.
 *  
 *  @author Ethan A Czuppa
 *  @author Matthew Carlson
 * 
 *  @date 22 Nov 2020 Created these files and started outlining the code structure.
 * 
 */


#include "libraries&constants.h"

// Set up shares and queues

// Queues for Encoder A
extern Queue<float> queue_pos_encoder_A;
extern Queue<float> queue_vel_encoder_A;
extern Queue<float> queue_delta_time_encoder_A;

// Queues for Encoder B
extern Queue<float> queue_pos_encoder_B;
extern Queue<float> queue_vel_encoder_B;
extern Queue<float> queue_delta_time_encoder_B;


// // Put this code into main or wherever we are defining the shares and queues this code

// Queue<float> queue_pos_encoder_A (600, "encoder position");
// Queue<float> queue_vel_encoder_A (600, "encoder velocity");
// Queue<float> queue_delta_time_encoder_A (600, "encoder delta time");

// Queue<float> queue_pos_encoder_B (600, "encoder position");
// Queue<float> queue_vel_encoder_B (600, "encoder velocity");
// Queue<float> queue_delta_time_encoder_B (600, "encoder delta time");
// // ?!?!?!?! CHECK THE "600" NUMBER... this is for how big the queue is, does it need 
// // to be bigger? Probably not but maybe.


// Encoder A Task

/** @brief   Task to run the left encoder, Encoder A (Motor 1 Encoder) 
 *  @details This task will interface with the Quad_Encoder class in order
 *           to read values from Encoder A. It will use methods established in
 *           the Quad_Encoder class to send values, via queues, of position, 
 *           velocity, and delta time to the controller.
 * 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void encoder_A_task (void* p_params)
{
    uint8_t enc_sigpin_A = PC7;             // PC7 = 9
    uint8_t enc_sigpin_B = PC6;             // PC6 = 34
    // TIM_TypeDef *a_p_eTIM = TIM8;

    // Uses the StopWatch class to find out how much time elapses between reads
    StopWatch velTmr(TIM2,PA0);             // ????? Will these values change per encoder ??!?!

    // Create an instance of Quad_Encoder for Encoder A
    Quad_Encoder encoder_A (enc_sigpin_A, enc_sigpin_B, TIM8);        // ???? Confirm the order of enc_sigpin_A and "  "_B
    encoder_A.enc_zero();
    velTmr.restart();
    TickType_t xLastWakeTime = xTaskGetTickCount();
    float position_A;
    float velocity_A;
    int32_t delta_time_A;
    //Serial << LeftEncoder.get_overflow() << LeftEncoder.get_prescale() << endl;

    for (;;)
    {
        // get position, velocity, and change in time from the encoder
        position_A = encoder_A.enc_read_pos();
        delta_time_A = velTmr.elapsed_time();
        float delta_position = encoder_A.enc_d_pos();
        velocity_A = (delta_position*1000000) /delta_time_A;           // CHECK THIS 1000000 NUMBER IS CORRECT

        // velocity_A = (encoder_A.enc_d_pos()*1000000) /delta_time_A;           // This is how it was before but I added another variable to make it more readable
        


        // put all those values into their respective queues. Used by the controller and for printing out (parameterization purposes)
        queue_pos_encoder_A.put(position_A);
        queue_vel_encoder_A.put(velocity_A);
        queue_delta_time_encoder_A.put(delta_time_A);
        velTmr.restart();
        vTaskDelayUntil(&xLastWakeTime, encoder_period_A);
    }

}




// Encoder B Task

/** @brief   Task to run the right encoder, Encoder B (Motor 2 Encoder) 
 *  @details This task will interface with the Quad_Encoder class in order
 *           to read values from Encoder B. It will use methods established in
 *           the Quad_Encoder class to send values, via queues, of position, 
 *           velocity, and delta time to the controller.
 * 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void encoder_B_task (void* p_params)
{
    uint8_t enc_sigpin_A = PA9;             // PC7 = 8
    uint8_t enc_sigpin_B = PA8;             // PC6 = 7
    // TIM_TypeDef *a_p_eTIM = TIM8;

    // Uses the StopWatch class to find out how much time elapses between reads
    StopWatch velTmr(TIM2,PA0);             // ????? Will these values change per encoder ??!?!

    // Create an instance of Quad_Encoder for Encoder B
    Quad_Encoder encoder_B (enc_sigpin_A, enc_sigpin_B, TIM8);        // ???? Confirm the order of enc_sigpin_A and "  "_B
    encoder_B.enc_zero();
    velTmr.restart();
    TickType_t xLastWakeTime = xTaskGetTickCount();
    float position_B;
    float velocity_B;
    int32_t delta_time_B;
    //Serial << LeftEncoder.get_overflow() << LeftEncoder.get_prescale() << endl;

    for (;;)
    {
        // get position, velocity, and change in time from the encoder
        position_B = encoder_B.enc_read_pos();
        delta_time_B = velTmr.elapsed_time();
        float delta_position = encoder_B.enc_d_pos();
        velocity_B = (delta_position*1000000) /delta_time_B;           // CHECK THIS 1000000 NUMBER IS CORRECT

        // velocity_B = (encoder_B.enc_d_pos()*1000000) /delta_time_B;           // This is how it was before but I added another variable to make it more readable
        


        // put all those values into their respective queues. Used by the controller and for printing out (parameterization purposes)
        queue_pos_encoder_B.put(position_B);
        queue_vel_encoder_B.put(velocity_B);
        queue_delta_time_encoder_B.put(delta_time_B);
        velTmr.restart();
        vTaskDelayUntil(&xLastWakeTime, encoder_period_B);
    }

}