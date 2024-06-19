#ifndef INC_AUDIO_H_
#define INC_AUDIO_H_

#include <stdbool.h>
#include <math.h>
#include "main.h"
#include "fatfs.h"
#include "stm32f4xx_hal_i2s_ex.h"
#include "wavPlayer.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define BUFFER_SIZE 256
#define HALF_BUFFER_SIZE BUFFER_SIZE/2
#define BUFFER_SIZE_INPUT 4096

extern I2S_HandleTypeDef hi2s2;
extern int16_t dacData[BUFFER_SIZE];
extern int16_t fileReadBuf[BUFFER_SIZE_INPUT]; // actually for one halfBuffer, now is = BUFFER_SIZE, to allow 2x playback speed
extern bool dma_dataReady;
extern volatile int16_t *outBufPtr;

extern volatile float pitchFactor;
extern volatile bool pitchChanged;

struct WavPlayer;

// Callbacks for DMA Complete
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2SS_TxCpltCallback(I2S_HandleTypeDef *hi2s);

// fills half of the buffer with next chunk of data from file
// returns the number of bytes read
uint16_t fillHalfBufferFromSD(struct WavPlayer *player, bool pitched);

// Test Functions
void generateSineWave(double frequency);
void initSineTable();

// Pitch functions
void setPitchFactor(float newPitchFactor);

int16_t interpolate(int16_t *buffer, float index);

#endif /* INC_AUDIO_H_ */