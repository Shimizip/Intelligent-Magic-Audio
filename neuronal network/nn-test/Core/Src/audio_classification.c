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
char aiOutDataLabels[AI_NETWORK_1_OUT_1_SIZE][10] = {
		"bass",
        "pitch",
        "sustained",
        "rhythmic",
        "melodic"
};

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

void classify_file(FIL *fil, char* file_name) {
	int16_t frame_buffer[FRAME_LENGTH];
	int16_t hop_buffer[HOP_LENGTH];
	int16_t resampled_chunk_data[FRAME_LENGTH];

	float total_file_classification_result[AI_NETWORK_1_OUT_1_SIZE];

	// reset all vars
	// TODO if necessary

	uint16_t number_subsamples_in_file = get_number_subsamples();
	float classification_results_subsamples[number_subsamples_in_file][AI_NETWORK_1_OUT_1_SIZE];

	// if number subsamples in file is zero => return empty classification result
	if (number_subsamples_in_file == 0) {
		for (int i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
			total_file_classification_result[i] = 0.0;
		}
	}

	// loop over all subsamples of file
	for (int subsample_count = 0; i < number_subsamples_in_file; i++) {
		// generate spectrogram for subsample (sliding window; window size = frame size = 1024; hop size 512)
		downsample_1024_samples(fil, resampled_chunk_data);
		for (int spectrogram_col_index = 0; i < SPECTROGRAM_COLS; i++) {
			if (spectrogram_col_index % 2 == 0) {
				// mod 2 == 0 => frame buffer is the read chunk; no need to read new data
				// copy chunk to frame buffer
				for (int n = 0; n < FRAME_LENGTH; i++) {
					frame_buffer[n] = resampled_chunk_data[n];
				}
				// hop buffer is the second half of the read chunk
				for (int n = 512; n < FRAME_LENGTH; i++) {
					hop_buffer[n] = resampled_chunk_data[n];
				}
			} else {
				// mod 2 == 1 => frame buffer is the hop buffer plus the first half of the chunk; read new data
				// first half of frame is the hop buffer
				for (int n = 0; n < HOP_LENGTH; i++) {
					frame_buffer[n] = hop_buffer[n];
				}
				// second half of frame is first half of newly read data
				for (int n = 0; n < HOP_LENGTH; i++) {
					frame_buffer[n+512] = resampled_chunk_data[n];
				}
				// read next chunk
				downsample_1024_samples(fil, resampled_chunk_data);
			}
			calculate_spectrogram_column(frame_buffer);
		}
		// Convert power spectrogram to dB
		spectrogram_power_to_db(spectrogram);

		// Run the neural network classification
		run_nn_classification(spectrogram, aiOutData);
		for (int i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
			classification_results_subsamples[subsample_count][i] = aiOutData[i];
		}
	}

	calculate_total_classification_result(total_file_classification_result, classification_results_subsamples, number_subsamples_in_file);
	store_classification_result();
}

// initializes all spectrogram generation related parameters
void spectrogram_generation_init(void) {
  /* Init RFFT */
  arm_rfft_fast_init_f32(&S_Rfft, 1024);

  /* Init Spectrogram */
  S_Spectr.pRfft    = &S_Rfft;
  S_Spectr.Type     = SPECTRUM_TYPE_POWER;
  S_Spectr.pWindow  = (float32_t *) hannWin_1024;
  S_Spectr.SampRate = TARGET_SAMPLE_RATE;
  S_Spectr.FrameLen = FRAME_LENGTH;
  S_Spectr.FFTLen   = FRAME_LENGTH;
  S_Spectr.pScratch = mel_spectrogram_column_buffer;

  /* Init Mel filter */
  S_MelFilter.pStartIndices = (uint32_t *) melFiltersStartIndices_1024_30;
  S_MelFilter.pStopIndices  = (uint32_t *) melFiltersStopIndices_1024_30;
  S_MelFilter.pCoefficients = (float32_t *) melFilterLut_1024_30;
  S_MelFilter.NumMels       = NUM_MEL_BANDS;

  /* Init MelSpectrogram */
  S_MelSpectr.SpectrogramConf = &S_Spectr;
  S_MelSpectr.MelFilter       = &S_MelFilter;
}

// frame = 1 spectrogram column = 1024 samples
void calculate_spectrogram_column(float* pFrame, int col_index) {
	// Create a Mel-scaled spectrogram column
	MelSpectrogramColumn(&S_MelSpectr, pFrame, mel_spectrogram_column_buffer);

	// Reshape and copy into output spectrogram
	for (uint32_t i = 0; i < SPECTROGRAM_ROWS; i++) {
		spectrogram[i * SPECTROGRAM_COLS + col_index] = mel_spectrogram_column_buffer[i];
	}
}

// converts the Mel spectrogram power to decibels (dB)
void spectrogram_power_to_db(float *pSpectrogram) {
    float max_mel_energy = FLT_MIN; // initial kleinster Wert
    float max_mel_log10;
    uint32_t rows = NUM_MEL_BANDS;
    uint32_t cols = SPECTROGRAM_COLS;
    uint32_t i, total_elements = rows * cols;

    // Find MelEnergy Scaling factor (highest value)
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

    // calculate log of reference once
    max_mel_log10 = 10.0f * log10f(max_mel_energy);

    // Scale Mel Energies and convert to dB
    for (i = 0; i < total_elements; i++) {
    	// https://librosa.org/doc/main/generated/librosa.power_to_db.html
    	pSpectrogram[i] = 10 * log10f(pSpectrogram[i]) - max_mel_log10;
    	//pSpectrogram[i] = 10.0f * log10f(pSpectrogram[i] / max_mel_energy);
        // Threshold to -80 dB and check for nan
        if (isnan(pSpectrogram[i]) || pSpectrogram[i] < -80.0f) {
        	pSpectrogram[i] = -80.0f;
        }
    }
}


void calculate_total_classification_result(float* total_file_classification_result, float classification_results_subsamples[][AI_NETWORK_1_OUT_1_SIZE], int number_subsamples_in_file) {
	for (int i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
		total_file_classification_result[i] = 0.0f;
	}

	// sum up result for each category
	for (int i = 0; i < number_subsamples_in_file; i++) {
		for (int j = 0; j < AI_NETWORK_1_OUT_1_SIZE; j++) {
			total_file_classification_result[j] += classification_results_subsamples[i][j];
		}
	}

	// calculate average result
	for (int i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
		total_file_classification_result[i] /= number_subsamples_in_file;
	}
}

void store_classification_result(float* result, char* file_name, FIL *fil) {
	// TODO
}



