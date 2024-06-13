/*
 * nn_helper_functions.h
 *
 *  Created on: May 16, 2024
 *      Author: STM32 Microelectronics
 */

#ifndef APP_NN_HELPER_FUNCTIONS_H_
#define APP_NN_HELPER_FUNCTIONS_H_

#include "ai_platform.h"
#include "ai_datatypes_defines.h"
#include "network_1.h"
#include "network_1_data.h"
#include "network_1_config.h"
#include "network_1_data_params.h"
#include "audio_classification.h"
#include <stdint.h>

#define AI_NETWORK_NUMBER 1

// Function prototypes

/**
 * @brief Logs the AI error.
 * @param err The AI error to log.
 * @param fct The function name where the error occurred.
 */
void aiLogErr(const ai_error err, const char *fct);

/**
 * @brief Calculates the size of the AI buffer.
 * @param buffer The AI buffer whose size is to be calculated.
 * @return The size of the buffer.
 */
ai_u32 aiBufferSize(const ai_buffer* buffer);

/**
 * @brief Retrieves the data weights address of the network.
 * @param ActivationSize The size of the activations.
 * @param ModelName Pointer to the model name.
 * @return The address of the data weights.
 */
ai_u8* aiNetworkRetrieveDataWeightsAddress(uint32_t ActivationSize, uint8_t **ModelName);

/**
 * @brief Prints the layout of the AI buffer.
 * @param msg The message to print before the buffer information.
 * @param idx The index of the buffer.
 * @param buffer The AI buffer to print.
 */
void aiPrintLayoutBuffer(const char *msg, int idx, const ai_buffer* buffer);

/**
 * @brief Prints the AI network report.
 * @param report The AI network report to print.
 */
void aiPrintNetworkInfo(const ai_network_report* report);

/**
 * @brief Converts input data from float to int8.
 * @param nn_name The name of the neural network.
 * @param In_f32 The input data in float format.
 * @param Out_int8 The output data in int8 format.
 * @return 0 on success, -1 on failure.
 */
int aiConvertInputFloat_2_Int8(ai_float *In_f32, ai_i8 *Out_int8);

/**
 * @brief Converts output data from int8 to float.
 * @param nn_name The name of the neural network.
 * @param In_int8 The input data in int8 format.
 * @param Out_f32 The output data in float format.
 * @return 0 on success, -1 on failure.
 */
int aiConvertOutputInt8_2_Float(ai_i8 *In_int8, ai_float *Out_f32);

#endif // NN_HELPER_FUNCTIONS_H
