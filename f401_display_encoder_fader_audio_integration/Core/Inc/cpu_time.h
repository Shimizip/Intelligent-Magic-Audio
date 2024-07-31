#ifndef CPU_TIME_H
#define CPU_TIME_H

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
