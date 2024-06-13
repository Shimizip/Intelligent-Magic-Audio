#include "audio.h"
#include "fatfs.h"
#include "wavDecoder.h"

bool dma_dataReady = false;
int16_t dacData[BUFFER_SIZE];
int16_t fileReadBuf[BUFFER_SIZE]; // actually for one halfBuffer, now is = BUFFER_SIZE, to allow 2x playback speed
volatile int16_t *outBufPtr = &dacData[0];
volatile float pitchFactor = 1.0;
volatile bool pitchChanged = false;

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
    outBufPtr = &dacData[0]; // set file read Buffer Pointer
    dma_dataReady = true;
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s){
    outBufPtr = &dacData[HALF_BUFFER_SIZE]; // set read Buffer Pointer to second half
    dma_dataReady = true;
}

// populates half of the DAC Buffer, with the help of a lookup table
void generateSineWave(double frequency) {
    static uint16_t phaseIndex = 0;                 // retain index between function calls
    double phaseIncrement = frequency * SINE_TABLE_SIZE / I2S_AUDIOFREQ_44K;  
    HAL_I2S_Transmit_DMA(&hi2s3, (void *)dacData, BUFFER_SIZE);
    while(1){
        if(dma_dataReady){
            for (uint8_t n = 0; n < (HALF_BUFFER_SIZE) - 1; n += 2) {
                // Lookup sine value from table
                outBufPtr[n] = sineTable[phaseIndex];
                
                // Increment phase index
                phaseIndex += phaseIncrement;
                
                // Wrap phase index if it exceeds table size
                if (phaseIndex >= SINE_TABLE_SIZE) {
                    phaseIndex -= SINE_TABLE_SIZE;
                }
            }
            dma_dataReady = false;   
        }

    }

}

uint16_t fillHalfBufferFromSD(FIL *fil, bool pitched){
    UINT bytesRead = 0;
    uint32_t samplesNeeded;
    if (pitched){
        float tempSamplesNeeded = (HALF_BUFFER_SIZE * pitchFactor);
        samplesNeeded = (uint32_t) ceil(tempSamplesNeeded);
        samplesNeeded = (samplesNeeded + 1) & ~1; // make sure its an even amount
    } else {
        samplesNeeded = HALF_BUFFER_SIZE;
    }
    uint32_t bytesNeeded = samplesNeeded * sizeof(uint16_t);
    // fill half of the buffer (16bit Samples -> BUFFER_SIZE)
    if (f_read(fil, fileReadBuf, bytesNeeded, &bytesRead) != FR_OK) {
    // Error handling
        while(1);
    }
    return (uint16_t) bytesRead;
}

void setPitchFactor(float newPitchFactor) {
    __disable_irq(); // Disable interrupts to ensure thread safety
    pitchFactor = newPitchFactor;
    pitchChanged = true;
    __enable_irq(); // Re-enable interrupts
}

uint16_t interpolate(uint16_t *buffer, float index, uint32_t bufferSize) {
    uint32_t idx = (uint32_t)index;
    float frac = index - idx;
    uint32_t nextIdx = (idx + 1) < bufferSize ? (idx + 1) : idx;
    return (uint16_t)((1.0f - frac) * buffer[idx] + frac * buffer[nextIdx]);
}
