/** @file       check_home.h 
 *  @brief      File containing the header for check_home.cpp.
 * 
 *  @author Matthew Carlson
 *  @author Ethan Czuppa
 * 
 *  @date 23 Nov 2020 Created files for the check home task
 *
 */
#ifndef CHECKHOMETASK_H
#define CHECKHOMETASK_H

#include "libraries&constants.h"

///@cond

//Task latency
const TickType_t check_home_latency = 100;          // latency should be 10ms

///@endcond

// check home task
void check_home_task (void* p_params);


#endif // CHECKHOMETASK_H