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
#include "featurescaler.h"
#include "network_1.h"
#include "network_1_data.h"
#include "network_1_config.h"
#include "network_1_data_params.h"

/* defines */
// define spectrogram dimensions
#define NUM_MEL_BANDS 30
#define SPECTROGRAM_ROWS NUM_MEL_BANDS
#define SPECTROGRAM_COLS 32

// define data preprocessing variables
#define TARGET_SAMPLE_RATE 16000
#define FRAME_LENGTH 1024
#define HOP_LENGTH 512
#define N_SAMPLES_PER_SUBSAMPLE 16896
#define N_FRAMES_PER_SUBSAMPLE SPECTROGRAM_COLS


/* function definitions */
int init_nn(void);
int de_init_nn(void);
int run_nn_classification(float* pSpectrogram, float* classification_result);
void spectrogram_generation_init(void);
void classify_file(FIL *fil, char* file_name)
void calculate_spectrogram_column(float* frame, int col_index);
void calculate_total_classification_result(float* total_file_classification_result, float classification_results_subsamples[][AI_NETWORK_1_OUT_1_SIZE], int number_subsamples_in_file);
void spectrogram_power_to_db(float32_t *spectrogram);
void store_classification_result(FIL *fil, char* file_name, float* classification_result)

// stm32cube.ai related variables
extern ai_handle network;
extern ai_u8 activations[AI_NETWORK_1_DATA_ACTIVATIONS_SIZE];
extern ai_buffer * ai_input;
extern ai_buffer * ai_output;

// own input and output variables for nn classification
extern float spectrogram[AI_NETWORK_1_IN_1_SIZE];
extern float aiOutData[AI_NETWORK_1_OUT_1_SIZE];

// variable for mapping aiOutData indices to the actual labels
extern char aiOutDataLabels[AI_NETWORK_1_OUT_1_SIZE][10];


// variables for spectrogram generation parameters
static arm_rfft_fast_instance_f32 S_Rfft;
static SpectrogramTypeDef         S_Spectr;
static MelSpectrogramTypeDef      S_MelSpectr;
static MelFilterTypeDef           S_MelFilter;

#endif
