#ifndef INC_WAVDECODER_H_
#define INC_WAVDECODER_H_

#include <stdint.h>
#include "fatfs.h"
#include "audio.h"

#define WAV_PCMSamples  8192

typedef struct wav_header {
    // RIFF Header
    uint32_t ChunkID;         // Contains "RIFF"
    uint32_t ChunkSize;       // Size of the wav portion of the file, which follows the first 8 bytes. File size - 8
    uint32_t Format;          // Contains "WAVE"

    // Format Header
    uint32_t Subchunk1ID;       // Contains "fmt " (includes trailing space)
    uint32_t Subchunk1Size;     // Should be 16 for PCM
    uint16_t AudioFormat;       // Should be 1 for PCM. 3 for IEEE Float
    uint16_t NumChannels ;
    uint32_t SampleRate;
    uint32_t ByteRate;        // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    uint16_t BlockAlign;      // num_channels * Bytes Per Sample
    uint16_t BitsPerSample;     // Number of bits per sample

    // Data
    uint32_t Subchunk2ID;       // Contains "data"
    uint32_t Subchunk2Size ;    // Number of bytes in data. Number of samples * num_channels * sample byte size
    // uint8_t bytes[];       // Remainder of wave file is bytes
} wav_header_t;

uint8_t checkWav(FIL *file, wav_header_t *wavHeader);

uint8_t wavPlay(FIL *file, wav_header_t *wavHeader);
uint8_t wavPlayPitched(FIL *file, wav_header_t *wavHeader);

void wavStop(void);

#endif /* INC_WAVDECODER_H_ */