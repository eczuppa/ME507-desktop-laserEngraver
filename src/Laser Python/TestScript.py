'''
    Import Libraries and define constants
'''
print('\nImporting libraries...')

import sys
import os
import numpy as np
import pandas as pd
import argparse
from configobj import ConfigObj
import datetime

import matplotlib.pyplot as plt
import matplotlib.patches as plotPatch
from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.lines import Line2D

import serial
from serial.tools import list_ports

print('Library imports completed\n')


# TO CONNECT TO A DEVICE:
# Use this command in the terminal (on a mac at least) to see if the serial port is blocked, and by what:
#  lsof | grep "/dev/cu"
#
# Use this command in the terminal to find the serial port at which your device is located:
#   python3 -m  serial.tools.list_ports -v
# 
# Enter the device name into SERIAL_DEVICE below. Evetually we'll figure out how to do this automatically.

# SERIAL_DEVICE = '/dev/cu.usbmodem141403'
SERIAL_DEVICE = '/dev/cu.usbmodem14203'



def main():

    print('Connecting to Serial Port on Laser...')
    ser = serial.Serial(SERIAL_DEVICE,
                        baudrate = 115200, 
                        timeout = 5)
    print('Serial Port connected\n')
    
    # print('Port Settings are:',ser.get_settings(),'\n')

    Data_to_write = 'G1 X46.12 Y39.20 S1.00 F600'

    Data_to_write += '\0'

    print('Writing Data:', Data_to_write)
    ser.write(Data_to_write.encode())


    print('Reading Data...')
    read_val = ser.read_until().decode()

    print('Read Val:',read_val)
    

if __name__ == "__main__":
    main()