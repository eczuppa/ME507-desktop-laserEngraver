#ifndef MOTIONPLANNING_H
#define MOTIONPLANNING_H

#include "libraries&constants.h"

class MotionPlanning
{
    protected:
    // variables to store array individual values and/or the whole XYF array
    // class member data to store the discretization period


    public:
    // a constructor that probably takes in the given array or individual variables X,Y,F current and last as well as the discretization period
    // a method to calculate the time the move should take based on the current feed rate
    //          if calculated distance is two short (in the case of linear interpolation of a curve, then don't generate a ramp input)
    //          could be a check for a <1 result in the next method
    // a method to calculate the number of sections to be generated based off of the previous method
    // a method that generates the discrete movements that make up the ramp input (needs to be called inside a for loop)
    // a getter method to get the discrete movements 

};

#endif //MOTIONPLANNING_H