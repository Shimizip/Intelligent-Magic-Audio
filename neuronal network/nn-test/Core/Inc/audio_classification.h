#ifndef AUDIO_CLASSIFICATION_H
#define AUDIO_CLASSIFICATION_H

#include "network_1.h"
#include "network_1_data.h"
#include "network_1_config.h"
#include "network_1_data_params.h"
#include "feature_extraction.h"



#define NUM_MEL_BANDS 30
#define SPECTROGRAM_ROWS NUM_MEL_BANDS
#define SPECTROGRAM_COLS 32

#define TARGET_SAMPLE_RATE 16000
#define FRAME_LENGTH 1024
#define HOP_LENGTH 512
#define INPUT_BUFFER_SIZE FRAME_LENGTH

#define N_SAMPLES_PER_SUBSAMPLE 16384 // TARGET_SAMPLE_RATE * (FRAME_LENGTH * 0.001)
#define N_FRAMES_PER_SUBSAMPLE SPECTROGRAM_COLS // ((n_samples_per_audiosubsample - FRAME_LENGTH + HOP_LENGTH) / HOP_LENGTH) + 1;

typedef enum
{
  RYTHMIC       = 0x00,
  VOCAL		    = 0x01,
  HARMONIC      = 0x02,
  SUSTAINED     = 0x03,
  PITCH 		= 0x04,
  UNDEFINED     = 0xFF
} NN_OutputTypeDef;

typedef enum
{
  NN_OK      = 0x00,
  NN_ERROR   = 0x01,
} NN_StatusTypeDef;

// definition of context structure of the NN
//typedef struct {
//    ai_handle handle;           // handle of neural network
//    ai_network_report report;   // report of NN (information and status)
//} network_context_t;

// spectrogram stuff
static arm_rfft_fast_instance_f32 S_Rfft;
static SpectrogramTypeDef         S_Spectr;
static MelSpectrogramTypeDef      S_MelSpectr;
static MelFilterTypeDef           S_MelFilter;

// Declare input and output buffers
//extern ai_i8 in_data[AI_NETWORK_1_IN_1_SIZE_BYTES];
//extern ai_i8 out_data[AI_NETWORK_1_OUT_1_SIZE_BYTES];
// Externe Deklarationen der Variablen
extern ai_handle network;
extern ai_buffer* ai_input;
extern ai_buffer* ai_output;
extern ai_u8 activations[];
extern float aiOutData[];
extern float spectrogram[];
extern uint32_t spectrogram_col_index;

static float32_t mel_spectrogram_column_buffer[SPECTROGRAM_ROWS];

// Declare initialization and classification functions
void Preprocessing_Init(void);
int resample_audio(const short* input_data, int input_length, int input_sample_rate, float32_t* output_data, int* output_length);
void frame_subsamples(float32_t* subsample, int subsample_length);
void preprocess_frame(float* frame);
void PowerToDb(float32_t *spectrogram);
int init_nn(void);
int run_nn_classification(ai_float* spectrogram, ai_float* classification_result);


#endif // AUDIO_CLASSIFICATION_H
