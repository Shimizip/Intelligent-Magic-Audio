/******************************************************************************
  * @file    audio_classification.c
  * @author  Leon Braungardt
  * @version V0.0.1
  * @date    16.05.2024
  * @brief   Audio Sample Classification algorithm
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <math.h>
#include "audio_classification.h"
#include "nn_helper_functions.h"
#include "network_1.h"
#include "featurescaler.h"
#include "feature_extraction.h"

/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// input and output buffers for NN classification
//ai_i8 in_data[AI_NETWORK_1_IN_1_SIZE_BYTES];
//ai_i8 out_data[AI_NETWORK_1_OUT_1_SIZE_BYTES];

// context for the audio neural network
//network_context_t audio_net_ctx;
#include <stdio.h>
#include <stdlib.h>


ai_handle network;
ai_u8 activations[AI_NETWORK_1_DATA_ACTIVATIONS_SIZE];
ai_buffer * ai_input;
ai_buffer * ai_output;

ai_float spectrogram[AI_NETWORK_1_IN_1_SIZE];
ai_float aiOutData[AI_NETWORK_1_OUT_1_SIZE] = {0.0, 0.0, 0.0, 0.0, 0.0};
uint32_t spectrogram_col_index = 0;


//// Statische Arrays für Eingabe- und Ausgabedaten
//static float32_t input_data_float[MAX_INPUT_LENGTH];
//static float32_t resampled_data[MAX_OUTPUT_LENGTH];

/**
 * @brief Resamples audio data in memory to 16 kHz using linear interpolation.
 * @param input_data Pointer to the input audio data buffer (16-bit PCM).
 * @param input_length Length of the input data buffer.
 * @param input_sample_rate Sample rate of the input audio data.
 * @param output_data Pointer to the pointer of the output audio data buffer (float).
 * @param output_length Pointer to the length of the output data buffer.
 * @return 0 on success, -1 on failure.
 */
//int resample_audio(const short* input_data, int input_length, int input_sample_rate, float32_t* output_data, int* output_length) {
//
//
//    *output_length = input_length;
//
////    // Konvertiere Eingabedaten von short zu float32_t
//    for (int i = 0; i < input_length; i++) {
//    	output_data[i] = audio_data[i] / 32768.0f;
//    }
//
//    return 0; // Erfolg
//}

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
	float32_t frames[N_FRAMES_PER_SUBSAMPLE][FRAME_LENGTH];

    for (int i = 0; i < N_FRAMES_PER_SUBSAMPLE; i++) {
        int start_index = i * HOP_LENGTH;
        for (int j = 0; j < FRAME_LENGTH; j++) {
            if (start_index + j < subsample_length) {
                frames[i][j] = subsample[start_index + j];
            } else {
                frames[i][j] = 0.0f; // pad with zeros
            }
        }
        preprocess_frame(frames[i]);
    }
}

// frame = 1 spectrogram column = 1024 samples
void preprocess_frame(float32_t* frame) {


	// Create a Mel-scaled spectrogram column
	MelSpectrogramColumn(&S_MelSpectr, frame, mel_spectrogram_column_buffer);

	// Reshape and copy into output spectrogram column
	for (uint32_t i = 0; i < SPECTROGRAM_ROWS; i++) {
		spectrogram[i * SPECTROGRAM_COLS + spectrogram_col_index] = mel_spectrogram_column_buffer[i];
	}

    spectrogram_col_index++;

    // If we have a full spectrogram (32 columns), process it
    if (spectrogram_col_index == SPECTROGRAM_COLS) {
        // Convert power spectrogram to dB
        PowerToDb(spectrogram);

        // Run the neural network classification

        if (run_nn_classification(spectrogram, aiOutData) == 0) {
        	/* Output results */
			for (uint32_t i = 0; i < AI_NETWORK_1_OUT_1_SIZE; i++) {
			  printf("%8.6f ", aiOutData[i]);
			}
        }

        // Reset the column index for the next spectrogram
        spectrogram_col_index = 0;
    }
}

void PowerToDb(float32_t *spectrogram)
{
  float32_t max_mel_energy = FLT_MIN; // Minimaler positiver Wert, um sicherzustellen, dass er überschrieben wird
  uint32_t rows = 30;
  uint32_t cols = 32;
  uint32_t i, total_elements = rows * cols;


  // Find MelEnergy Scaling factor
  for (i = 0; i < total_elements; i++) {
    if (spectrogram[i] > max_mel_energy) {
      max_mel_energy = spectrogram[i];
    }
  }

  // Check if max_mel_energy is zero to avoid division by zero
  if (max_mel_energy == 0.0f) {
    // If max_mel_energy is zero, the spectrogram is all zeros, and we can directly set the output to -80 dB
    for (i = 0; i < total_elements; i++) {
      spectrogram[i] = -80.0f;
    }
    return;
  }

  // Scale Mel Energies and convert to dB
  for (i = 0; i < total_elements; i++) {
    spectrogram[i] = 10.0f * log10f(spectrogram[i] / max_mel_energy);
    // Threshold to -80 dB
    if (spectrogram[i] < -80.0f) {
      spectrogram[i] = -80.0f;
    }
  }
}


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
//	ai_error err;
//
//	err = ai_network_1_create(&audio_net_ctx.handle, NULL);
//	if (err.type) {
//		aiLogErr(err, "ai_network_1_create");
//	    return -1;
//	}
//
//	// Print network report
//	if (ai_network_1_get_info(audio_net_ctx.handle, &audio_net_ctx.report)) {
//		aiPrintNetworkInfo(&audio_net_ctx.report);
//	} else {
//		err = ai_network_1_get_error(audio_net_ctx.handle);
//		aiLogErr(err, "ai_network_1_get_info");
//		ai_network_1_destroy(audio_net_ctx.handle);
//		audio_net_ctx.handle = AI_HANDLE_NULL;
//		return -1;
//	}
//
//	return 0; // success
	return 0;
}

// Function to run the neural network and classify input data
int run_nn_classification(float* spectrogram, float* classification_result) {
//    ai_i8 nn_input[AI_NETWORK_1_IN_1_SIZE];
//    ai_i8 nn_output[AI_NETWORK_1_OUT_1_SIZE];

    ai_i32 batch;
    ai_error err;

    // Z-Score Scaling on input feature
//    for (uint32_t i = 0; i < SPECTROGRAM_ROWS * SPECTROGRAM_COLS; i++) {
//        spectrogram[i] = (spectrogram[i] - featureScalerMean[i % SPECTROGRAM_ROWS]) / featureScalerStd[i % SPECTROGRAM_ROWS];
//    }

    // Convert input data from float to int8
    //aiConvertInputFloat_2_Int8(spectrogram, nn_input);

    // Define input and output buffers
//    ai_buffer nn_input_buffer[AI_NETWORK_1_IN_NUM] = AI_NETWORK_1_IN;
//    ai_buffer nn_output_buffer[AI_NETWORK_1_OUT_NUM] = AI_NETWORK_1_OUT;

    /* Update IO handlers with the data payload */
    ai_input[0].data = AI_HANDLE_PTR(spectrogram);
    ai_output[0].data = AI_HANDLE_PTR(classification_result);

//    // Set input data pointer
//    nn_input_buffer[0].data = AI_HANDLE_PTR(nn_input);
//
//    // Set output data pointer
//    nn_output_buffer[0].data = AI_HANDLE_PTR(nn_output);
    if(network == AI_HANDLE_NULL)
      {
          printf("E: network handle is NULL\r\n");
          return -1;
      }
    // Run the network
    batch = ai_network_1_run(network, ai_input, ai_output);
    if (batch != 1) {
        err = ai_network_1_get_error(network);
        printf("AI ai_network_run error - type=%d code=%d\r\n", err.type, err.code);
        return -1;
    }

//    if (ai_network_1_run(audio_net_ctx.handle, nn_input_buffer, nn_output_buffer) != 1) {
//        aiLogErr(ai_network_1_get_error(audio_net_ctx.handle), "ai_network_1_run");
//        return -1;
//    }
//
//    // Convert output data from int8 to float
//    aiConvertOutputInt8_2_Float(nn_output, classification_result);

    return 0; // Success
}
