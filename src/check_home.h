/** @file check_home.h is the check home task function file header
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

// check home task
void check_home_task (void* p_params);

const TickType_t check_home_latency = 100;          // latency should be 10ms


#endif // CHECKHOMETASK_H