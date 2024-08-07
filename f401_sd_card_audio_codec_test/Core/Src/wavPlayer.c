/**
 * @file wavPlayer.c
 * @brief Implementation of a WAV player using FATFS and I2S with DMA.
 * 
 * This file provides functions to:
 * - Initialize the WAV player.
 * - Read and validate WAV file headers.
 * - Handle playback control via button actions.
 * - Load and play WAV files, with optional pitch shifting.
 * 
 */

#include "fatfs.h"
#include "wavPlayer.h"
#include "string.h"

/**
 * @brief Initializes a WAV player with specified file and header.
 * 
 * Sets up the `WavPlayer` structure with the given file pointer and WAV header.
 * Initializes playback control flags, header size, and pitch factor to default values.
 * 
 * @param player Pointer to the `WavPlayer` structure to initialize.
 * @param file Pointer to the `FIL` object representing the WAV file.
 * @param wavHeader Pointer to the `wav_header_t` structure containing the WAV file header.
 */
void initPlayer(WavPlayer *player, FIL *file, wav_header_t *wavHeader) {
    player->file = file;
    player->wavHeader = wavHeader;
    player->restartPlayback = false;
    player->playbackActive = false;
    player->headerSize = 0;
    player->pitchFactor = 1.0f;
    player->pitchChanged = false;
}

/**
 * @brief Reads and populates the WAV header from a file.
 * 
 * This function reads the WAV file header and fills the provided `wav_header_t` structure.
 * It continues reading chunks until it finds the "fmt " subchunk and returns the total
 * size of the WAV header.
 * 
 * @param file Pointer to the `FIL` object representing the WAV file.
 * @param wavHeader Pointer to the `wav_header_t` structure to be populated with header data.
 * @return Size of the WAV header in bytes, or 0 on error.
 */
uint32_t populateWavHeader(FIL *file, wav_header_t *wavHeader){
    UINT count = 0;
    uint32_t headerSize = 0;
    
    f_lseek(file, 0);
    // Read RIFF header and first Chunk
    volatile FRESULT res;

    res = f_read(file, wavHeader, sizeof(uint32_t) * 5, &count);
    // if (res != FR_OK) {
    //     volatile int dummy = 0; 
    //     return 0;
    // }
    headerSize += count;

    // Read additional chunks until "fmt " subchunk is found
    while (1) {
        if (wavHeader->Subchunk1ID == 0x20746d66) { // "fmt "
            // Essential "fmt " subchunk found
            volatile int dummy = 0; 
            return headerSize;
        }

        f_lseek(file, f_tell(file) + wavHeader->Subchunk1Size);
        headerSize += wavHeader->Subchunk1Size;

        // Calculate the address of the next subchunk header
        wav_header_t* nextHeader = (wav_header_t*)((char*)wavHeader + sizeof(uint32_t) * 3);

        // Read next subchunk header / rest of the wav header
        res = f_read(file, nextHeader, sizeof(wav_header_t) - sizeof(uint32_t) * 3, &count);
        if (res != FR_OK) {
            volatile int dummy = 0; 
            return 0;
        }
        headerSize += count;
    }
}

/**
 * @brief Validates the WAV file header for correctness.
 * 
 * This function reads the WAV header from the `WavPlayer` structure and checks for
 * validity based on key header fields. It verifies the RIFF header and essential
 * "fmt " subchunk to ensure the WAV file conforms to expected formats.
 * 
 * @param player Pointer to the `WavPlayer` structure containing the WAV file and header.
 * @return `1` if the WAV header is valid, `0` otherwise.
 */
uint8_t checkWav(WavPlayer *player) {
    uint8_t wav_OK = 1;

    player->headerSize = populateWavHeader(player->file, player->wavHeader);
    // Check RIFF header
    if (player->wavHeader->ChunkID != 0x46464952 || player->wavHeader->Format != 0x45564157) {
        return 0;
    }
        // Check essential "fmt " subchunk
    if (player->wavHeader->Subchunk1Size != 16 || player->wavHeader->AudioFormat != 1) {
        return 0;
    }
    // other header Checks, to ensure correct format:
    // - check for max duration 
    // here also Codec clock and mono/stereo may be dynamically set, depending on the .wav file
    // this would allow for different file formats
    // e.g. 
    // - different samplerate
    // - different channel numbers (mono/stereo) 

    return wav_OK;
}

/**
 * @brief Handles the play button action for audio playback.
 * 
 * This function toggles the playback state of the `WavPlayer`. If playback is not active,
 * it sets the `playbackActive` flag to `true`. If playback is already active, it sets
 * the `restartPlayback` flag to `true` to restart playback.
 * 
 * @param player Pointer to the `WavPlayer` structure managing the audio playback.
 */
void playButtonHandler(WavPlayer *player){
    if(!player->playbackActive){
        player->playbackActive = true;
    }
    else if(player->playbackActive){
        player->restartPlayback = true;
    }
}

/**
 * @brief Loads a WAV file and validates its header format.
 * 
 * This function opens a WAV file using FATFS, populates the `wav_header_t` structure
 * in the `WavPlayer`, and checks if the file format is correct. It returns the result
 * of the file opening operation.
 * 
 * @param player Pointer to the `WavPlayer` structure that will be used to manage the WAV file.
 * @param filename Name of the WAV file to be loaded.
 * @return `FR_OK` if the file is successfully opened and format is valid, or an error code from FATFS otherwise.
 */
FRESULT wavLoad(WavPlayer *player,const char *filename){
    FRESULT res;
    res = f_open(player->file, filename, FA_READ);
    if (res == FR_OK){
        checkWav(player);
    }
    return res;
}

/**
 * @brief Plays the WAV file from the given `WavPlayer`.
 * 
 * This function initializes the DMA stream for I2S transmission, reads data from
 * the WAV file, and plays it through the DAC. It handles playback restarting and
 * manages the remaining bytes of audio data. The function also resets the playback
 * state upon completion.
 * 
 * @param player Pointer to the `WavPlayer` structure containing the WAV file and header.
 * @return `0` on successful playback completion. Non-zero return values are reserved for error handling.
 */
uint8_t wavPlay(WavPlayer *player){
    f_lseek(player->file,player->headerSize);

    uint32_t length = player->wavHeader->Subchunk2Size ;    // Number of bytes in data. Number of samples * num_channels * sample byte size
    uint32_t remainingBytes = length;
    
    // Start DMA Stream
    HAL_I2S_Transmit_DMA(&hi2s2,(uint16_t *)dacData, BUFFER_SIZE);

    // float left;

    while(remainingBytes > 0){
        if(dma_dataReady){
            uint16_t bytesRead = fillHalfBufferFromSD(player, false);
            memcpy(outBufPtr, fileReadBuf, HALF_BUFFER_SIZE * sizeof(uint16_t));
            
            // for(int n = 0; n < HALF_BUFFER_SIZE; n++){
            //     left = outBufPtr[n];
            //     left = 1.0 * left;
            //     outBufPtr[n] = left;
            // }

            if(player->restartPlayback){
                f_lseek(player->file,player->headerSize);
                remainingBytes = length;
                player->restartPlayback = false;
            }
            
            if (bytesRead > remainingBytes) {
                break;
            } else {
                remainingBytes -= bytesRead;
            }
            
            dma_dataReady = false;
        }
    }
    HAL_I2S_DMAStop(&hi2s2);
    f_lseek(player->file,player->headerSize);
    player->playbackActive = false;
    return 0;
}

/**
 * @brief Plays a WAV file with pitch shifting applied.
 * 
 * This function plays a WAV file with pitch adjustment using DMA for I2S transmission. 
 * It reads audio data from the file, applies pitch shifting by adjusting the playback speed,
 * and handles playback restarting. The function updates the pitch factor dynamically if changed.
 * 
 * @param player Pointer to the `WavPlayer` structure managing the WAV file and playback.
 * @return `0` on successful playback completion. Non-zero return values are reserved for error handling.
 */
uint8_t wavPlayPitched(WavPlayer *player) {
    f_lseek(player->file,player->headerSize);

    uint32_t length = player->wavHeader->Subchunk2Size;  // Number of bytes in data. Number of samples * num_channels * sample byte size
    uint32_t remainingBytes = length;

    float currentPitchFactor = player->pitchFactor;
    float baseIndex = 0.0f;

    int16_t lastOutBufL = 0;
    int16_t lastOutBufR = 0;

    uint32_t leftReadIndex = 0;
    uint32_t rightReadIndex = 0;

    // Start DMA Stream
    HAL_I2S_Transmit_DMA(&hi2s2, (void *)dacData, BUFFER_SIZE);
    while (remainingBytes > 0) {
        uint16_t bytesRead;
        if (dma_dataReady) {
            bytesRead = fillHalfBufferFromSD(player, true); // loads necessary data from sd card to fireReadBuffer

            // Adjust the samples for pitch shifting by skipping or repeating samples
            // memcpy(outBufPtr, fileReadBuf, HALF_BUFFER_SIZE * sizeof(uint16_t));
            for (uint32_t n = 0; n < (HALF_BUFFER_SIZE) - 1; n += 2) {
                // Calculate the index for left sample
                
                // interpolation may give even better results
                // round samples without interpolation
                int32_t res = (int32_t) roundf(baseIndex) * 2;
                if (res < 0) {
                    outBufPtr[n] = lastOutBufL;
                    outBufPtr[n+1] = lastOutBufR;
                } else {
                    leftReadIndex = (uint32_t)res;
                    leftReadIndex = (leftReadIndex + 1) & ~1; // make sure its an even amount -- ditch the last 0b1
                    rightReadIndex = leftReadIndex + 1;

                    outBufPtr[n] = fileReadBuf[leftReadIndex];
                    outBufPtr[n+1] = fileReadBuf[rightReadIndex];

                    lastOutBufL = outBufPtr[n];
                    lastOutBufR = outBufPtr[n+1];
                }

                baseIndex += currentPitchFactor;

            }

            if (baseIndex <= (bytesRead / sizeof(uint16_t))/2 - 1 )   {
                baseIndex -= (bytesRead / sizeof(uint16_t)/2) - 1;
            } else {
                baseIndex -= (bytesRead / sizeof(uint16_t)/2);
            }

            if (bytesRead > remainingBytes) {
                break;
            } else {
                remainingBytes -= bytesRead;
            }

            // Update pitch factor if changed
            if (player->pitchChanged) {
                __disable_irq();
                currentPitchFactor = player->pitchFactor;
                player->pitchChanged = false;
                __enable_irq();
            }

            if(player->restartPlayback){
                f_lseek(player->file,player->headerSize);
                remainingBytes = length;
                player->restartPlayback = false;
            }

            dma_dataReady = false;
        }
    }

    HAL_I2S_DMAStop(&hi2s2);
    f_lseek(player->file,player->headerSize);
    player->playbackActive = false;
    return 0;
}