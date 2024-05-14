#include <stdbool.h>
#include <math.h>
#include "main.h"
#include "stm32f4xx_hal_i2s_ex.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define BUFFER_SIZE 128

extern I2S_HandleTypeDef hi2s3;

extern int16_t dacData[BUFFER_SIZE];
extern bool dataReady;

extern volatile int16_t *outBufPtr;

// Callbacks for DMA Complete
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2SS_TxCpltCallback(I2S_HandleTypeDef *hi2s);

// Test Functions
void generateSineWave(double frequency);
void initSineTable();

void fillBuffer();
