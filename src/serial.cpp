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


// // MOTOR A ENCODER DATA
// extern Queue<float> encoder_A_pos; 
// extern Queue<float> encoder_A_velocity;
// extern Queue<uint16_t> encoder_A_dt;

// // MOTOR B ENCODER DATA
// extern Queue<float> encoder_B_pos;
// extern Queue<float> encoder_B_velocity;
// extern Queue<uint16_t> encoder_B_dt;

extern Share<float> encoder_A_pos;
extern Share<float> encoder_A_velocity;
extern Share<uint32_t> encoder_A_dt;

// Shares for Encoder B
extern Share<float> encoder_B_pos;
extern Share<float> encoder_B_velocity;
extern Share<uint32_t> encoder_B_dt;


//---------------------------PYTHON SCRIPT COMMUNICATION FILES---------------------------

extern Queue<char[LINE_BUFFER_SIZE]> chars_to_print;
extern Queue<char[LINE_BUFFER_SIZE]> read_chars;


/** @brief      Task which reads the serial port and echos back what it sees.
 *  @details    This task reads an input into the serial port (from the python UI
 *              script presumably) and echos it back so the source knows what was sent. 
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

    //For testing
    #define TESTING

    //State variable to continue to read or not (if read queue gets close to full)
    uint8_t read_state = READY;
    //Sign that we need to update python that we're ready
    // bool update_input = true;

    //Turn off LED to start
    digitalWrite(LED_BUILTIN,LOW);

    //Task for loop
    for(;;)
    {
        switch (read_state)
        {
            // State WAITING means that the mc is waiting for python to tell it that it has something to send. 
            // Python will ask "Ready?" when it has something to send. 
            case READY:
                
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
                    read_chars.put(line);       //Put line data into the read_string

                    // line[strlen(line)] = '\n';       //Add a newline (\n) char to the end to signify to the python script that the line has ended
                    // print_serial(line);              //Temporarily echo

                    // Reset line and incomingByte for next time
                    memset(line,'\0',sizeof(line));
                    incomingByte = -1;

                    digitalWrite(LED_BUILTIN,LOW);  //Signal recieved, turn light off

                    //If the queue is filling and we aren't ready for more data: Go to NOT_READY state
                    if (read_chars.available() >= READ_Q_SIZE - PAUSE_Q_LIMIT)      //NOT ready: queue is close to full
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
                if (read_chars.available() < READ_Q_SIZE - PAUSE_Q_LIMIT)      //Ready to go; queue has enough space
                    {
                        read_state = READY;         //Switch state to READY
                    }
                break;

            //We should never get here, right?
            default:
                read_state = NOT_READY;

        } //Switch case for read_state
        






        // //Read ready state 1: read the serial port and send to the queue
        // if (read_ready == READY) 
        // {
        //     //Tell python that we're ready for data, if it think's we're not
        //     if (update_input)
        //     {
        //         digitalWrite(LED_BUILTIN,HIGH);     //Turn on LED: We're ready!
        //         print_serial("Ready\n");        //Send signal to python
        //         update_input = false;               //No need to update again for now!
        //     }

        //     //If there's something coming from python...
        //     if (Serial.available() > 0) 
        //     { 
        //         // read the incoming byte:
        //         incomingByte = Serial.read();

        //         //Add character to line
        //         line[strlen(line)] = (char)incomingByte;
        //     }

        //     //If we get the end of a line...
        //     if (incomingByte == int16_t('\0'))
        //     {
        //         //Put line data into the read_string
        //         read_chars.put(line);

        //         // line[strlen(line)] = '\n';       //Add a newline (\n) char to the end to signify to the python script that the line has ended
        //         // print_serial(line);              //Temporarily echo

        //         // Reset line and incomingByte for next time
        //         memset(line,'\0',sizeof(line));
        //         incomingByte = -1;

        //         //Signal to python that we're ready for more, and that we're not if we aren't
        //         if (read_chars.available() >= READ_Q_SIZE - PAUSE_Q_LIMIT)      //NOT ready: queue is close to full
        //         {
        //             digitalWrite(LED_BUILTIN,LOW);
        //             read_ready = WAIT;      //Set the state to WAIT
        //             print_serial("Wait\n");
        //         }
        //         else
        //         {
        //             print_serial("Ready\n");
        //         }
        //     }
        // }
        
        // //State Checker: Check if the queue is close to full, and tell python to stop sending code until there's space:
        // //If we're still waiting but we have some room in the queue, then start reading again!
        // if ( (read_chars.available() < READ_Q_SIZE - PAUSE_Q_LIMIT) & (read_ready == WAIT) )
        // {
        //     read_ready = READY;     //Switch to ready state
        //     update_input = true;    //Say that we need to update python of our status
        // }


        //testing mode: Not taking inputs from python
        #ifndef TESTING
        #define TESTING
        String test_line = "G1 X46.12 Y39.20 S1.00 F600";
        ///Set each character in static char @c line to match our test string, just like it 
        ///would in the parser (once that's finished)
        for (uint8_t char_counter = 0; char_counter<=test_line.length();  char_counter++)
        {
            line[char_counter] = test_line[char_counter];
        }
        // Put line data into the read_string
        read_chars.put(line);
        #endif //TESTING


        vTaskDelay(20);
    }
}




/** @brief      Task which prints any string that is sent to the strings_to_print queue. 
 *  @details    This task reads checks if there is anything in the strings_to_print queue, 
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
        if (chars_to_print.any())
        {
            //Get it from the queue...
            chars_to_print.get(print_string);
            
            //Then print it!
            Serial << print_string;
        }
        vTaskDelay(100);
    }

}



/** @brief      Task which loads the print queue with something to print
 *  @details    This function adds things to the printing queue which will be printed by
 *              the @c task_print_serial task function. The function is overloaded to 
 *              allow for a variety of input types.
 *  @param      p_params A pointer to function parameters which we don't use.
 */
//For strings
void print_serial(String printed_string)
{
    //create char array to print
        char char_print[LINE_BUFFER_SIZE];
    //set print char array to string of input string
        printed_string.toCharArray(char_print,LINE_BUFFER_SIZE);
    //Put into queue
        chars_to_print.put(char_print);
}

//For floats
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

//For uint8_t
void print_serial(uint8_t printed_int)
{
    //Get string of input character array
        String str_print = (String)printed_int;
    //create char array to print
        char char_print[LINE_BUFFER_SIZE];
    //set print char array to char of input string
        str_print.toCharArray(char_print,LINE_BUFFER_SIZE);
    //Put into queue
        chars_to_print.put(char_print);
}

//For character arrays
void print_serial(char *printed_char)
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

//For single chars
void print_serial(char printed_char)
{
    //create char array to print
        char char_print[LINE_BUFFER_SIZE];
    //Assign char to char array
        char_print[0] = printed_char;
    //Put into queue
        chars_to_print.put(char_print);
}

//For constant chars
void print_serial(const char* printed_char)
{
    //create char array to print
        char char_print[LINE_BUFFER_SIZE];
    //Assign const char* to char array
        strcpy(char_print,printed_char);
    //Put into queue
        chars_to_print.put(char_print);
}












//-----------------------MICROCONTROLLER UI FILES: FOR TESTING-------------------------------



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
        // Serial << "encA and B pos: " <<"      "<< enc_pos_out_A <<"      "<< enc_pos_out_B <<"              "<<enc_dt_out_A<<"              "<< enc_dt_out_B <<endl;
        // Serial << "encA and B vel: " <<"      "<< enc_vel_out_A <<"      "<< enc_vel_out_B <<"              "<<enc_dt_out_A<<"              "<< enc_dt_out_B <<endl;
        // Serial << "encA pos,vel,dt: " << enc_pos_out_A <<"         "<<enc_vel_out_A<< "              "<< enc_dt_out_A << endl;
        //Serial << "encB pos,vel,dt: " << enc_pos_out_B <<"         "<<enc_vel_out_B<< "              "<< enc_dt_out_B << endl;
        


        vTaskDelay(UI_period);

    }
}