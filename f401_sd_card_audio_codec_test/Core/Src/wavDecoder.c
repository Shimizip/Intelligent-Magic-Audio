#include "fatfs.h"
#include "wavDecoder.h"

// load .wav header and check for valid header members
uint8_t checkWav(FIL *file, wav_header_t *wavHeader) {
    uint8_t wav_OK = 1;
    UINT count = 0;

    // Read RIFF header and first Chunki
    if (f_read(file, wavHeader, sizeof(uint32_t) * 5, &count) != FR_OK) {
        return 0;
    }

    // Check RIFF header
    if (wavHeader->ChunkID != 0x46464952 || wavHeader->Format != 0x45564157) {
        return 0;
    }

    // Read additional chunks until "fmt " subchunk is found
    while (1) {
        if (wavHeader->Subchunk1ID == 0x20746d66) { // "fmt "
            // Essential "fmt " subchunk found
            break;
        }

        f_lseek(file, f_tell(file) + wavHeader->Subchunk1Size);

        // Calculate the address of the next subchunk header
        wav_header_t* nextHeader = (wav_header_t*)((char*)wavHeader + sizeof(uint32_t) * 3);

        // Read next subchunk header / rest of the wav header
        if (f_read(file, nextHeader, sizeof(wav_header_t) - sizeof(uint32_t) * 3, &count) != FR_OK) {
            return 0;
        }
    }

    // Check essential "fmt " subchunk
    if (wavHeader->Subchunk1Size != 16 || wavHeader->AudioFormat != 1) {
        return 0;
    }

    return wav_OK;
}

// assumes wavHeader is already populated
uint8_t wavPlay(FIL *file, wav_header_t *wavHeader){
  uint32_t length = wavHeader->Subchunk2Size ;    // Number of bytes in data. Number of samples * num_channels * sample byte size
  uint32_t remainingBytes = length;

  // Start DMA Stream
  volatile HAL_StatusTypeDef dmaStatus = HAL_I2S_Transmit_DMA(&hi2s3,(uint16_t *)dacData, BUFFER_SIZE);
  while(remainingBytes > 0){
      if(dma_dataReady){
        uint16_t bytesRead = fillHalfBufferFromSD(file);
        remainingBytes -= bytesRead;
        dma_dataReady = false;
      }
  }
  HAL_I2S_DMAStop(&hi2s3);
}