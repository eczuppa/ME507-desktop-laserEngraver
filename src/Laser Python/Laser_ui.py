# Import Libraries and define constants

print('\nLaser Program Initializing')
print('Importing libraries...')

#Standard Libraries:
import sys
import os
import serial

#Other files with subfunctions:
import Preview_Gcode as gc
# import Argument_Parser as ap
import Serial_coms as sc
import Laser_print as lp


print('Library imports completed\n')


#Create Constants
SERIAL_DEVICE = '/dev/cu.usbmodem141403'
# SERIAL_DEVICE = '/dev/cu.usbmodem14203'


# TO CONNECT TO A DEVICE:
# Use this command in the terminal (on a mac at least) to see if the serial port is blocked, and by what:
#  lsof | grep "/dev/cu"
#
# Use this command in the terminal to find the serial port at which your device is located:
#   python3 -m  serial.tools.list_ports -v
# 
# Enter the device name into SERIAL_DEVICE below. Evetually we'll figure out how to do this automatically.


help_menu   = "Use these commands to run the laser cutter UI:\n"
help_menu  += "p, prnt     Print a .gcode file\n"
help_menu  += "h, help     Display help menu\n"
help_menu  += "e, exit     Exit program"






#--------------------------------------------------------------------------------------------
#--------------------------------------------------------------------------------------------

def main():

    #Initialize serial port connection with classconstructor
    print('Connecting to Serial Port on Laser...')
    ComPort = sc.serial_coms(SERIAL_DEVICE)
    print('Serial Port connected\n')

    running_UI = True   #As long as this is true, run the main UI loop. 

    # Main Loop for UI Program: Read commands, then execute them
    while running_UI:

        #Get input command from user
        input_command = input(">>")

        #Exit command
        if input_command == "e" or input_command == "exit":
            running_UI = False

        #Help command
        elif input_command == "h" or input_command == "help":
            print(help_menu)

        #Print file command
        elif input_command == "p" or input_command == "prnt":
            printer = lp.Laser_printer(ComPort)
            printer.preview_file()
            printer.send_file()

        #Invalid command
        else:
            print('"' + input_command + '" is an invalid Command. Choose one of the commands below:\n')
            print(help_menu)


        
    

if __name__ == "__main__":
    main()

#--------------------------------------------------------------------------------------------
#--------------------------------------------------------------------------------------------

