#include "audioPreprocessor.h"

static float32_t firState[NUM_TAPS + BLOCK_SIZE - 1];  // State buffer for FIR filter
arm_fir_decimate_instance_f32 FIR_F32_Struct;


// Function to get the current cycle count
uint32_t DWT_GetCycleCount(void) {
    return DWT->CYCCNT;
}


arm_status initFilter(){
    return arm_fir_decimate_init_f32(&FIR_F32_Struct, (uint16_t) NUM_TAPS, (uint8_t) DECIMATION_FACTOR_ROUNDED, filter_taps, firState, BLOCK_SIZE);
}

// returns the number of bytes written to resampled file
uint16_t resampleFile(FIL *inFil, FIL *outFil, wav_header_t *waveHeaderInput, wav_header_t *waveHeaderResampled){
    
    int16_t inputChunk[NUM_SAMPLES_CHUNK_IN];
    int16_t *currentInBlock;
    int16_t currentInBlockMono[BLOCK_SIZE];
    int16_t outputChunk[NUM_SAMPLES_CHUNK_OUT];

    for(int i = 0; i < NUM_SAMPLES_CHUNK_OUT; i ++){
        outputChunk[i] = 0;
    }

    UINT br;
    UINT bw;


    // for writing whole file to sd card
    uint32_t length = waveHeaderInput->Subchunk2Size ;    // Number of bytes in data. Number of samples * num_channels * sample byte size
    uint32_t remainingBytes = length;
    uint32_t bytesRead = 0;
    uint32_t bytesWritten = 0;
    // copy full file

    // populate outputWavHeader with data from original file.
    // after that change fields according to output file format
    uint32_t headerSize = populateWavHeader(inFil, waveHeaderResampled);

    while (remainingBytes > 0) {
        // fill input Chunk Buffer
        f_read(inFil, inputChunk, RESAMPLE_CHUNK_IN_SIZE, &br);
        bytesRead += br; 
        // fill outputChunk Buffer - Block by Block
        uint32_t block_index = 0;
        uint32_t num_blocks = NUM_SAMPLES_CHUNK_IN / (BLOCK_SIZE * NUM_CHANNELS);
        // loop through inputChunk and generate according outputchunk
        while (block_index < num_blocks) {
            // set pointer to current Block within Input Chunk
            currentInBlock = &inputChunk[block_index * BLOCK_SIZE * NUM_CHANNELS];

            // sum up l and r to mono
            for(int n = 0; n < BLOCK_SIZE; n++){
                currentInBlockMono[n] = (currentInBlock[n*2] + currentInBlock[n*2 + 1]) / 2;
            }
            
            downsample_Block(currentInBlockMono, &outputChunk[block_index * (BLOCK_SIZE/ DECIMATION_FACTOR_ROUNDED)]);

            // Move to the next block
            block_index++;
            // output_index++;
        }

        if (br > remainingBytes) {
            break;
        } else {
            remainingBytes -= br;
        }
        f_write(outFil, outputChunk, RESAMPLE_CHUNK_OUT_SIZE, &bw);
        bytesWritten += bw;
        if (bw != BLOCK_SIZE / DECIMATION_FACTOR_ROUNDED * sizeof(float32_t)) {
            // Handle write error
        }
    }

    // alter new headerMetadata to new .wav Format
    waveHeaderResampled->Subchunk2Size = bytesWritten;
    waveHeaderResampled->SampleRate = waveHeaderInput->SampleRate / DECIMATION_FACTOR_ROUNDED;
    waveHeaderResampled->ByteRate = waveHeaderResampled->SampleRate * waveHeaderResampled->NumChannels * waveHeaderResampled->BitsPerSample / 8;
    waveHeaderResampled->NumChannels = 1;

    // write new header to resampled file
    f_lseek(outFil, 0);
    f_write(outFil, waveHeaderResampled, headerSize, &bw);
    
    return bytesWritten;
}

// takes and produces int16_t
void downsample_Block(int16_t *src, int16_t *dest){
        // Temporary buffer for float input data for l and r channel
        float32_t inputFilterMono[BLOCK_SIZE];

        // Temporary buffer for float output data
        float32_t outputFilterMono[ BLOCK_SIZE / DECIMATION_FACTOR_ROUNDED];
    
        // Convert int16_t input data to float32_t
        arm_q15_to_float(src, inputFilterMono, BLOCK_SIZE);

        // Perform FIR filtering and decimation on merged mono input
        arm_fir_decimate_f32(&FIR_F32_Struct, inputFilterMono, outputFilterMono, BLOCK_SIZE);

        // Copy filtered output block to chunk buffer
        arm_float_to_q15(outputFilterMono, dest, (BLOCK_SIZE /  DECIMATION_FACTOR_ROUNDED));
}

// get number of 
// trow away floating point data after comma
uint16_t get_number_subsamples(FIL *file){
    wav_header_t header;
    populateWavHeader(file, &header);

    uint32_t audio_data = header.Subchunk2Size;

    // calculate number of mono samples in downsampled samplerate
    uint32_t num_samples = ((audio_data / sizeof(int16_t)) / header.NumChannels) / DECIMATION_FACTOR_ROUNDED; 
    uint32_t num_frames = (num_samples - HOP_SIZE) / STEP_SIZE;
    return num_frames;
}

// takes an input sample array with number of samples:  
// NUM_SAMPLES_CHUNK_OUT * DECIMATION_FACTOR_ROUNDED = NUM_SAMPLES_CHUNK_IN
// returns the actual number of samples downsampled 
uint32_t downsample_chunk_to_1024_samples(FIL *file, int16_t outChunk[NUM_SAMPLES_CHUNK_OUT]){
        
        // input buffers
        int16_t inputChunk[NUM_SAMPLES_CHUNK_IN];
        int16_t *currentInBlock;
        int16_t currentInBlockMono[BLOCK_SIZE];

        UINT br;

        f_read(file, inputChunk, RESAMPLE_CHUNK_IN_SIZE, &br);
        uint32_t samples_read = br * sizeof(int16_t);
        
        // fill outputChunk Buffer - Block by Block
        uint32_t block_index = 0;
        uint32_t num_blocks = samples_read / (BLOCK_SIZE * NUM_CHANNELS);
        // loop through inputChunk and generate according outputchunk
        while (block_index < num_blocks) {
            // set pointer to current Block within Input Chunk
            currentInBlock = &inputChunk[block_index * BLOCK_SIZE * NUM_CHANNELS];

            // sum up l and r to mono
            for(int n = 0; n < BLOCK_SIZE; n++){
                currentInBlockMono[n] = (currentInBlock[n*2] + currentInBlock[n*2 + 1]) / 2;
            }
            
            downsample_Block(currentInBlockMono, &outChunk[block_index * (BLOCK_SIZE/ DECIMATION_FACTOR_ROUNDED)]);

            // Move to the next block
            block_index++;
        }
        // return 
}