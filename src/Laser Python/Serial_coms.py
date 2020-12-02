#-----------------------------------------------------------------------------------------------
# This file defines functions that will communicate with the [Laser] through the serial port.
#-----------------------------------------------------------------------------------------------


import serial
# from serial.tools import list_ports


# TO CONNECT TO A DEVICE:
# Use this command in the terminal (on a mac at least) to see if the serial port is blocked, and by what:
#  lsof | grep "/dev/cu"
#
# Use this command in the terminal to find the serial port at which your device is located:
#   python3 -m  serial.tools.list_ports -v
# 
# Enter the device name into SERIAL_DEVICE below. Evetually we'll figure out how to do this automatically.


class serial_coms:
    def __init__(self, Serial_Device):
        #Initialize connection with serial port in the constructor:
        self.ser = serial.Serial(Serial_Device,
                            baudrate = 115200, 
                            timeout = 5)

    def settings(self):
        #Get serial port settings
        settings = self.ser.get_settings()

        #Print the settings to the terminal
        # print('Port Settings are:',settings,'\n')

        return settings

    def write(self,Data_to_write):
        # Data_to_write = 'G1 X46.12 Y39.20 S1.00 F600'
        # print('Writing Data:', Data_to_write)

        #Write encoded data to the serial port
        self.ser.write(Data_to_write.encode())

    def read(self):
        # print('Reading Data...')

        # Read the decoded string from the serial port:
        read_val = self.ser.read_until().decode()

        return read_val

        # print('Read Val:',read_val)