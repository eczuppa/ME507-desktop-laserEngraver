/** @file serial.cpp
 *      This file defines functions that will print to and read the serial port in order to 
 *      communicate with the python script that sends the Gcode and general commands to the 
 *      laser. 
 *
 *  @author  Niko Banks
 *  @author  Ethan Czuppa
 *  @date    Nov 10 2020 
 * 
*/

#include "libraries&constants.h"

//Don't document this part
///@cond
//Shares and queues should go here
extern Queue<char[LINE_BUFFER_SIZE]> chars_to_print_queue;
extern Queue<char[LINE_BUFFER_SIZE]> read_chars_queue;

///@endcond


//---------------------------PYTHON SCRIPT COMMUNICATION FILES---------------------------



/** @brief      Task which reads the serial port and puts it in a queue.
 *  @details    This task reads an input into the serial port (from the python UI
 *              script presumably) and reads it. The task has 3 main states, which are READY,
 *              READING, and NOT_READY. The task will remain in READY initially until it is 
 *              asked "Ready?" through the serial port, in which case it will transition to
 *              READING to read the data and put it into the read_chars queue. If the read_chars
 *              queue is full, the task will go to the NOT_READY state until space has been cleared.
 *  @param      p_params A pointer to function parameters which we don't use.
 */
void task_read_serial(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning
    // Set the timeout for reading from the serial port to the maximum
    // possible value, essentially forever for a real-time control program
    Serial.setTimeout (0xFFFFFFFF);

    //incoming byte data variable (comes as bytes, need to translate to char)
    int16_t incomingByte = -1;

    // String line = "\0";
    char line[LINE_BUFFER_SIZE];
    //Set all values in line to null character
    memset(line,'\0',sizeof(line));

    ///@cond
    //For testing
    #define TESTING_WITHOUT_PYTHON
    bool line_one = 1;
    ///@endcond

    //State variable to continue to read or not (if read queue gets close to full)
    uint8_t read_state = READY;
    // bool state_state = true;
    //Sign that we need to update python that we're ready
    // bool update_input = true;

    //Turn off LED to start
    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,LOW);

    //Task for loop
    for(;;)
    {
        switch (read_state)
        {
            // State READY means that the mc is ready and waiting for python to tell it that it has something 
            // to send. Python will ask "Ready?" when it has something to send. 
            case READY:

                // if (state_state)
                // {
                //     print_serial("READY STATE");
                //     state_state = false;
                // }
                
                if (Serial.available() > 0)     //If there's something coming from python...
                { 
                    incomingByte = Serial.read();               //Read the incoming byte
                    line[strlen(line)] = (char)incomingByte;    //Add character to line
                }
                
                if (incomingByte == int16_t('\0'))      //If we get the end of a line...
                {
                    line[strlen(line)] = '\n';       //Add a newline (\n) char to the end to signify to the python script that the line has ended
                    // print_serial("LINE RECIEVED: \n");
                    // print_serial(line);              //Temporarily echo
                    if (strcmp(line,"Ready?\0"))
                    {
                        read_state = READING; //Change state to ready

                        // Reset line and incomingByte for next time
                        memset(line,'\0',sizeof(line));
                        incomingByte = -1;

                        digitalWrite(LED_BUILTIN,HIGH);     //Turn on LED: We're ready!
                        print_serial("Ready\n");            //Send signal to python that we're ready
                    }
                    //If line was somthing other than "Ready\0", then it wasn't intended for us. 
                    //For now, just reset line and start counting again.
                    else
                    {
                        // Reset line and incomingByte for next time
                        memset(line,'\0',sizeof(line));
                        incomingByte = -1;
                    }
                }
                break;

            // State READING is where a real input command line will be read. We transition to this state
            // after we get the "Ready?" command from the python script, and have told the python script
            // that we are indeed ready.
            case READING:
                
                if (Serial.available() > 0)         //If there's something coming from python...
                { 
                    incomingByte = Serial.read();               //Read the incoming byte
                    line[strlen(line)] = (char)incomingByte;    //Add character to line
                }

                if (incomingByte == int16_t('\0'))   //If we get the end of a line...
                {
                    read_chars_queue.put(line);       //Put line data into the read_string

                    // line[strlen(line)] = '\n';       //Add a newline (\n) char to the end to signify to the python script that the line has ended
                    // print_serial(line);              //Temporarily echo

                    // Reset line and incomingByte for next time
                    memset(line,'\0',sizeof(line));
                    incomingByte = -1;

                    digitalWrite(LED_BUILTIN,LOW);  //Signal recieved, turn light off

                    //If the queue is filling and we aren't ready for more data: Go to NOT_READY state
                    if (read_chars_queue.available() >= READ_Q_SIZE - READ_Q_PAUSE_LIMIT)      //NOT ready: queue is close to full
                    {
                        read_state = NOT_READY;         //Switch state to NOT_READY
                    }
                    //If the queue has no issues, go back to the READY state
                    else
                    {
                        read_state = READY;
                    }
                }   
                break;

            // State NOT_READY is a waiting sate that we'll sit in and effectively, do nothing. That is, until
            // the queue has opened up enough to where we can be ready again, in which case we'll go back to 
            // the READY state.
            case NOT_READY:
                if (read_chars_queue.available() < READ_Q_SIZE - READ_Q_PAUSE_LIMIT)      //Ready to go; queue has enough space
                    {
                        read_state = READY;         //Switch state to READY
                    }
                break;

            //We should never get here, right?
            default:
                read_state = NOT_READY;

        } //Switch case for read_state
        



        //testing mode: Not taking inputs from python (enable above)
        #ifdef TESTING_WITHOUT_PYTHON
            if (line_one)
            {
                String test_line = "G1 X46.12 Y39.20 S1.00 F600";
                // test_line = "$H";
                //Set each character in static char line to match our test string, just like it 
                //would in the parser (once that's finished)
                for (uint8_t char_counter = 0; char_counter<=test_line.length();  char_counter++)
                {
                    line[char_counter] = test_line[char_counter];
                }
                // Put line data into the read_string
                read_chars_queue.put(line);
                line_one = false;
            }
        #endif //TESTING_WITHOUT_PYTHON

        //Task delay
        vTaskDelay(2);
    }
}



/** @brief      Task which prints any string that is sent to the chars_to_print queue. 
 *  @details    This task reads checks if there is anything in the chars_to_print queue, 
 *              and if there is something, it prints it to the serial port. 
 *  @param      p_params A pointer to function parameters which we don't use.
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
        if (chars_to_print_queue.any())
        {
            //Get it from the queue...
            chars_to_print_queue.get(print_string);
            
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
 * 
 *  @overload  void print_serial(float printed_float)
 *  @overload  void print_serial(uint8_t printed_int)
 *  @overload  void print_serial(char *printed_char)
 *  @overload  void print_serial(char printed_char)
 *  @overload  void print_serial(const char* printed_char)
 */
//For strings
void print_serial(String printed_string)
{
    //create char array to print
        char char_print[LINE_BUFFER_SIZE] = "";
    //set print char array to string of input string
        printed_string.toCharArray(char_print,LINE_BUFFER_SIZE);
    //Put into queue
        chars_to_print_queue.put(char_print);
}

//For floats
void print_serial(float printed_float)
{
    //Get string of input character array
        String str_print = (String)printed_float;
    //create char array to print
        char char_print[LINE_BUFFER_SIZE] = "";
    //set print char array to char of input string
        str_print.toCharArray(char_print,LINE_BUFFER_SIZE);
    //Put into queue
        chars_to_print_queue.put(char_print);
}

//For uint8_t
void print_serial(uint8_t printed_int)
{
    //Get string of input character array
        String str_print = (String)printed_int;
    //create char array to print
        char char_print[LINE_BUFFER_SIZE] = "";
    //set print char array to char of input string
        str_print.toCharArray(char_print,LINE_BUFFER_SIZE);
    //Put into queue
        chars_to_print_queue.put(char_print);
}

//For character arrays
void print_serial(char *printed_char)
{
    //Get string of input character array
        String str_print = (String)printed_char;
    //create char array to print
        char char_print[LINE_BUFFER_SIZE] = "";
    //set print char array to char of string of input char array (redundant? yes. But it doesn't seem to work otherwise.)
        str_print.toCharArray(char_print,LINE_BUFFER_SIZE);
    //Put into queue
        chars_to_print_queue.put(char_print);
}

//For single chars
void print_serial(char printed_char)
{
    //create char array to print
        char char_print[LINE_BUFFER_SIZE] = "";
    //Assign char to char array
        char_print[0] = printed_char;
    //Put into queue
        chars_to_print_queue.put(char_print);
}

//For constant chars
void print_serial(const char* printed_char)
{
    //create char array to print
        char char_print[LINE_BUFFER_SIZE] = "";
    //Assign const char* to char array
        strcpy(char_print,printed_char);
    //Put into queue
        chars_to_print_queue.put(char_print);
}












//-----------------------MICROCONTROLLER UI FILES: FOR TESTING-------------------------------


///@cond
Share<int32_t> s_duty_cycle ("Power");
// REMOVE THESE SHARES ONCE ENCODER TESTING IS NOT REQUIRED
// Shares for Encoder A
extern Share<float> encoder_A_pos;
extern Share<float> encoder_A_velocity;
extern Share<uint32_t> encoder_A_dt;

// Shares for Encoder B
extern Share<float> encoder_B_pos;
extern Share<float> encoder_B_velocity;
extern Share<uint32_t> encoder_B_dt;
///@endcond

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
 * 
 *           @b NOTE: This function is only used for testing. 
 *           
 *  @param   stream The serial device such as @c Serial used to communicate
 */

int32_t parseIntWithEcho (Stream& stream)
{
    const uint8_t MAX_INT_DIGITS_PIWE = 24;        // More than a 64-bit integer has
    char ch_in = 0;                           // One character from the buffer
    char in_buf[MAX_INT_DIGITS_PIWE];              // Character buffer for input
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
        if (ch_in == '\n' || count >= (MAX_INT_DIGITS_PIWE - 1))
        {
            in_buf[count] = '\0';             // String must have a \0 at end
            return atoi (in_buf);
        }
    }
}


/** @brief   Task which interacts with a user. 
 *  @details This task demonstrates how to use a FreeRTOS task for interacting
 *           with some user while other more important things are going on.
 *           @b NOTE: This function is only used for testing. 
 * 
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_ui (void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning
    // int32_t user_power;               // UI task's variable that is put into the share, duty_cycle
    float enc_pos_out_A;
    float enc_vel_out_A;
    uint32_t enc_dt_out_A;

    float enc_pos_out_B;
    float enc_vel_out_B;
    uint32_t enc_dt_out_B;

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
        encoder_A_pos.get(enc_pos_out_A);
        encoder_A_velocity.get(enc_vel_out_A);
        encoder_A_dt.get(enc_dt_out_A);

        encoder_B_pos.get(enc_pos_out_B);
        encoder_B_velocity.get(enc_vel_out_B);
        encoder_B_dt.get(enc_dt_out_B);
       
        // if (!encoder_A_pos.is_empty())
        // {
        //     encoder_A_pos.get(enc_pos_out_A);
        //     encoder_A_velocity.get(enc_vel_out_A);
        //     encoder_A_dt.get(enc_dt_out_A);
        // }

        // if (!encoder_B_pos.is_empty())
        // {
        //     encoder_B_pos.get(enc_pos_out_B);
        //     encoder_B_velocity.get(enc_vel_out_B);
        //     encoder_B_dt.get(enc_dt_out_B);
        // }
        
        // encoder_B_pos.get(enc_pos_out_B);
        // encoder_B_velocity.get(enc_vel_out_B);
        // encoder_B_dt.get(enc_dt_out_B);

        // Serial << enc_vel_out << endl;
        Serial << "encA and B pos: " <<"      "<< enc_pos_out_A <<"      "<< enc_pos_out_B <<"              "<<enc_dt_out_A<<"              "<< enc_dt_out_B <<endl;
        // Serial << "encA and B vel: " <<"      "<< enc_vel_out_A <<"      "<< enc_vel_out_B <<"              "<<enc_dt_out_A<<"              "<< enc_dt_out_B <<endl;
        // Serial << "encA pos,vel,dt: " << enc_pos_out_A <<"         "<<enc_vel_out_A<< "              "<< enc_dt_out_A << endl;
        //Serial << "encB pos,vel,dt: " << enc_pos_out_B <<"         "<<enc_vel_out_B<< "              "<< enc_dt_out_B << endl;
        


        vTaskDelay(UI_period);

    }
}