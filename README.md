# ME507-desktop-laserEngraver

## Overview

This repository consists of the collected efforts of Niko Banks, Matthew Carlson, and Ethan Czuppa to develop an open source, desktop, CoreXY laser engraver for the ME 507 class at Cal Poly SLO. As an open source design, this design builds on the work of others and is intended as an academic exercise to yield useful results. We would like to thank the following individuals and organizations for their previous work on desktop laser engravers/cutters:
  * The developers of LaserWeb, an application made to export G-code and run laser cutters https://github.com/LaserWeb/LaserWeb4
  * Thomas and his design of a CoreXY laser Engraver on PrusaPrinters Beta https://www.prusaprinters.org/prints/18771-corexy-laser-engraver
  * Instructables user nccwarp9 for his CoreXY laser Engraver design https://www.instructables.com/CoreXY-Laser-Engraver/
  * Many thanks to the other open source developers that built the drivers, wrappers, classes, and packages to allow our code to run on the STM32 Nucleo L476RG 
  * Dr. Ridgely from the Cal Poly Mechanical Engineering Department for his ME 507 library support and excellence as a professor in the midst of a pandemic
  
## Contents

The following sections are available for use under the GNU public license V3.0:
   1.  Disclaimer
   2.  Electronics Documentation (EAGLE schematic and board files)
   3.  A complete Bill of Materials with component sources
   4.  Software Repository including developed modules and externally developed libraries
   5.  Software Dependencies
   6.  CAD (final assembly/sub-assembly/part files in .step format)
   7.  3D printed Structural Components (.STL files with slicer settings documented separately)
   8.  Assembly Instructions
   9.  Software Documentation
   10. User manual
   
## Disclaimer

This is an entirely open source project developed by college students over the course of a single quarter and some change. While development is still ongoing, we are no where near completion, and even when we have finished the first full release the successful use of this library and laser engraver - should you chose to construct - will vary from end user to end user. Additionally, we make no claims to have developed this entirely on our own. We leveraged large portions of others people's code, designs, and ideas and gave them credit for it. As such this repository, while it contains everything needed to build this hobby-level desktop laser engraver it is neither intended for commercial use nor allowed to be included in a commercial product - see the license if you are unsure if your usage of this repository is permitted. 

## Electronics Documentation
Currently pending
- [ ] put board and schematic on here in the repository, maybe have pictures of them here 

## Bill of Materials
Currently pending
- [ ] add a link to a google sheet version of our current excel

## Software Repository

You're in it right now :tada: Check out the *scr* file directory for the actual code.

If you have questions about how to use the software or how it works checkout the **Software Documentation** section :octocat: 

## Software Dependencies 

**NOTE:** the development and use of this software was done in *VScode* with the *platformIO extension* using the *Arduino framework* and **all** *included packages.* If you plan to use this code in a different enviornment make sure that you will be able to get all the packages included in the Arduino framework (especially the STM32duino / STM32 Generic packages) into the IDE of your choice.

All the libraries that our classes and modules depend on are specified in the platform.ini file but, for your convenience, they have also been included here:

* Spluttflob's fork of the Arduino PrintStream Library: https://github.com/spluttflob/Arduino-PrintStream.git
* My personal fork of the STM32FreeRTOS library that has a faster tickrate: https://github.com/eczuppa/STM32FreeRTOS.git
* J. Rowberg's I2C Library: https://github.com/jrowberg/i2cdevlib.git
* ElectronicCat's MPU6050 accelerometer driver library: https://github.com/ElectronicCats/mpu6050.git
* Paul Stoffregen's OneWire communication library: https://github.com/PaulStoffregen/OneWire.git
* Miles Burton's fork of the Dallas Temperature Library: https://github.com/milesburton/Arduino-Temperature-Control-Library.git
* An updated version of the Arduino Core for STM32FreeRTOS: https://github.com/stm32duino/Arduino_Core_STM32.git
* nikob997's fork of tomstewart89's Basic Linear Algebra library: https://github.com/nikob997/BasicLinearAlgebra.git

Thank you to all the developers!! Y'all make open source projects like this one possible!! :+1: :octocat: :sparkles: :tada:  

## CAD
Currently pending
- [ ] make a google drive folder with all the .step files in it or place in repository directly...

## 3D Printed Components
Currently pending
- [ ] make a google drive folder with all the .stl files in it or place in repository directly...

## Assembly Instructions
Currently pending
- [ ] make a google doc with these (give viewing access to those with the link)

## Software Documentation
Have a question on software usage? 
Check out the generated version of all those doxygen comments here: https://eczuppa.github.io/ME507-desktop-laserEngraver/index.html
  
## User Manual 
Currently pending
- [ ] make a google doc with a quick start and troubleshooting guide (give viewing access to those with the link)
