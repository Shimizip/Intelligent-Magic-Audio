#ifndef RESAMPLE_H
#define RESAMPLE_H

#include <stdint.h>
#include "arm_math.h"
#include "fatfs.h"
#include "lowpass_16kFilter.h"
#include "wavPlayer.h"

// Constants
#define DECIMATION_FACTOR_ROUNDED 3 // Rounded up from 44.1kHz / 16kHz = 2.75625
#define NUM_CHANNELS 2

// number samples per chunk, round Input Chunk up to be even (in stereo processing)
#define NUM_SAMPLES_CHUNK_OUT 1024
#define NUM_SAMPLES_CHUNK_IN ((NUM_SAMPLES_CHUNK_OUT * DECIMATION_FACTOR_ROUNDED * NUM_CHANNELS + 1) & ~1)

// #define NUM_SAMPLES_CHUNK_OUT ((NUM_SAMPLES_CHUNK_IN / DECIMATION_FACTOR_ROUNDED + 1) & ~1)
// #define NUM_SAMPLES_CHUNK_IN ((44100 / 8 + 1) & ~1)

// actual Byte sizes of the chunks
#define RESAMPLE_CHUNK_IN_SIZE  (NUM_SAMPLES_CHUNK_IN * sizeof(int16_t)) // 1/8 second
#define RESAMPLE_CHUNK_OUT_SIZE  (NUM_SAMPLES_CHUNK_OUT * sizeof(int16_t))

// 
#define FRAME_SIZE 1024 // window frame size
#define HOP_SIZE 512    // overlap between frames
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
