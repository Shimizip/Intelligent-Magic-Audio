/**
 * @file wavPlayer.h
 * @brief Header file for WAV audio player functionality.
 * 
 * Defines the structures and function prototypes for handling WAV file playback.
 */

#ifndef INC_WAVPLAYER_H_
#define INC_WAVPLAYER_H_

#include <stdint.h>
#include "fatfs.h"
#include "audio.h"

/**
 * @brief Represents the header of a WAV file.
 * 
 * This structure defines the layout of the WAV file header, which includes information
 * about the RIFF chunk, format details, and the data chunk. It provides essential metadata
 * for interpreting the WAV file's audio data.
 */
typedef struct wav_header_t {
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

/**
 * @brief Represents a WAV audio player with playback control and state management.
 * 
 * This structure contains information and control flags related to WAV audio playback,
 * including file handling, playback status, and pitch adjustment.
 */
typedef struct WavPlayer {
    volatile bool restartPlayback;
    volatile bool playbackActive;
    FIL *file;
    wav_header_t *wavHeader;
    uint32_t headerSize;
    float pitchFactor;
    bool pitchChanged;
} WavPlayer;

void initPlayer(WavPlayer *player, FIL *file, wav_header_t *wavHeader);

FRESULT wavLoad(WavPlayer *player,const char *filename);

uint32_t populateWavHeader(FIL *file, wav_header_t *wavHeader);
uint8_t checkWav(WavPlayer *player);

uint8_t wavPlay(WavPlayer *player);
uint8_t wavPlayPitched(WavPlayer *player);

void playButtonHandler(WavPlayer *player);

void wavStop(void);

#endif /* INC_WAVPLAYER_H_ */