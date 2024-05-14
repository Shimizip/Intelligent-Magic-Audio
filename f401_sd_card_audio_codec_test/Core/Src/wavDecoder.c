#include "fatfs.h"
#include "wavDecoder.h"
#include <malloc.h>

// check for valid .wav header
uint8_t checkWav(FIL *file){
  uint8_t wav_OK = 1;
  //char WavInfo[36] = "WAV Info: ";
  wav_header_t wavHeader;
  UINT count = 0;

  if( (f_read(file, &wavHeader, 44, &count) != FR_OK) || count<44){           // Read wav header
    return 0;
  }
  if(wavHeader.ChunkID != 0x46464952 ){   // "RIFF"
    wav_OK = 0;
  }
  if(wavHeader.Format != 0x45564157 ){    // "WAVE"
    wav_OK = 0;
  }
  if(wavHeader.Subchunk1ID != 0x20746d66 ){ // "fmt "
    wav_OK = 0;
  }
  if(wavHeader.Subchunk1Size != 16 ){     // Should be 16
    wav_OK = 0;
  }
  if(wavHeader.AudioFormat != 1 ){      // Should be 1
    wav_OK = 0;
  }
  if((wavHeader.NumChannels != 2) || (wavHeader.SampleRate!=44100) || (wavHeader.BitsPerSample != 16 ) || (wavHeader.Subchunk2ID != 0x61746164 ) ){
    wav_OK = 0;
  }
  return wav_OK;
}