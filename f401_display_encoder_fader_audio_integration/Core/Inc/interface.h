#include <stdlib.h>
#include <stdbool.h>
#include "ssd1306.h"
#include "fonts.h"
#include "display.h"
#include "filemanager.h"
#include "main.h"
#include "ff.h"
#include "util.h"

#define THREASHOLD 0.1f
#define EXISTS 0
#define NOT_EXISTS -1
#define SAME 0
#define NOT_SAME -1
#define SMOOTHING_HEIGHT 30000


extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;

extern FileManager fm;
extern FileManager fmCopy;

extern FATFS FatFs;
extern FILINFO fno;
extern DIR dir; //Directory
extern FRESULT fres;
extern FIL file;        // File-Objekt für FatFs


//extern Filemanager fm;
extern uint32_t adcBuffer[NUM_CHANNELS];
extern char fileNamesSDCard[MAX_FILES][MAX_FILENAME_LENGTH];
extern int fileCount;

extern bool adcDmaFlag;
extern bool updateScreenFlag;
extern bool sortFilesFlag;


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

