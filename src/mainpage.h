/** @file mainpage.h
 *  @mainpage DigitalCarpenter - Lite Documentation
 * 
 *  @section dcl_intro Introduction
 *  
 *  Welcome to the documentation page for the DigitalCarpenter - Lite C++ Library! Our hobby level desktop laser engraver is still in
 *  the development phase, but feel free to parse all of the currently documented code. This desktop laser engraver software is a 
 *  less portable, simplified STM32FreeRTOS version of GRBL, but with significantly less emphasis on portability, and was developed
 *  as a project for the ME 507 class at Cal Poly SLO. The intent was that this would be a reasonably capable desktop laser engraver 
 *  laser cutter for the cost of materials and components to make it with a unique hardware setup - CoreXY positioning done by brushed
 *  DC motors with quadrature encoders. Finally, this is an open source project that makes use of open source software
 *  and is intended to be kept as such under the conditions of the GNU GPL v3 License.  
 * 
 *  @section dcl_libdeps Library Dependencies 
 *  
 *  Our library is built off of several other hardware driver and communication protocol libraries. Due to the current state
 *  of development of this opens source project some of the libraries listed below have not been used, but are intended for 
 *  future work are marked with a double asterisk (**). 
 *  
 *  - Spluttflob's ME 507 Support Library: https://github.com/spluttflob/ME507-Support.git 
 *  - Spluttflob's fork of the Arduino PrintStream Library: https://github.com/spluttflob/Arduino-PrintStream.git
 *  - My personal fork of the STM32FreeRTOS library that has a faster tickrate: https://github.com/eczuppa/STM32FreeRTOS.git
 *  - J. Rowberg's I2C Library: https://github.com/jrowberg/i2cdevlib.git **
 *  - ElectronicCat's MPU6050 accelerometer driver library: https://github.com/ElectronicCats/mpu6050.git **
 *  - Paul Stoffregen's OneWire communication library: https://github.com/PaulStoffregen/OneWire.git
 *  - Miles Burton's fork of the Dallas Temperature Library: https://github.com/milesburton/Arduino-Temperature-Control-Library.git
 *  - An updated version of the Arduino Core for STM32FreeRTOS: https://github.com/stm32duino/Arduino_Core_STM32.git
 *  - nikob997's fork of tomstewart89's Basic Linear Algebra library: https://github.com/nikob997/BasicLinearAlgebra.git
 *  
 *  All of these libraries and our own modules, see the Class list and File Members sections, are stored in a master header file
 *  called @c libraries&constants.h. If you make additions to this library be sure to include the header files of the modules you 
 *  add to the master header file so your modified files will be imported where they are needed. 
 * 
 *  To avoid include errors please make use of the following technique in all of the header files of the modules you add to this library:
 * 
 *  @code
 *  #ifndef _YOURHEADERFILENAME_H
 *  #define _YOURHEADERFILENAME_H
 *  ...
 *  #include libraries&constants.h
 *  ...
 *  {your code}
 *  #end if // _YOURHEADERFILENAME_H
 *  @endcode 
 * 
 *  @section dcl_hl_wf High Level Work Flow
 * 
 *  @image html ProgramFlow.png
 *  @image latex ProgramFlow.eps "Program Flow" width =18cm
 * 
 *  @section dcl_fn Current & Intended Functionality
 * 
 *  The custom shield we designed to interface with the STM32 Nucleo L476RG for the DigitalCarpenter - Lite was subject to a few design
 *  oversights that currently prevent it from functioning properly. This shield is currently undergoing a redesign process followed by 
 *  a more thorough design and functionality check. So, at the moment, the DigitalCarpenter - Lite does not run at all. 
 * 
 *  Despite current setbacks, here is a preview of the functionality this project is intended to have as well as what has been developed.
 *  
 *  - 0.1 mm positioning accuracy
 *  - A 2.5W Optical power output laser module
 *  - A 330 x 419 mm cutting/engraving area
 *  - Basic fume extraction and Flame out detection
 *  - A laser-safe enclosure with open-cover detection
 *  - GRBL-based GCODE parsing
 *  - A simple Python UI with Hardware/Software Emergency Stop
 *  - Full PID position and velocity control
 *  - Vector cutting 
 * 
 *  Future Work:
 *  
 *  - GRBL compatibility
 *  - Simple crash detection - with limit switches on all for corners of frame
 *  - Acceleration feedback and control
 *  - Air Assist
 *  - Rastering/Engraving
 * 
 *  @section dcl_portability Software Portability Notice
 * 
 *  The DigitalCarpenter - Lite C++ library was developed in VScode using the PlatfromIO extension on the Arduino framework. 
 *  This library is intended for use with the STM32 Nucleo L476RG and has not been tested on other platforms. If you wish to 
 *  use this library on a different MCU be advised that there is no current existing support for this from our documentation,
 *  so, YMMV. 
 * 
 * 
 * 
 * 
 */ 