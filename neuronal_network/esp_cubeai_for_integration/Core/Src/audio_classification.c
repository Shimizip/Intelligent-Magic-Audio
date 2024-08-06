/**
 * @file audio_classification.c
 * @brief Audio classification implementation for embedded systems via stm32cube.ai (for integration into the system). Everything is untested!
 *
 * This file includes the complete integration of audio classification functions tailored for an embedded environment using STM32 hardware. It covers the initialization and de-initialization of neural network components, the generation and processing of spectrograms from audio data, classification of these spectrograms, and the handling of classification results. The code supports operations on audio data that involve feature scaling, neural network inference, and post-processing to interpret the network's output.
 *
 * Functions in this file demonstrate the full pipeline from reading raw audio data, converting it into Mel-spectrogram columns, classifying these spectrograms using a pre-trained neural network, and storing the results.
 *
 * @date June 20, 2024
 * @author Leon Braungardt
 */
#include "audio_classification.h"
#include <math.h>
#include "feature_extraction.h"
#include "network_1.h"
#include "network_1_data.h"
#include "network_1_config.h"
#include "network_1_data_params.h"

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
 * @var char aiOutDataLabels[AI_NETWORK_1_OUT_1_SIZE][10]
 * @brief Labels for the classification outputs of the neural network.
 *
 * This array stores string labels corresponding to the classification categories of the neural network outputs.
 */
char aiOutDataLabels[AI_NETWORK_1_OUT_1_SIZE][10] = {
		"bass",
        "pitched",
        "sustained",
        "rhythmic",
        "melodic"
};

/**
 * @var float32_t mel_spectrogram_column_buffer[SPECTROGRAM_ROWS]
 * @brief Buffer to store a single column of the Mel spectrogram during preprocessing.
 */
float32_t mel_spectrogram_column_buffer[SPECTROGRAM_ROWS];



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

	// enabling CRC clock for using AI libraries
	__HAL_RCC_CRC_CLK_ENABLE();

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


/**
 * @brief De-initializes the neural network and disables necessary hardware configurations.
 *
 * This function is responsible for safely shutting down the neural network instance by destroying it and deallocating any associated resources. Additionally, it disables the CRC (Cyclic Redundancy Check) clock used by the STM32Cube.AI library.
 *
 * @return int Returns 0 on successful de-initialization or -1 if an error occurs during the destruction of the neural network.
 *
 */
int de_init_nn() {
	// disable CRC clock
	__HAL_RCC_CRC_CLK_DISABLE();

	if (network != AI_HANDLE_NULL) {
		if (ai_network_1_destroy(network) != AI_HANDLE_NULL) {
			printf("error destroying nn");
			return -1;
	    }
		network = AI_HANDLE_NULL;
	}
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

    ai_input[0].data = AI_HANDLE_PTR(pSpectrogram);
    ai_output[0].data = AI_HANDLE_PTR(classification_result);

    if(network == AI_HANDLE_NULL)
    {
    	return -1;
    }

    // scale features with the same scaler as in the Jupyter Notebook
    // (for each spectrogram value: subtract mean value and divide by scaler)
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
 * @brief Initializes the mel-spectrogram calculation lib which is part of the audio preprocessing.
 *
 * This function sets up the components needed for generating Mel spectrograms from audio data.
 *
 * @note Derived from the FP-AI-SENSING feature pack: https://www.st.com/en/embedded-software/fp-ai-sensing1.html
 */
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

/**
 * @brief Classifies audio data from a single file using a neural network.
 *
 * This function processes an audio file and splits it into audiosubsamples. It handles reading data from the file, generating spectrogram columns, converting them to dB, running neural network classifications, and aggregating the results. The entire classification result for the file is stored afterwards.
 *
 * @param FIL *fil Pointer to the file on the SD card.
 * @param char* file_name Name of the file being classified, used for storing results.
 */
void classify_file(FIL *fil, char* file_name) {
	int16_t frame_buffer[FRAME_LENGTH];
	int16_t hop_buffer[HOP_LENGTH];
	int16_t resampled_chunk_data[FRAME_LENGTH];
	uint32_t number_subsamples_in_file;
	int err;

	float total_file_classification_result[AI_NETWORK_1_OUT_1_SIZE];

	number_subsamples_in_file = get_number_subsamples();
	float classification_results_subsamples[number_subsamples_in_file][AI_NETWORK_1_OUT_1_SIZE];

	// if number subsamples in file is zero => return empty classification result
	// TODO (but a bit complicated to implement) fill up missing samples with zero-padding
	if (number_subsamples_in_file == 0) {
		for (int i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
			total_file_classification_result[i] = 0.0;
		}
	}

	// loop over all subsamples of file
	for (int subsample_count = 0; i < number_subsamples_in_file; i++) {
		// generate spectrogram for subsample (sliding window; window size = frame size = 1024; hop size 512)
		for (int spectrogram_col_index = 0; i < SPECTROGRAM_COLS; i++) {
			if (spectrogram_col_index % 2 == 0) {
				// mod 2 == 0 => frame buffer is the read chunk
				// read chunk from SD card
				downsample_1024_samples(fil, resampled_chunk_data);
				// copy chunk to frame buffer
				for (int n = 0; n < FRAME_LENGTH; i++) {
					frame_buffer[n] = resampled_chunk_data[n];
				}
				// hop buffer is the second half of the read chunk
				for (int n = 0; n < HOP_LENGTH; i++) {
					hop_buffer[n] = resampled_chunk_data[n+512];
				}
			} else {
				// mod 2 == 1 => frame buffer is the hop buffer plus the first half of the chunk
				// first half of frame is the hop buffer
				for (int n = 0; n < HOP_LENGTH; i++) {
					frame_buffer[n] = hop_buffer[n];
				}
				// second half of frame is first half of newly read data
				for (int n = 0; n < HOP_LENGTH; i++) {
					frame_buffer[n+512] = resampled_chunk_data[n];
				}
			}
			calculate_spectrogram_column(frame_buffer);
		}
		// Convert power spectrogram to dB
		spectrogram_power_to_db(spectrogram);

		// Run the neural network classification
		if (run_nn_classification(spectrogram, aiOutData) == 0) {
			for (int i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
				classification_results_subsamples[subsample_count][i] = aiOutData[i];
			}
		} else {
			for (int i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
				classification_results_subsamples[subsample_count][i] = 0.0f;
			}
		}

	}

	calculate_total_classification_result(total_file_classification_result, classification_results_subsamples, number_subsamples_in_file);
	store_classification_result(fil, file_name, total_file_classification_result);
}

/**
 * @brief Generates a Mel-scaled spectrogram column from a frame (1024 samples) and inserts it into a spectrogram array.
 *
 * @param float* pFrame Pointer to the audio frame.
 * @param int col_index Index at which the spectrogram column will be inserted into the main spectrogram array.
 *
 */
void calculate_spectrogram_column(float* pFrame, int col_index) {
	// Create a Mel-scaled spectrogram column
	MelSpectrogramColumn(&S_MelSpectr, pFrame, mel_spectrogram_column_buffer);

	// Reshape and copy into output spectrogram
	for (uint32_t i = 0; i < SPECTROGRAM_ROWS; i++) {
		spectrogram[i * SPECTROGRAM_COLS + col_index] = mel_spectrogram_column_buffer[i];
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
void spectrogram_power_to_db(float *pSpectrogram) {
    float max_mel_energy = FLT_MIN; // Minimaler positiver Wert, um sicherzustellen, dass er überschrieben wird
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

/**
 * @brief Aggregates classification results from subsamples into a total result.
 *
 * This function calculates the total classification results by summing and then averaging the results of individual subsamples.
 *
 * @param float* total_file_classification_result Array to store the final averaged classification results.
 * @param float classification_results_subsamples[][AI_NETWORK_1_OUT_1_SIZE] Two-dimensional array containing classification results for each subsample.
 * @param int number_subsamples_in_file The total number of subsamples processed, used for averaging.
 *
 */
void calculate_total_classification_result(float* total_file_classification_result, float classification_results_subsamples[][AI_NETWORK_1_OUT_1_SIZE], int number_subsamples_in_file) {
	for (int i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
		total_file_classification_result[i] = 0.0f;
	}

	// sum up result for each category
	for (int i = 0; i < number_subsamples_in_file; i++) {
		for (int j = 0; j < AI_NETWORK_1_OUT_1_SIZE; j++) {
			// check for NaNs and ignore them (get valued as 0)
			if (!isnan(classification_results_subsamples[i][j])) {
			    total_file_classification_result[j] += classification_results_subsamples[i][j];
			}
		}
	}

	// calculate average result
	for (int i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
		total_file_classification_result[i] /= number_subsamples_in_file;
	}
}

/**
 * @brief Stores the classification results into struct saved to a file.
 *
 * This function is intended to write the classification results for an audiosample to the file containing the struct with all classification results as an object.
 *
 * @param FIL *fil Pointer to the file structure representing the file containing the struct (with all classification results) as an object.
 * @param char* file_name Name of the file where the classification results will be stored.
 * @param float* classification_result Array containing the neural network's classification results to be stored.
 *
 * @note Unfortunately, this function was not implemented due to the full system integration never happening because the project reached its time limit.
 */
void store_classification_result(FIL *fil, char* file_name, float* classification_result) {
	// TODO implementation
}



