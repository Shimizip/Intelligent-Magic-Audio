/*
 * audio_classification.c
 *
 *  Created on: Jun 20, 2024
 *      Author: leon
 */

#include "audio_classification.h"
#include <math.h>
#include "feature_extraction.h"
#include "network_1.h"
#include "network_1_data.h"
#include "network_1_config.h"
#include "network_1_data_params.h"

ai_handle network;
ai_u8 activations[AI_NETWORK_1_DATA_ACTIVATIONS_SIZE];
ai_buffer * ai_input;
ai_buffer * ai_output;
ai_float spectrogram[AI_NETWORK_1_IN_1_SIZE];

float aiOutData[AI_NETWORK_1_OUT_1_SIZE] = {0.0, 0.0, 0.0, 0.0, 0.0};
uint32_t spectrogram_col_index = 0;
float32_t mel_spectrogram_column_buffer[SPECTROGRAM_ROWS];

static float32_t frame[FRAME_LENGTH];

// Function to initialize the neural network
int init_nn() {
	ai_error err;

	/* Create a local array with the addresses of the activations buffers */
	const ai_handle act_addr[] = { activations };
	/* Create an instance of the model */
	err = ai_network_1_create_and_init(&network, act_addr, NULL);
	if (err.type != AI_ERROR_NONE) {
		printf("ai_network_create error - type=%d code=%d\r\n", err.type, err.code);
		return -1;
	}
	ai_input = ai_network_1_inputs_get(network, NULL);
	ai_output = ai_network_1_outputs_get(network, NULL);

	return 0;
}

// Function to run the neural network and classify input data
int run_nn_classification(ai_float* pSpectrogram, ai_float* classification_result) {
    ai_i32 batch;
    ai_error err;

    ai_input[0].data = AI_HANDLE_PTR(pSpectrogram);
    ai_output[0].data = AI_HANDLE_PTR(classification_result);

    if(network == AI_HANDLE_NULL)
      {
          return -1;
      }


    // Run the network
    batch = ai_network_1_run(network, ai_input, ai_output);
    if (batch != 1) {
        err = ai_network_1_get_error(network);
        printf("AI ai_network_run error - type=%d code=%d\r\n", err.type, err.code);
        return -1;
    }

    return 0; // Success
}


void test_call_classification(float* input_audio_data) {
	for (int i = 0; i < N_FRAMES_PER_SUBSAMPLE; i++) {
		for (int j = 0; j < 32; j++) {
			mel_spectrogram_column_buffer[j] = j * 1.0;
		}

		spectrogram_col_index++;
		for (uint32_t k = 0; k < SPECTROGRAM_ROWS; k++) {
			spectrogram[k * SPECTROGRAM_COLS + spectrogram_col_index] = mel_spectrogram_column_buffer[k];
		}
	}
	process_subsample(spectrogram);
}

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

// splits one subsample in 1024 samples long, 512 samples overlapping frames
void frame_subsamples(float32_t* subsample, int subsample_length) {
    for (int i = 0; i < N_FRAMES_PER_SUBSAMPLE; i++) {
        int start_index = i * HOP_LENGTH;
        for (int j = 0; j < FRAME_LENGTH; j++) {
            if (start_index + j < subsample_length) {
                frame[j] = subsample[start_index + j];
            } else {
                frame[j] = 0.0f; // pad with zeros
            }
        }
        preprocess_frame(frame);
    }
}

// frame = 1 spectrogram column = 1024 samples
void preprocess_frame(float* pFrame) {
	// Create a Mel-scaled spectrogram column
	MelSpectrogramColumn(&S_MelSpectr, pFrame, mel_spectrogram_column_buffer);

	// Reshape and copy into output spectrogram
	for (uint32_t i = 0; i < SPECTROGRAM_ROWS; i++) {
		int idx = i * SPECTROGRAM_COLS + spectrogram_col_index;
		if (idx > 960) {
			printf("bla");
		}
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

void process_subsample(float* pSpectrogram) {
	// Convert power spectrogram to dB
	PowerToDb(spectrogram);

	// Run the neural network classification
	if (run_nn_classification(spectrogram, aiOutData) == 0) {
		/* Output results */
		for (uint32_t i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
		  printf("%8.6f ", aiOutData[i]);
		}
	}
}

void PowerToDb(float *pSpectrogram) {
    float max_mel_energy = FLT_MIN; // Minimaler positiver Wert, um sicherzustellen, dass er überschrieben wird
    uint32_t rows = 30;
    uint32_t cols = 32;
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
