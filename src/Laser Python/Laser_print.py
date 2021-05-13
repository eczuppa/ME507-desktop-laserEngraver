#-----------------------------------------------------------------------------------------------
# This file defines functions that will run the UI to print a file to the laser cutter.
#-----------------------------------------------------------------------------------------------

import numpy as np

import Preview_Gcode as gc
import Serial_coms as sc
import time

class Laser_printer:
    def __init__(self,ComPort):
        #Get handle for class object for serial port
        self.ComPort = ComPort

        # Get filepath from user
        self.Gcode_Filepath = input("Enter filepath of .gcode file: ")

        if self.Gcode_Filepath == '':
            self.Gcode_Filepath = "/Users/Niko/Documents/Laser/Gcode Testing/Vector_Test.gcode"     #default value
        #Extract data from Gcode: 
        self.data = gc.read_gcode(self.Gcode_Filepath)


    def preview_file(self):
        # #Preview Gcode data:
        input_command = input('Do you want to preview the gcode? Y/N\n ')
        if input_command == 'Y' or input_command == 'y' or input_command == '':
            gc.preview_path(self.data)


    def send_file(self):
        input_command = input('Ok to send to Laser? Y/N')
        if input_command == 'Y' or input_command == 'y' or input_command == '':

            # Currently only writes line 10 of data; update this later to send all data, but we'll use 
            # this for testing
            # Data_to_write = self.data[17]
            # Data_to_write += '\0'           #termination char for microcontroller

            # print('Sending Gcode:',Data_to_write)
            # self.ComPort.write(Data_to_write)

            # print('Reading Data...')
            # read_val = self.ComPort.read()

            # print('Read Val:',read_val)
            # print(len(self.data))


            i = 0
            
            while  i <= 10:     #np.arange(0,len(self.data) 

                #Add comment displayer later

                #Ask the microcontroller if it's ready for data
                # print("Asking mc if it's ready")
                self.ComPort.write("Ready?\0")

                #Read the status of the mc:
                awaiting_response = True
                while awaiting_response:
                    # print("Waiting for response")
                    # time.sleep(0.100)
                    read_val = self.ComPort.read()
                    # print(read_val.encode())

                    # The microcontroller will send "Ready" when it can take a gcode line: 
                    # This line won't be reached until we get a ready response from the mc.
                    if read_val == 'Ready\n':
                        # print('Response Recived')
                        awaiting_response = False

                        line = self.data[i] #get line from data
                        line += '\0'        #Add null char to signal end of line for mc

                        #write the line to the mc and move i to the next line
                        time.sleep(0.010)
                        self.ComPort.write(line)
                        i += 1

                        #print the line so we can see what was just sent
                        # if line[0] == ';':
                            # print(line[1:len(line)])      
                        print(line[0:len(line)])                 

                # #If we're told to wait, just wait here until we get a new reading from read_val.
                # elif read_val == "Wait":
                #     #Stay in the while loop reading values until we get a "Ready" signal from the mc. 
                #     awaiting_response = True
                #     while awaiting_response:
                #         read_val = self.ComPort.read()
                #         if read_val == "Ready":
                #             awaiting_response = False


            
