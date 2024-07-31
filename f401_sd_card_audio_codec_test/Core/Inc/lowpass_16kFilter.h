#ifndef LOWPASS_16KFILTER_H_
#define LOWPASS_16KFILTER_H_

#include "arm_math.h"


#define NUM_TAPS 57  // HAS TO BE A MULTIPLE OF THE DECIMATION_FACTOR
// #define NUM_TAPS 84  // HAS TO BE A MULTIPLE OF THE DECIMATION_FACTOR
#define BLOCK_SIZE 33 // HAS TO BE A MULTIPLE OF THE DECIMATION_FACTOR

extern float32_t filter_taps[NUM_TAPS];

#endif