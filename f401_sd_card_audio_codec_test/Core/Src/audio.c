/**
 * @file audio.c
 * @brief Audio playback and processing functions.
 * 
 * Implements functions for audio playback using DMA and I2S, including sine wave 
 * generation, buffer management, and pitch adjustment. 
 * Also provides a precomputed sine wave table for testing the Codec. 
 */

#include "audio.h"

bool dma_dataReady = false;
int16_t dacData[BUFFER_SIZE];
int16_t fileReadBuf[BUFFER_SIZE_INPUT]; // actually for one halfBuffer, now is = BUFFER_SIZE, to allow 2x playback speed
volatile int16_t *outBufPtr = &dacData[0];
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

/**
 * @brief Called when the first half of the I2S buffer is transmitted.
 * 
 * Updates the buffer pointer to the start of `dacData` and sets the DMA data ready flag.
 * 
 * @param hi2s Pointer to the I2S handle structure.
 */
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s){
    outBufPtr = &dacData[0]; // set file read Buffer Pointer
    dma_dataReady = true;
}

/**
 * @brief Called when the entire I2S buffer is transmitted.
 * 
 * Updates the buffer pointer to the second half of `dacData` and sets the DMA data ready flag.
 * 
 * @param hi2s Pointer to the I2S handle structure.
 */
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s){
    outBufPtr = &dacData[HALF_BUFFER_SIZE]; // set read Buffer Pointer to second half
    dma_dataReady = true;
}

/**
 * @brief Populates half of the DAC buffer with a sine wave using a lookup table.
 * 
 * The function generates a sine wave based on the provided frequency. It uses a 
 * lookup table to fill the DAC buffer with sine values and manages the phase index 
 * for the waveform. It waits for the DMA to be ready before updating 
 * the buffer.
 * 
 * @param frequency Desired frequency of the sine wave.
 */
void generateSineWave(double frequency) {
    static uint16_t phaseIndex = 0;                 /**< Retains the index between function calls */
    double phaseIncrement = frequency * SINE_TABLE_SIZE / I2S_AUDIOFREQ_44K;  
    HAL_I2S_Transmit_DMA(&hi2s2, (void *)dacData, BUFFER_SIZE);
    
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

/**
 * @brief Fills half of the buffer with audio samples from an SD file.
 * 
 * The function reads audio samples from an SD card file into a buffer. If the `pitched` 
 * parameter is true, it adjusts the number of samples to account for pitch shifting. 
 * It ensures the number of samples is even and reads the data into `fileReadBuf`.
 * 
 * @param player Pointer to a `WavPlayer` structure containing file information and pitch factor.
 * @param pitched Flag indicating whether pitch shifting should be applied.
 * 
 * @return The number of bytes actually read from the file.
 */
uint16_t fillHalfBufferFromSD(WavPlayer *player, bool pitched){
    UINT bytesRead = 0;
    uint32_t samplesNeeded;
    
    if (pitched) {
        float tempSamplesNeeded = (HALF_BUFFER_SIZE * player->pitchFactor);
        samplesNeeded = (uint32_t) ceilf(tempSamplesNeeded);
        samplesNeeded = (samplesNeeded + 1) & ~1; // Ensure it's an even number
    } else {
        samplesNeeded = HALF_BUFFER_SIZE;
    }
    
    uint32_t bytesNeeded = samplesNeeded * sizeof(uint16_t);
    
    // Fill half of the buffer with 16-bit samples
    if (f_read(player->file, fileReadBuf, bytesNeeded, &bytesRead) != FR_OK) {
        // Error handling
        while(1);
    }
    
    return (uint16_t) bytesRead;
}

/**
 * @brief Sets a new pitch factor for audio playback.
 * 
 * Updates the global pitch factor and marks it as changed. Interrupts are temporarily 
 * disabled to ensure thread safety during the update.
 * 
 * @param newPitchFactor The new pitch factor to set.
 */
void setPitchFactor(float newPitchFactor) {
    __disable_irq(); // Disable interrupts to ensure thread safety
    pitchFactor = newPitchFactor;
    pitchChanged = true;
    __enable_irq(); // Re-enable interrupts
}
