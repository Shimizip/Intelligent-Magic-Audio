#include <stdint.h>
#include "math.h"
#include "fatfs.h"

#include "audio.h"
#include "lowpass_16kFilter.h"

#define RESAMPLE_CHUNK_OUT_SIZE 16000
#define RESAMPLE_CHUNK_IN_SIZE  44100/8 // 1/8 second

static int16_t resampleChunkBuf[RESAMPLE_CHUNK_OUT_SIZE];

void resample(int16_t *resampleChunkBuf, FIL *fil, lowpass_16kFilter* f){
    lowpass_16kFilter_init(f);

    float decimation_factor = 44100.0 / 16000.0;
    float input_index = 0.0;
    uint32_t output_index = 0;
    float sample;

    int16_t input[4096];
    UINT btr;


    while(output_index < RESAMPLE_CHUNK_OUT_SIZE){

        // read input file buffer
        f_read(fil, input, RESAMPLE_CHUNK_IN_SIZE, &btr);
        input_index = 0;

        // actual downsampling
        while ((uint32_t)round(input_index) < RESAMPLE_CHUNK_IN_SIZE) {
            // Round the input index to the nearest integer
            uint32_t nearest_index = (uint32_t)round(input_index);
            
            // Ensure we do not go out of bounds
            if (nearest_index >= RESAMPLE_CHUNK_IN_SIZE) {
                break;
            }

            // Select the nearest sample    
            sample = input[nearest_index];

            // filter sample
            lowpass_16kFilter_put(f, sample);
            sample = lowpass_16kFilter_get(f);

            resampleChunkBuf[output_index++] = (uint16_t)sample;

            // Increment the input index by the decimation factor
            input_index += decimation_factor;
        }
    }
}