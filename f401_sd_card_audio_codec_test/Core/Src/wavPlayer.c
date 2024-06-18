#include "fatfs.h"
#include "wavPlayer.h"
#include "string.h"

void initPlayer(WavPlayer *player, FIL *file, wav_header_t *wavHeader) {
    player->file = file;
    player->wavHeader = wavHeader;
    player->restartPlayback = false;
    player->playbackActive = false;
}

// load .wav header and check for valid header members
uint8_t checkWav(WavPlayer *player) {
    uint8_t wav_OK = 1;
    UINT count = 0;

    // Read RIFF header and first Chunki
    if (f_read(player->file, player->wavHeader, sizeof(uint32_t) * 5, &count) != FR_OK) {
        return 0;
    }

    // Check RIFF header
    if (player->wavHeader->ChunkID != 0x46464952 || player->wavHeader->Format != 0x45564157) {
        return 0;
    }

    // Read additional chunks until "fmt " subchunk is found
    while (1) {
        if (player->wavHeader->Subchunk1ID == 0x20746d66) { // "fmt "
            // Essential "fmt " subchunk found
            break;
        }

        f_lseek(player->file, f_tell(player->file) + player->wavHeader->Subchunk1Size);

        // Calculate the address of the next subchunk header
        wav_header_t* nextHeader = (wav_header_t*)((char*)player->wavHeader + sizeof(uint32_t) * 3);

        // Read next subchunk header / rest of the wav header
        if (f_read(player->file, nextHeader, sizeof(wav_header_t) - sizeof(uint32_t) * 3, &count) != FR_OK) {
            return 0;
        }
    }

    // Check essential "fmt " subchunk
    if (player->wavHeader->Subchunk1Size != 16 || player->wavHeader->AudioFormat != 1) {
        return 0;
    }

    return wav_OK;
}

void playButtonHandler(WavPlayer *player){
    if(!player->playbackActive){
        player->playbackActive = true;
    }
    else if(player->playbackActive){
        player->restartPlayback = true;
    }
}

uint8_t wavPlay(WavPlayer *player){
    // populate wavHeader and check if file is in correct format
    checkWav(player);
    uint32_t length = player->wavHeader->Subchunk2Size ;    // Number of bytes in data. Number of samples * num_channels * sample byte size
    uint32_t remainingBytes = length;

    // Start DMA Stream
    HAL_I2S_Transmit_DMA(&hi2s3,(uint16_t *)dacData, BUFFER_SIZE);

    float left;

    while(remainingBytes > 0){
        if(dma_dataReady){
            uint16_t bytesRead = fillHalfBufferFromSD(player->file, false);
            memcpy(outBufPtr, fileReadBuf, HALF_BUFFER_SIZE * sizeof(uint16_t));
            
            // for(int n = 0; n < HALF_BUFFER_SIZE; n++){
            //     left = outBufPtr[n];
            //     left = 1.0 * left;
            //     outBufPtr[n] = left;
            // }
            if(player->restartPlayback){
                f_lseek(player->file,0);
                player->restartPlayback = false;
            }
            
            remainingBytes -= bytesRead;
            dma_dataReady = false;
        }
    }
    HAL_I2S_DMAStop(&hi2s3);
    f_lseek(player->file,0);
    player->playbackActive = false;
    return 0;
}

uint8_t wavPlayPitched(WavPlayer *player) {
    uint32_t length = player->wavHeader->Subchunk2Size;  // Number of bytes in data. Number of samples * num_channels * sample byte size
    uint32_t remainingBytes = length;

    float currentPitchFactor = pitchFactor;
    float sampleIndex = 0.0f;

    // Start DMA Stream
    HAL_I2S_Transmit_DMA(&hi2s3, (void *)dacData, BUFFER_SIZE);
    while (remainingBytes > 0) {
        uint16_t bytesRead;
        uint32_t leftIndex;
        if (dma_dataReady) {
            bytesRead = fillHalfBufferFromSD(player->file, true); // loads necessary data from sd card to fireReadBuffer
            remainingBytes -= bytesRead;

            // // Update pitch factor if changed
            // if (pitchChanged) {
            //     __disable_irq();
            //     currentPitchFactor = pitchFactor;
            //     pitchChanged = false;
            //     __enable_irq();
            // }

            // Adjust the samples for pitch shifting by skipping or repeating samples
            // memcpy(outBufPtr, fileReadBuf, HALF_BUFFER_SIZE * sizeof(uint16_t));
            for (uint8_t n = 0; n < (HALF_BUFFER_SIZE) - 1; n += 2) {
                // Calculate the index for left sample
                leftIndex = (uint32_t)sampleIndex;
                outBufPtr[n] = fileReadBuf[leftIndex];

                // Calculate the index for right sample
                // uint32_t rightIndex = leftIndex + 1;
                // outBufPtr[n * 2 + 1] = fileReadBuf[rightIndex];

                sampleIndex += currentPitchFactor * 2;
            }

            // Ensure sampleIndex is within the bounds
            if (sampleIndex >= bytesRead / sizeof(uint16_t)) {
                sampleIndex -= bytesRead / sizeof(uint16_t);
            }

            // Ensure sampleIndex is within the bounds
            // if (sampleIndex >= HALF_BUFFER_SIZE) {
            //     sampleIndex -= HALF_BUFFER_SIZE;
            // }

            dma_dataReady = false;
        }
    }
    f_lseek(player->file,0);
    HAL_I2S_DMAStop(&hi2s3);
    return 0;
}