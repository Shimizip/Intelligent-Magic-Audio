/**
  ******************************************************************************
  * @file           : interface.h
  * @brief          : Header file for interface functions and global definitions
  ******************************************************************************
  * @attention
  *
  * This file contains function prototypes and global definitions for managing
  * user inputs, file operations, and display updates. It includes:
  * - Function prototypes for handling ADC conversions, GPIO interrupts, and timer callbacks.
  * - Functions for initializing the display, updating the screen, sorting files,
  *   comparing ADC values, resetting displayed files, and managing file storage on SD.
  * - Global variables and external references related to ADC, I2C, timer, file management,
  *   and display operations.
  *
  * These functions and definitions are crucial for interfacing between the user inputs,
  * display rendering, and file management system, ensuring smooth operation of the application.
  *
  *******************************************************************************
  */
#include <stdlib.h>
#include <stdbool.h>
#include "ssd1306.h"
#include "fonts.h"
#include "display.h"
#include "filemanager.h"
#include "main.h"
#include "ff.h"
#include "util.h"

/**
 * @brief Definitions for thresholds, existence checks, comparison results, and smoothing height.
 *
 * These macros are used for setting thresholds, checking existence, comparing results, and defining
 * the height for smoothing calculations.
 */

#define THREASHOLD               0.1f ///< Threashold value for class comparison

#define EXISTS                  0 ///< Value indicating existence

#define NOT_EXISTS              -1 ///< Value indicating non-existence

#define SAME                    0 ///< Value indicating comparison result is the same

#define NOT_SAME                -1 ///< Value indicating comparison result is different

#define SMOOTHING_HEIGHT        30000 ///< Height value used for smoothing calculations


//extern parameters from main
//--------------------------------------------------------------------------

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;

extern FileManager fm;
extern FileManager fmCopy;

extern FATFS FatFs;
extern FILINFO fno;
extern DIR dir;
extern FRESULT fres;
extern FIL file;


extern uint32_t adcBuffer[NUM_CHANNELS];
extern char fileNamesSDCard[MAX_FILES][MAX_FILENAME_LENGTH];
extern int fileCount;

extern bool adcDmaFlag;
extern bool updateScreenFlag;
extern bool sortFilesFlag;

//---------------------------------------------------------------------------

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void screenInit(char (*fileNames)[MAX_FILENAME_LENGTH]);
void updateScreen(void);
void sortFiles(void);
void compareADCValues(void);
void resetShownFiles(void);
void writeFileManagerOnSD(void);
void readFileManagerFromSD(void);
void listFiles(const char *path);

