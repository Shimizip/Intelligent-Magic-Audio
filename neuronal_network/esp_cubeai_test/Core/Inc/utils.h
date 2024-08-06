/*
 * utils.h
 *
 *  Created on: Jul 3, 2024
 *      Author: leon
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <stdint.h>



// enable the DWT cycle counter
void EnableDWT(void);

// Functions to start and stop cycle measurement
void StartCycleMeasurement(void);
void StopCycleMeasurement(void);

// Function to get the measured cycle count
uint32_t GetMeasuredCycles(void);

// Function to convert cycles to milliseconds
uint32_t CyclesToMilliseconds(uint32_t cycles);

// Function to convert cycles to microseconds
uint32_t CyclesToMicroseconds(uint32_t cycles);

#endif
