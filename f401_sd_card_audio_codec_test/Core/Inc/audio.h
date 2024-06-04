#include <stdbool.h>
#include <math.h>
#include "main.h"
#include "wavDecoder.h"
#include "fatfs.h"
#include "stm32f4xx_hal_i2s_ex.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define BUFFER_SIZE 256
#define HALF_BUFFER_SIZE BUFFER_SIZE/2

extern I2S_HandleTypeDef hi2s3;

extern int16_t dacData[BUFFER_SIZE];
extern int16_t fileReadBuf[BUFFER_SIZE]; // actually for one halfBuffer, now is = BUFFER_SIZE, to allow 2x playback speed
extern bool dma_dataReady;
extern volatile int16_t *outBufPtr;

extern volatile float pitchFactor;
extern volatile bool pitchChanged;

// Callbacks for DMA Complete
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2SS_TxCpltCallback(I2S_HandleTypeDef *hi2s);

// fills half of the buffer with next chunk of data from file
// returns the number of bytes read
uint16_t fillHalfBufferFromSD(FIL *fil, bool pitched);

// Test Functions
void generateSineWave(double frequency);
void initSineTable();

// Pitch functions
void setPitchFactor(float newPitchFactor);

uint16_t interpolate(uint16_t *buffer, float index, uint32_t bufferSize);
