/*
 * audio_classification.h
 *
 *  Created on: Jun 20, 2024
 *      Author: leon
 */

#ifndef AUDIO_CLASSIFICATION_H_
#define AUDIO_CLASSIFICATION_H_

/* includes */
#include "feature_extraction.h"
#include "network_1.h"
#include "network_1_data.h"
#include "network_1_config.h"
#include "network_1_data_params.h"

/* defines */
#define NUM_MEL_BANDS 30
#define SPECTROGRAM_ROWS NUM_MEL_BANDS
#define SPECTROGRAM_COLS 32

#define TARGET_SAMPLE_RATE 16000
#define FRAME_LENGTH 1024
#define HOP_LENGTH 512
#define N_SAMPLES_PER_SUBSAMPLE 16384 // TARGET_SAMPLE_RATE * (FRAME_LENGTH * 0.001)
#define N_FRAMES_PER_SUBSAMPLE SPECTROGRAM_COLS



/* function definitions */
int init_nn(void);
int run_nn_classification(float* pSpectrogram, float* classification_result);
void test_call_classification(float* audio_data);
void spectrogram_generation_init(void);
int resample_audio(const short* input_data, int input_length, int input_sample_rate, float32_t* output_data, int* output_length);
void classify_file(FIL *fil, char* file_name)
void calculate_spectrogram_column(float* frame, int col_index);
void calculate_total_classification_result(float* total_file_classification_result, float classification_results_subsamples[][AI_NETWORK_1_OUT_1_SIZE], int number_subsamples_in_file);
void spectrogram_power_to_db(float32_t *spectrogram);
void store_classification_result(float* result, char* file_name, FIL *fil);

// Cube.AI related variables
extern ai_handle network;
extern ai_u8 activations[AI_NETWORK_1_DATA_ACTIVATIONS_SIZE];
extern ai_buffer * ai_input;
extern ai_buffer * ai_output;

// custom variables for Cube.AI input/output
extern float aiOutData[AI_NETWORK_1_OUT_1_SIZE];
extern char aiOutDataLabels[AI_NETWORK_1_OUT_1_SIZE][10];
extern float spectrogram[AI_NETWORK_1_IN_1_SIZE];

// variables for spectrogram generation parameters
static arm_rfft_fast_instance_f32 S_Rfft;
static SpectrogramTypeDef         S_Spectr;
static MelSpectrogramTypeDef      S_MelSpectr;
static MelFilterTypeDef           S_MelFilter;

#endif
