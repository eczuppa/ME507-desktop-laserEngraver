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

//---------------------------PYTHON SCRIPT COMMUNICATION FILES---------------------------

extern Queue<char[LINE_BUFFER_SIZE]> chars_to_print;
extern Queue<char[LINE_BUFFER_SIZE]> read_chars;


/** @brief      Task which reads the serial port and echos back what it sees.
 *  @details    This task reads an input into the serial port (from the python UI
 *              script presumably) and echos it back so the source knows what was sent. 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_read_serial(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning
    // Set the timeout for reading from the serial port to the maximum
    // possible value, essentially forever for a real-time control program
    Serial.setTimeout (0xFFFFFFFF);

    // char incomingByte[64]; // for incoming serial data
    // String incomingByte;
    int16_t incomingByte = -1;
    // String line = "\0";
    char line[LINE_BUFFER_SIZE];

    //Set all valuew in line to null character
    memset(line,'\0',sizeof(line));

    for(;;)
    {
        //Set LED to off
        digitalWrite(LED_BUILTIN,LOW);

        if (Serial.available() > 0) 
        { 
            //Turn on light
            digitalWrite(LED_BUILTIN,HIGH);

            // read the incoming byte:
            incomingByte = Serial.read();

            //Add character to line
            line[strlen(line)] = (char)incomingByte;
            
            //Turn off light
            digitalWrite(LED_BUILTIN,HIGH);
        }
        //If we get the end of a line...
        // else if (line.length() != 0)
        if (incomingByte == int16_t('\0'))
        {
            //Put line data into the read_string
            read_chars.put(line);

            //Add a newline (\n) char to the end to signify to the python script that the line has ended
            line[strlen(line)] = '\n';

            //Temporarily echo
            print_serial(line);
            // Serial << line;

            // Reset line and incomingByte for next time
            memset(line,'\0',sizeof(line));
            incomingByte = -1;
        }
        vTaskDelay(2);
    }
}




/** @brief      Task which prints any string that is sent to the strings_to_print queue. 
 *  @details    This task reads checks if there is anything in the strings_to_print queue, 
 *              and if there is something, it prints it to the serial port. 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_print_serial(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning
    // Set the timeout for reading from the serial port to the maximum
    // possible value, essentially forever for a real-time control program
    Serial.setTimeout (0xFFFFFFFF);

    //Initialize string to print
    char print_string[LINE_BUFFER_SIZE];

    for(;;)
    {
        //When you get a string in the string_to_print queue...
        if (chars_to_print.any())
        {
            //Get it from the queue...
            chars_to_print.get(print_string);
            
            //Then print it!
            Serial << print_string;
        }
        vTaskDelay(10);
    }

}




/** @brief      Task which loads the print queue with something to print
 *  @details    This function adds things to the printing queue which will be printed by
 *              the @c task_print_serial task function. The function is overloaded to 
 *              allow for a variety of input types.
 *  @param      p_params A pointer to function parameters which we don't use.
 */
void print_serial(String printed_string)
{
    //create char array to print
        char char_print[LINE_BUFFER_SIZE];
    //set print char array to string of input string
        printed_string.toCharArray(char_print,LINE_BUFFER_SIZE);
    //Put into queue
        chars_to_print.put(char_print);
}

void print_serial(float printed_float)
{
    //Get string of input character array
        String str_print = (String)printed_float;
    //create char array to print
        char char_print[LINE_BUFFER_SIZE];
    //set print char array to char of input string
        str_print.toCharArray(char_print,LINE_BUFFER_SIZE);
    //Put into queue
        chars_to_print.put(char_print);
}

void print_serial(char printed_char[LINE_BUFFER_SIZE])
{
    //Get string of input character array
        String str_print = (String)printed_char;
    //create char array to print
        char char_print[LINE_BUFFER_SIZE];
    //set print char array to char of string of input char array (redundant? yes. But it doesn't seem to work otherwise.)
        str_print.toCharArray(char_print,LINE_BUFFER_SIZE);
    //Put into queue
        chars_to_print.put(char_print);
}










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
    // int32_t user_power;               // UI task's variable that is put into the share, duty_cycle
    float enc_pos_out;
    float enc_vel_out;
    float enc_dt_out;

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
            encoder_queue.get(enc_vel_out);
            encoder_queue.get(enc_dt_out);
        }
        // Serial << enc_vel_out << endl;
        Serial << "pos,vel,delta_time is: " << enc_pos_out <<"          "<< enc_vel_out << "            " << enc_dt_out <<endl;
        // Serial << "pos,vel,delta_time is: " <<"          "<< enc_vel_out << "            " << enc_dt_out <<endl;


        vTaskDelay(UI_period);

    }
}