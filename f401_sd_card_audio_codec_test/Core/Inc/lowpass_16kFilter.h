/**
 * @file lowpass_16kFilter.h
 * @brief Header file for the low-pass FIR filter used for decimation.
 * 
 * Defines the number of taps and block size for the FIR filter. Includes
 * the external filter coefficients and necessary header files for ARM CMSIS-DSP functions.
 */
#ifndef LOWPASS_16KFILTER_H_
#define LOWPASS_16KFILTER_H_

#include "arm_math.h"


/**
 * @brief Number of taps in the FIR filter.
 */
#define NUM_TAPS 57

/**
 * @brief Size of each block for processing, must be a multiple of the decimation factor.
 */
#define BLOCK_SIZE 33

extern float32_t filter_taps[NUM_TAPS];

#endif