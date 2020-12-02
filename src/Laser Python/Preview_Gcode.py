#-----------------------------------------------------------------------------------------------
# This file defines functions that will preview the gcode before sending it out to be printed.
#-----------------------------------------------------------------------------------------------


import numpy as np
import matplotlib.pyplot as plt


# This function reads a gcode file specified by Gcode_Filepath and returns list "data" where 
# each item in the list is a line of gcode.
def read_gcode(Gcode_Filepath):
    #Get data from gcode file:
    with open(Gcode_Filepath,'r') as f_gcode:
        data = f_gcode.read().splitlines()

    return data

#-----------------------------------------------------------------------------------------------

# This function takes the data from a full gcode file (data) and extracts the x and y 
# commands from that file to preview it before sending the data to the laser.
def extract_xy(data):
    
    # Set origin point
    coords = [[0,0]]

    # Set up variables for loop
    x_num_flag = bool(0)
    y_num_flag = bool(0)
    x_num = float(0)
    y_num = float(0)
    line_count = 0



    #Loop through each line
    for line in data:

        #If we have an x or y coordinate entry, loop through each letter
        if ('G0' in line) or ('G1' in line):

            #Add to line count: Should read index of Gcode movement line
            line_count += 1

            #Loop through each letter, and one more loop at the end of the line to convert floats:
            for letter_index in range(0,len(line)+1):

                #If at end of line, add a space (signify end of a number)
                if letter_index == len(line):
                    letter = ' '
                #Otherwise, index number as normal
                else:
                    letter = line[letter_index]

                #Save X assignment
                if x_num_flag == 1:
                    if letter.isdigit() or (letter == '.') or (letter == '-'):
                        X_str += letter
                    else:
                        x_num_flag = 0
                        x_num = float(X_str)

                #Save Y assignment
                if y_num_flag == 1:
                    if letter.isdigit() or (letter == '.') or (letter == '-'):
                        Y_str += letter
                    else:
                        y_num_flag = 0
                        y_num = float(Y_str)

                #Check beginning of x or y command
                if letter == 'X':
                    x_num_flag = 1
                    X_str = ''

                if letter == 'Y':
                    y_num_flag = 1
                    Y_str = ''

            #End of letter loop

            line_coords = np.array([[x_num,y_num]])
        
            coords = np.append(coords,line_coords, axis = 0)
            #print('Line segment appended')
        #end of G0 G1 command check

        #Now get an array of line segments for travel to plot in a different color:
        if 'G0' in line:
            travel_coords = np.array([coords[line_count-1,:]])
            travel_coords = np.append(travel_coords,line_coords, axis = 0)

            if 'travel_segments' in locals():
                travel_segments = np.append(travel_segments,np.expand_dims(travel_coords,axis = 0), axis = 0)
            else:
                travel_segments = np.expand_dims(travel_coords,axis = 0)
    #end of line loop

    return coords, travel_segments
    
#-----------------------------------------------------------------------------------------------


# This function creates a preview of the Gcode path which will be sent to the laser. 
def preview_path(data):

    # Get xy data from gcode
    coords, travel_segments = extract_xy(data)
    
    #Create plot
    plt.figure(figsize = (11.5,8))
    plt.gca().set_aspect('equal', adjustable='box')

    #Plot all paths for the laser in one color
    plt.plot(coords[:,0],coords[:,1], color = '#2469BA', label = 'Cut')

    #Plot travel paths in a different color
    for i in range(0,len(travel_segments)):
        if i == 0:
            plt.plot(travel_segments[i,:,0],travel_segments[i,:,1], color = '#24FFBA', label = 'Travel')
        else:
            plt.plot(travel_segments[i,:,0],travel_segments[i,:,1], color = '#24FFBA')
    
    #Add legend
    plt.legend(loc="upper left")

    #Set plot limits to a square with max dims
    # x0,x1 = plt.xlim()
    # y0,y1 = plt.ylim()
    # center = [(x1+x0)/2, (y1-y0)/2]
    # width = max(x1-x0,y1-y0)
    # plt.xlim(center[0]-width/2, center[0]+width/2,)
    # plt.ylim(center[1]-width/2, center[1]+width/2,)

    #Show Preview
    plt.show()

# -----------------------------------------------------------------------------------------------
