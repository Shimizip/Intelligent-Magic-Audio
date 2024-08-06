/*
 * audio_classification.h
 *
 *  Created on: Jun 20, 2024
 *      Author: leon
 */

#ifndef AUDIO_CLASSIFICATION_H_
#define AUDIO_CLASSIFICATION_H_

#include "feature_extraction.h"
#include "network_1.h"
#include "network_1_data.h"
#include "network_1_config.h"
#include "network_1_data_params.h"


#define NUM_MEL_BANDS 30
#define SPECTROGRAM_ROWS NUM_MEL_BANDS
#define SPECTROGRAM_COLS 32

#define TARGET_SAMPLE_RATE 16000
#define FRAME_LENGTH 1024
#define HOP_LENGTH 512
#define N_SAMPLES_PER_SUBSAMPLE 16384 // TARGET_SAMPLE_RATE * (FRAME_LENGTH * 0.001)
#define N_FRAMES_PER_SUBSAMPLE SPECTROGRAM_COLS




int init_nn(void);
int run_nn_classification(float* pSpectrogram, float* classification_result);
void test_call_classification(float* audio_data);
void Preprocessing_Init(void);
int resample_audio(const short* input_data, int input_length, int input_sample_rate, float32_t* output_data, int* output_length);
void frame_subsamples(float32_t* subsample, int subsample_length);
void preprocess_frame(float* frame);
void process_subsample(float* pSpectrogram);
void PowerToDb(float32_t *spectrogram);


extern ai_handle network;
extern ai_u8 activations[AI_NETWORK_1_DATA_ACTIVATIONS_SIZE];
extern ai_buffer * ai_input;
extern ai_buffer * ai_output;

extern ai_float aiOutData[AI_NETWORK_1_OUT_1_SIZE];
extern ai_float spectrogram[AI_NETWORK_1_IN_1_SIZE];

extern uint32_t spectrogram_col_index;

// spectrogram stuff
static arm_rfft_fast_instance_f32 S_Rfft;
static SpectrogramTypeDef         S_Spectr;
static MelSpectrogramTypeDef      S_MelSpectr;
static MelFilterTypeDef           S_MelFilter;

#endif
