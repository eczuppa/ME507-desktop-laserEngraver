/** @file serial.cpp
 *      This file defines functions that will print to and read the serial port in order to 
 *      communicate with the python script that sends the Gcode to the laser. 
 *
 *  @author  Niko Banks
 *  @author  Ethan Czuppa
 *  @date    Nov 10 2020 
 * 
*/
#include "libraries&constants.h"


//-----------------------MICROCONTROLLER UI FILES: FOR TESTING-------------------------------

extern Queue<float> encoder_queue;
Share<int32_t> s_duty_cycle ("Power");

/** @brief   Read an integer from a serial device, echoing input and blocking.
 *  @details This function reads an integer which is typed by a user into a
 *           serial device. It uses the Arduino function @c readBytes(), which
 *           blocks the task which calls this function until a character is
 *           read. When any character is received, it is echoed through the
 *           serial port so the user can see what was typed. Only decimal
 *           integers are supported; negative integers beginning with a single
 *           @c - sign or positive ones with a @c + will work. 
 * 
 *           @b NOTE: The serial device must have its timeout set to a very
 *           long time, or this function will malfunction. A recommended call:
 *           @code
 *           Serial.setTimeout (0xFFFFFFFF);
 *           @endcode
 *           Assuming that the serial port named @c Serial is being used.
 *  @param   stream The serial device such as @c Serial used to communicate
 */

int32_t parseIntWithEcho (Stream& stream)
{
    const uint8_t MAX_INT_DIGITS = 24;        // More than a 64-bit integer has
    char ch_in = 0;                           // One character from the buffer
    char in_buf[MAX_INT_DIGITS];              // Character buffer for input
    uint8_t count = 0;                        // Counts characters received

    // Read until return is received or too many characters have been read.
    // The readBytes function blocks while waiting for characters
    while (true)
    {
        stream.readBytes (&ch_in, 1);         // Read (and wait for) characters
        in_buf[count++] = ch_in;
        stream.print (ch_in);                 // Echo the character
        if (ch_in == '\b' && count)           // If a backspace, back up one
        {                                     // character and try again
            count -= 2;
        }
        if (ch_in == '\n' || count >= (MAX_INT_DIGITS - 1))
        {
            in_buf[count] = '\0';             // String must have a \0 at end
            return atoi (in_buf);
        }
    }
}


/** @brief   Task which interacts with a user. 
 *  @details This task demonstrates how to use a FreeRTOS task for interacting
 *           with some user while other more important things are going on.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_ui (void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning
    //int32_t user_power;               // UI task's variable that is put into the share, duty_cycle
    float enc_pos_out;

    // Set the timeout for reading from the serial port to the maximum
    // possible value, essentially forever for a real-time control program
    Serial.setTimeout (0xFFFFFFFF);
    //Serial << "I am ready for input" << endl;

    // The task's infinite loop goes here
    for (;;)
    {
        // // Ask for User input in a desired range
        // Serial << "Enter desired Duty Cycle (+/-100%): " << endl;
        // // Use parseIntWithEcho to get integer entered into serial monitor
        // user_power = parseIntWithEcho(Serial);
        // // Take output of parseIntWithEcho and put in share duty_cycle
        // s_duty_cycle.put(user_power);

        // Print Current Encoder Position
        
        if (!encoder_queue.is_empty())
        {
            encoder_queue.get(enc_pos_out);
        }
        Serial << "The current Encoder position in mm is: " << enc_pos_out << endl;


        vTaskDelay(UI_period);

    }
}

//---------------------------PYTHON SCRIPT COMMUNICATION FILES---------------------------

extern Queue<String> string_to_print;


/** @brief      Task which reads the serial port and echos back what it sees.
 *  @details    This task reads an input into the serial port (from the python UI
 *              script presumably) and echos it back so the source knows what was sent. 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void echo_serial(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning
    // Set the timeout for reading from the serial port to the maximum
    // possible value, essentially forever for a real-time control program
    // Serial.setTimeout (0xFFFFFFFF);

    char incomingByte[64]; // for incoming serial data

    // strcpy(incomingByte,"G1 X46.12 Y39.20 S1.00 F600");

    // Serial << strlen(incomingByte);

    for(;;)
    {
        if (Serial.available() > 0) 
        {
            //Clear incomingByte char array before entering new data
            memset(incomingByte, '\0', strlen(incomingByte)*sizeof(char)); 
            
            // read the incoming byte:
            Serial.readBytes(incomingByte,64);

            incomingByte[strlen(incomingByte)] = '\n';

            // Echo what you got:
            Serial << incomingByte;
            
        }
        vTaskDelay(100);
    }
}




/** @brief      Task which reads the serial port and echos back what it sees.
 *  @details    This task reads an input into the serial port (from the python UI
 *              script presumably) and echos it back so the source knows what was sent. 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void print_serial(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning
    // Set the timeout for reading from the serial port to the maximum
    // possible value, essentially forever for a real-time control program
    Serial.setTimeout (0xFFFFFFFF);

    //Work in progress...


}

