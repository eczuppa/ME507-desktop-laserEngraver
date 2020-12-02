/** @file       commands.cpp
 *  @brief      File containing functions that interpret machine and G-code commands.
 *  @details    This file defines the functions necessary to run the full translation of commands coming from the 
 *              serial port before sending them to the control task.
 *
 *  @author  Niko Banks
 *  @date    Nov 10 2020 
 * 
*/

#include "libraries&constants.h"

//Don't document this part
///@cond
//Shares and queues should go here

// Queue to hold desired positions and feedrates of motors A and B
Queue<desired_pos_vel_S> desired_queue(DES_Q_SIZE,"Desired A+B Position and Feeds");

// Share to signal for homing sequence or not
Share<bool> check_home ("Homing Flag");

//Queue that holds read character arrays
extern Queue<char[LINE_BUFFER_SIZE]> read_chars;

///@endcond


/** @brief      Task which reads data from the serial port, translates it, and sends it where it needs to go.
 *  @details    This task function reads a line from the read_chars queue of gcode or other commands 
 *              and splits it up into the separate commands. Commands are then sent to the control task via queues
 *              and shares. Gcode is translated from @c X @c Y and @c F values into desired positions and feedrates 
 *              for both motors; @c _A_setpoint, @c _A_feed, @c _B_setpoint and @c _B_feed, which are class member 
 *              data of the class @c Kinematics_coreXY. Regular machine commands such as homing are also translated 
 *              to motor commands, which are then sent to the control task so it may run them. 
 * 
 *  @param      p_params A pointer to function parameters which we don't use.
 */
void task_translate(void* p_params)
{
    (void)p_params;                   // Does nothing but shut up a compiler warning
    // Set the timeout for reading from the serial port to the maximum
    // possible value, essentially forever for a real-time control program
    Serial.setTimeout (0xFFFFFFFF);

    //Initialize translator and decoder class members 
    Kinematics_coreXY translator;
    decode decoder;

    //Create instance of desired struct for passing into the queue
    desired_pos_vel_S desired;

    //Create line char array to hold incoming data from the @c read_chars queue
    char line[LINE_BUFFER_SIZE];

    //Create empty machine_cmd state variable
    uint8_t machine_cmd = MACHINE_CMD_NULL;

    //Reset homing_flag to false
    check_home.put(false);

    for(;;)
    {   
        //If we have space in the queues for A and B positions and feedrates...
        // if 

        //If there is a read line of gcode/commands in the queue...
        if (read_chars.any())
        {
            //Read the line
            read_chars.get(line);

            //if the line is actually a machine command:
            if (line[0] == '$')
            {
                machine_cmd = interpret_machinecmd_line(line);
                switch (machine_cmd)
                {
                    //Error in command: Command not supported
                    case MACHINE_CMD_NULL:
                        //Print an error?
                        break;
                    //Go into homing cycle
                    case MACHINE_CMD_HOME:
                        check_home.put(true);
                        break;
                    default:
                        //Shouldn't get here, hopefully
                        break;
                };
            }
            //If we didn't get into a machine command, it must be a gcode command!
            else
            {
                //Interpret the line as gcode (data is stored in decoder class)
                decoder.interpret_gcode_line(line);
                //update kinematic translator with new values
                translator.calculate_kinematics(decoder);

                //Put translated items into struct for clean transfer
                desired.A_pos = translator.get_A_setpoint();
                desired.B_pos = translator.get_B_setpoint();
                desired.A_feed = translator.get_F_A();
                desired.B_feed = translator.get_F_B();

                //Put laser power into desired struct
                desired.S = decoder.get_S();

                //Enter struct into the queue for desired positions, feedrates, and laser power
                desired_queue.put(desired);
            }
        }
        vTaskDelay(100);
    }
}


/** @brief      Function which interprets a line containing a machine command.
 *  @details    This function takes in a line containing a command for the laser that begins with
 *              a @c $, signalling that it is a machine command and not a line of gcode. It then interprets
 *              the command in the line and returns on the information. 
 *              <b>Disclaimer: This function currently only supports homing commands. More will be added in the 
 *              future.</b>
 * 
 *  @param      line A line containing a command to be interpreted. 
 *  @returns    an indicator for the command that was entered
 */
uint8_t interpret_machinecmd_line(char *line)
{
    uint8_t cmd_indicator = MACHINE_CMD_NULL;
    //Homing Command
    if (strcmp(line,"$H") == 0)
    {
        // print_serial("\nFOUND HOME CMD\n");
        cmd_indicator = MACHINE_CMD_HOME;
    }
    //Unsupported command
    else
    {
        cmd_indicator = MACHINE_CMD_NULL;
    }
    
    
    return cmd_indicator;
}