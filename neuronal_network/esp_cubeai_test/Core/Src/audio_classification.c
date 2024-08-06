/**
 * @file audio_classification.c
 * @brief Audio classification implementation for embedded systems via stm32cube.ai (for testing purposes).
 *
 * This file implements the methods and algorithms required for audio classification using neural networks on embedded systems via stm32cube.ai. It includes initialization and execution of a neural network, preprocessing of audio data to generate spectrograms, and the handling of neural network output.
 *
 * @date June 20, 2024
 * @author Leon Braungardt
 */

#include "audio_classification.h"
#include "featurescaler.h"
#include <math.h>
#include "feature_extraction.h"
#include "network_1.h"
#include "network_1_data.h"
#include "network_1_config.h"
#include "network_1_data_params.h"
#include "utils.h"

/**
 * @var ai_handle network
 * @brief Handle to the neural network (stm32cube.ai) instance used for audio classification.
 */
ai_handle network;

/**
 * @var ai_u8 activations[AI_NETWORK_1_DATA_ACTIVATIONS_SIZE]
 * @brief Buffer for storing activation data required by stm32cube.ai.
 */
ai_u8 activations[AI_NETWORK_1_DATA_ACTIVATIONS_SIZE];

/**
 * @var ai_buffer* ai_input
 * @brief Pointer to the input buffer of the stm32cube.ai.
 */
ai_buffer * ai_input;

/**
 * @var ai_buffer* ai_output
 * @brief Pointer to the output buffer of stm32cube.ai, where classification results are stored.
 */
ai_buffer * ai_output;

/**
 * @var ai_float spectrogram[AI_NETWORK_1_IN_1_SIZE]
 * @brief Array to hold the input spectrogram data that feeds into the neural network.
 */
ai_float spectrogram[AI_NETWORK_1_IN_1_SIZE];

/**
 * @var float aiOutData[AI_NETWORK_1_OUT_1_SIZE]
 * @brief Output data from the neural network, containing classification results.
 */
float aiOutData[AI_NETWORK_1_OUT_1_SIZE] = {0.0, 0.0, 0.0, 0.0, 0.0};

/**
 * @var uint32_t spectrogram_col_index
 * @brief Index to track the current column in the spectrogram buffer during data preprocessing.
 */
uint32_t spectrogram_col_index = 0;

/**
 * @var float32_t mel_spectrogram_column_buffer[SPECTROGRAM_ROWS]
 * @brief Buffer to store a single column of the Mel spectrogram during preprocessing.
 */
float32_t mel_spectrogram_column_buffer[SPECTROGRAM_ROWS];

/**
 * @var static float32_t frame[FRAME_LENGTH]
 * @brief Temporary buffer to hold a single audio frame for processing.
 */
static float32_t frame[FRAME_LENGTH];

/**
 * @var static uint32_t cpu_cycles
 * @brief Counter for the CPU cycles used during the neural network processing, used for time (performance) measurement.
 */
static uint32_t cpu_cycles;

/**
 * @var static uint32_t time_millis
 * @brief Milliseconds elapsed during the neural network processing, calculated from cpu_cycles for performance analysis.
 */
static uint32_t time_millis;


/**
 * @brief Initializes the neural network for audio classification.
 *
 * This function sets up the neural network by creating an instance of the model and initializing it with the required activation buffers. It retrieves the necessary input and output buffers for processing the audio data. Error handling is incorporated to manage potential initialization failures.
 *
 * @return int Returns 0 on successful initialization, or -1 if an error occurs during the network creation or initialization.
 *
 * @note Drived from https://wiki.st.com/stm32mcu/wiki/AI:How_to_perform_motion_sensing_on_STM32L4_IoTnode#Create_an_STM32Cube-AI_application_using_X-CUBE-AI
 */
int init_nn() {
	ai_error err;

	/* Create a local array with the addresses of the activations buffers */
	const ai_handle act_addr[] = { activations };
	/* Create an instance of the model */
	err = ai_network_1_create_and_init(&network, act_addr, NULL);
	if (err.type != AI_ERROR_NONE) {
		return -1;
	}

	// initialize ai_input and ai_output variables of type ai_buffer
	ai_input = ai_network_1_inputs_get(network, NULL);
	ai_output = ai_network_1_outputs_get(network, NULL);

	return 0;
}

/**
 * @brief Runs the neural network to classify audio data based on the provided spectrogram.
 *
 * This function takes a pointer to a spectrogram array, processes it by normalizing with pre-calculated mean and standard deviation values, and then feeds it into the neural network. The output from the neural network is stored in the provided classification_result array.
 *
 * @param ai_float* pSpectrogram Pointer to the input spectrogram array.
 * @param ai_float* classification_result Pointer to the array where the neural network's output (classification results) will be stored.
 *
 * @return int Returns 0 on successful classification, or -1 if an error occurs during the network run or if the network handle is null.
 *
 */
int run_nn_classification(ai_float* pSpectrogram, ai_float* classification_result) {
    ai_i32 batch;
    ai_error err;

    // set inout and output pointers
    ai_input[0].data = AI_HANDLE_PTR(pSpectrogram);
    ai_output[0].data = AI_HANDLE_PTR(classification_result);

    if(network == AI_HANDLE_NULL)
    {
    	return -1;
    }

    // before classification, imitate the standardization by applying the during training exported feature scaler
    for (int i = 0; i < AI_NETWORK_1_IN_1_SIZE; i++) {
    	pSpectrogram[i] = (pSpectrogram[i] - feature_scaler_mean[i]) / feature_scaler_std[i];
	}

    // Run the network
    batch = ai_network_1_run(network, ai_input, ai_output);
    if (batch != 1) {
        err = ai_network_1_get_error(network);
        return -1;
    }

    return 0;
}


/**
 * @brief Tests the classification process by simulating the generation and processing of a spectrogram without actually requiring input data. Was used for previous tests.
 *
 * @note The loop fixed at 32 iterations represents a placeholder for actual Mel spectrogram calculation based on audio input.
 */
void test_call_classification() {
	for (int i = 0; i < N_FRAMES_PER_SUBSAMPLE; i++) {
		// "generate" spectrogram data
		for (int j = 0; j < 32; j++) {
			mel_spectrogram_column_buffer[j] = j * 1.1;
		}

		spectrogram_col_index++;

		// copy spectrogram column to spectrogram var
		for (uint32_t k = 0; k < SPECTROGRAM_ROWS; k++) {
			spectrogram[k * SPECTROGRAM_COLS + spectrogram_col_index] = mel_spectrogram_column_buffer[k];
		}
	}
	// continue with classification
	process_subsample(spectrogram);
}

/**
 * @brief Initializes the mel-spectrogram calculation lib which is part of the audio preprocessing.
 *
 * This function sets up the components needed for generating Mel spectrograms from audio data.
 *
 * @note Derived from the FP-AI-SENSING feature pack: https://www.st.com/en/embedded-software/fp-ai-sensing1.html
 */
void Preprocessing_Init(void) {
  /* Init RFFT */
  arm_rfft_fast_init_f32(&S_Rfft, 1024);

  /* Init Spectrogram */
  S_Spectr.pRfft    = &S_Rfft;
  S_Spectr.Type     = SPECTRUM_TYPE_POWER;
  S_Spectr.pWindow  = (float32_t *) hannWin_1024;
  S_Spectr.SampRate = 16000;
  S_Spectr.FrameLen = 1024;
  S_Spectr.FFTLen   = 1024;
  S_Spectr.pScratch = mel_spectrogram_column_buffer;

  /* Init Mel filter */
  S_MelFilter.pStartIndices = (uint32_t *) melFiltersStartIndices_1024_30;
  S_MelFilter.pStopIndices  = (uint32_t *) melFiltersStopIndices_1024_30;
  S_MelFilter.pCoefficients = (float32_t *) melFilterLut_1024_30;
  S_MelFilter.NumMels       = 30;

  /* Init MelSpectrogram */
  S_MelSpectr.SpectrogramConf = &S_Spectr;
  S_MelSpectr.MelFilter       = &S_MelFilter;
}

/**
 * @brief Splits an audiosubsample into overlapping frames ready for further processing.
 *
 * This function segments an audiosubsample into frames 1024 samples with a 512 sample overlap, preparing them for feature extraction (spectrogram calculation). Each frame is extracted based on the hop length and frame length definitions, with zero-padding applied where necessary. The frames are then passed to the `preprocess_frame` function for following processing steps.
 *
 * @param float32_t* subsample Pointer to the array containing the audio subsample data.
 */
void frame_subsamples(float32_t* subsample) {
	int start_index = 0;
	// iterate over all frames in one subsample
    for (int i = 0; i < N_FRAMES_PER_SUBSAMPLE; i++) {
    	// copy the frame of 1024 to the frame variable (sliding window)
        start_index = i * HOP_LENGTH;
        for (int j = 0; j < FRAME_LENGTH; j++) {
            if (start_index + j < N_SAMPLES_PER_SUBSAMPLE) {
                frame[j] = subsample[start_index + j];
            } else {
                frame[j] = 0.0f; // pad with zeros
            }
        }
        preprocess_frame(frame);
    }
}

/**
 * @brief Processes a single audio frame and incorporates it into the spectrogram.
 *
 * This function takes an audio frame, computes its Mel-scaled spectrogram column, and appends this column to the ongoing spectrogram array. Once the spectrogram is filled with the number of columns determined by SPECTROGRAM_COLS, it triggers the calculation of the spectrogram.
 *
 * @param float* pFrame Pointer to the array containing the audio frame data, which is 1024 samples in length.
 *
 * @note This function updates the global `spectrogram_col_index` to manage the position within the spectrogram array. When the spectrogram array is full, it calls `process_subsample` to handle the filled spectrogram and resets the index for new data.
 */
void preprocess_frame(float* pFrame) {
	// Create a Mel-scaled spectrogram column
	MelSpectrogramColumn(&S_MelSpectr, pFrame, mel_spectrogram_column_buffer);

	// Reshape and copy into output spectrogram
	for (uint32_t i = 0; i < SPECTROGRAM_ROWS; i++) {
		spectrogram[i * SPECTROGRAM_COLS + spectrogram_col_index] = mel_spectrogram_column_buffer[i];
	}

    spectrogram_col_index++;

    // If we have a full spectrogram (32 columns), process it
    if (spectrogram_col_index == SPECTROGRAM_COLS) {
    //if (spectrogram_col_index == 1) {
    	process_subsample(spectrogram);
        // Reset the column index for the next spectrogram
        spectrogram_col_index = 0;
    }
}

/**
 * @brief Processes a complete spectrogram, runs classification, and measures performance.
 *
 * This function is responsible for converting a power spectrogram into decibel scale, running the neural network to classify the audio data, and measuring the processing time in CPU cycles and milliseconds. It performs a series of operations: conversion of spectrogram values with the predefined scaler (imitating the standardization), neural network classification and performance tracking. The classification results are then contained in aiOutData.
 *
 * @param float* pSpectrogram Pointer to the array holding the complete spectrogram data that will be processed and classified.
 *
 */
void process_subsample(float* pSpectrogram) {
	// Convert power spectrogram to dB
	PowerToDb(spectrogram);

	StartCycleMeasurement();

	// Run the neural network classification
	if (run_nn_classification(spectrogram, aiOutData) == 0) {
		StopCycleMeasurement();

		cpu_cycles = GetMeasuredCycles();
		time_millis = CyclesToMilliseconds(cpu_cycles);
		/* Output results */
		for (uint32_t i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
		  printf("%8.6f ", aiOutData[i]); //placeholder for breakpoint
		}
	}
}

/**
 * @brief Converts power values in a spectrogram to decibels (dB).
 *
 * This function scans the spectrogram for the maximum power value to use as a reference for converting all power values to dB scale. It handles special cases where the maximum power is zero (silence) by setting all dB values to -80 dB to avoid division by zero. Values below -80 dB or resulting in NaN are also set to -80 dB to ensure numerical stability.
 *
 * @param float *pSpectrogram Pointer to the spectrogram data array.
 *
 */
void PowerToDb(float *pSpectrogram) {
    float max_mel_energy = FLT_MIN; // minimal possible value to ensure it gets overridden
    uint32_t rows = SPECTROGRAM_ROWS;
    uint32_t cols = SPECTROGRAM_COLS;
    uint32_t i, total_elements = rows * cols;

    // Find MelEnergy Scaling factor
    for (i = 0; i < total_elements; i++) {
        if (pSpectrogram[i] > max_mel_energy) {
            max_mel_energy = pSpectrogram[i];
        }
    }

    // Check if max_mel_energy is zero to avoid division by zero
    if (max_mel_energy == 0.0f) {
        // If max_mel_energy is zero, the spectrogram is all zeros, and we can directly set the output to -80 dB
        for (i = 0; i < total_elements; i++) {
        	pSpectrogram[i] = -80.0f;
        }
        return;
    }

    // Scale Mel Energies and convert to dB
    for (i = 0; i < total_elements; i++) {
    	pSpectrogram[i] = 10.0f * log10f(pSpectrogram[i] / max_mel_energy);
        // Threshold to -80 dB
        if (pSpectrogram[i] < -80.0f) {
        	pSpectrogram[i] = -80.0f;
        }
        // Check for nan and replace with -80
        if (isnan(pSpectrogram[i])) {
        	pSpectrogram[i] = -80.0f;
        }
    }
}
