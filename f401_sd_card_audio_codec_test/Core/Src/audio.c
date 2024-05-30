#include "audio.h"
#include "fatfs.h"
#include "wavDecoder.h"

bool dma_dataReady = false;
int16_t dacData[BUFFER_SIZE];
volatile int16_t *outBufPtr = &dacData[0];

#define SINE_TABLE_SIZE 256 // Adjust the table size as needed for your accuracy/performance tradeoff

// Precomputed sine wave table
int16_t sineTable[SINE_TABLE_SIZE];

// Populate the sine wave table during initialization
void initSineTable() {
    for (int i = 0; i < SINE_TABLE_SIZE; ++i) {
        sineTable[i] = (int16_t)(32767 * sin(2.0 * M_PI * i / SINE_TABLE_SIZE));
    }
}

// Callbacks for DMA Complete
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s){
    outBufPtr = &dacData[0];
    dma_dataReady = true;
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s){
    outBufPtr = &dacData[BUFFER_SIZE/2];
    dma_dataReady = true;
}

// populates half of the DAC Buffer, with the help of a lookup table
void generateSineWave(double frequency) {
    static uint16_t phaseIndex = 0;                 // retain index between function calls
    double phaseIncrement = frequency * SINE_TABLE_SIZE / I2S_AUDIOFREQ_44K;  

    for (uint8_t n = 0; n < (BUFFER_SIZE / 2) - 1; n += 2) {
        // Lookup sine value from table
        outBufPtr[n] = sineTable[phaseIndex];
        
        // Increment phase index
        phaseIndex += phaseIncrement;
        
        // Wrap phase index if it exceeds table size
        if (phaseIndex >= SINE_TABLE_SIZE) {
            phaseIndex -= SINE_TABLE_SIZE;
        }
    }
}

uint16_t fillHalfBufferFromSD(FIL *fil){
    UINT bytesRead = 0;
    // fill half of the buffer (16bit Samples -> BUFFER_SIZE)
    if (f_read(fil,(void *)outBufPtr, BUFFER_SIZE, &bytesRead) != FR_OK) {
    // Error handling
        while(1);
    }
    // for (uint8_t n = 0; n < BUFFER_SIZE / 2; n += 2) {
    //     outBufPtr[n] *= 2;
    // }
    return (uint16_t) bytesRead;
}
