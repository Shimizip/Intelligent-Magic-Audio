/**
 * @file audioPreprocessor.h
 * @brief Header file for the audio preprocessing DSP-Operations.
 * 
 */
#ifndef RESAMPLE_H
#define RESAMPLE_H

#include <stdint.h>
#include "arm_math.h"
#include "fatfs.h"
#include "lowpass_16kFilter.h"
#include "wavPlayer.h"

// Constants
/**
* @brief Decimation factor rounded up from 2.8 to 3.0
*/
#define DECIMATION_FACTOR_ROUNDED 3 // Rounded up from 44.1kHz / 16kHz = 2.75625
#define NUM_CHANNELS 2

/**
 * @brief Number of samples per chunk for output processing.
 */
#define NUM_SAMPLES_CHUNK_OUT 1024
/**
 * @brief Number of samples per chunk for input processing, adjusted to be even for stereo.
 */
#define NUM_SAMPLES_CHUNK_IN ((NUM_SAMPLES_CHUNK_OUT * DECIMATION_FACTOR_ROUNDED * NUM_CHANNELS + 1) & ~1)
/**
 * @brief Byte size of the input chunk for resampling.
 */
#define RESAMPLE_CHUNK_IN_SIZE (NUM_SAMPLES_CHUNK_IN * sizeof(int16_t))
/**
 * @brief Byte size of the output chunk for resampling.
 */
#define RESAMPLE_CHUNK_OUT_SIZE (NUM_SAMPLES_CHUNK_OUT * sizeof(int16_t))
/**
 * @brief Size of the window frame for processing.
 */
#define FRAME_SIZE 1024
/**
 * @brief Overlap size between consecutive frames.
 */
#define HOP_SIZE 512
/**
 * @brief Step size between frames, calculated as the difference between frame size and overlap size.
 */
#define STEP_SIZE (FRAME_SIZE - HOP_SIZE)
// External filter coefficients (defined in your filter file)
extern float32_t filter_taps[NUM_TAPS];

// Function declarations
arm_status initFilter();
uint16_t resampleFile(FIL *inFil, FIL *outFil, wav_header_t *waveHeaderInput, wav_header_t *waveHeaderResampled);
void downsample_Block(int16_t *src, int16_t *dest);
uint16_t get_number_subsamples(FIL *file);
uint32_t downsample_to_1024_samples(FIL *file, int16_t outChunk[NUM_SAMPLES_CHUNK_OUT]);

#endif // RESAMPLE_H
