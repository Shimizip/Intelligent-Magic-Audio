#include "fatfs.h"
#include "wavPlayer.h"
#include "string.h"

void initPlayer(WavPlayer *player, FIL *file, wav_header_t *wavHeader) {
    player->file = file;
    player->wavHeader = wavHeader;
    player->restartPlayback = false;
    player->playbackActive = false;
    player->headerSize = 0;
    player->pitchFactor = 3.5f;
    player->pitchChanged = false;
}

// load .wav header and check for valid header members
uint8_t checkWav(WavPlayer *player) {
    uint8_t wav_OK = 1;
    UINT count = 0;

    // Read RIFF header and first Chunk
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
        player->headerSize += player->wavHeader->Subchunk1Size;

        // Calculate the address of the next subchunk header
        wav_header_t* nextHeader = (wav_header_t*)((char*)player->wavHeader + sizeof(uint32_t) * 3);

        // Read next subchunk header / rest of the wav header
        if (f_read(player->file, nextHeader, sizeof(wav_header_t) - sizeof(uint32_t) * 3, &count) != FR_OK) {
            return 0;
        }
        player->headerSize += count;
    }

    // Check essential "fmt " subchunk
    if (player->wavHeader->Subchunk1Size != 16 || player->wavHeader->AudioFormat != 1) {
        return 0;
    }

    // save headerSize for later usage
    player->headerSize = (uint32_t) player->file->fptr;

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

// load file via FATFS 
// populate wavHeader and check if file is in correct format
FRESULT wavLoad(WavPlayer *player,const char *filename){
    FRESULT res;
    res = f_open(player->file, filename, FA_READ);
    if (res == FR_OK){
        checkWav(player);
    }
    return res;
}

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

                // interpolate left and right samples
                // outBufPtr[n] = interpolate(fileReadBuf, baseIndex);
                // outBufPtr[n+1] = interpolate(fileReadBuf, baseIndex + currentPitchFactor);

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