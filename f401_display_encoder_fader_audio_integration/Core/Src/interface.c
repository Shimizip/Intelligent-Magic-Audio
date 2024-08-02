#include "interface.h"

uint32_t rotary_enc_count = 0;

bool switch_push_button = false;
bool debounce = false;
bool init = false;
bool sortFilesFlag = false;

uint16_t cnt;
uint32_t potentiometer_value = 0 ;
uint32_t smoothValue[NUM_CHANNELS] = {0};
uint8_t pastClassPercentADC[NUM_CHANNELS] = {0};
uint8_t currentClassPercentADC[NUM_CHANNELS] = {0};
uint16_t endValues[NUM_CHANNELS] = {0};
char faderProzent[2][50];


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

    // werte Encoder aus Rotor DC Motor aus
    if (GPIO_Pin == enc_a_clk_in1_Pin && !debounce) {
          __HAL_TIM_CLEAR_FLAG(&htim5, TIM_FLAG_UPDATE);
          HAL_TIM_Base_Start_IT(&htim5);
          debounce = true;
       //wenn A High und B High, increment Encoder Count
        if (HAL_GPIO_ReadPin(enc_b_dt_in2_GPIO_Port, enc_b_dt_in2_Pin)) {
          rotary_enc_count--;
          cursorDown(&fm);
        } else {
          // wenn A High und B Low, decrement Encoder Count
            rotary_enc_count++;
            cursorUp(&fm);
        }
    }

    if(GPIO_Pin == enc_switch_in3_Pin && !debounce){
      __HAL_TIM_CLEAR_FLAG(&htim5, TIM_FLAG_UPDATE);
      HAL_TIM_Base_Start_IT(&htim5);
      debounce = true;
      switch_push_button =true;
      selectFile(&fm);
      init = false;
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // reset debounce flag nach Timer5 elapsed
    if (htim == &htim5) {
        HAL_TIM_Base_Stop_IT(&htim5);
        debounce = false;
    }
    if (htim == &htim3){
        HAL_TIM_Base_Stop_IT(&htim3);
    	updateScreenFlag = true;
    	adcDmaFlag = true;
        __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
        HAL_TIM_Base_Start_IT(&htim3);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == ADC1){
		cnt++;
		for(int i = 0; i < NUM_CHANNELS; i++){
			smoothValue[i]+=adcBuffer[i];
		}
		if(cnt >= SMOOTHING_HEIGHT){
			HAL_ADC_Stop_DMA(&hadc1);
			for(int j = 0; j < NUM_CHANNELS; j++){
				currentClassPercentADC[j]=(smoothValue[j]/SMOOTHING_HEIGHT)/41;
				fm.fader_Class[j] = ((smoothValue[j]/SMOOTHING_HEIGHT)/4096.0f);
				smoothValue[j]=0;
			}
			cnt=0;

			sprintf(faderProzent[0], "F1: %u F2: %u",currentClassPercentADC[0], currentClassPercentADC[1]);
			sprintf(faderProzent[1], "F3: %u F4: %u",currentClassPercentADC[2], currentClassPercentADC[3]);
		}
	}
}
void screenInit(char (*fileNames)[MAX_FILENAME_LENGTH]){
  if (ssd1306_Init(&hi2c1) != 0) {
  	Error_Handler();
  }
  HAL_Delay(1000);

  ssd1306_Fill(Black);
  ssd1306_UpdateScreen(&hi2c1);

  HAL_Delay(1000);

  init = true;

  float fileClasses[MAX_FILES][MAX_CLASSES] = {
      {0.100, 0.550}, {0.100, 0.500}, {0.150, 0.450}, {0.180, 0.420}, {0.900, 0.700},
      {0.350, 0.250}, {0.450, 0.650}, {0.550, 0.750}, {0.650, 0.850}, {0.750, 0.950},
      {0.850, 0.050}, {0.950, 0.150}, {0.050, 0.250}, {0.150, 0.350}, {0.250, 0.450},
      {0.350, 0.550}, {0.450, 0.650}, {0.550, 0.750}, {0.650, 0.850}, {0.750, 0.950},
      {0.850, 0.050}, {0.950, 0.150}, {0.050, 0.250}, {0.150, 0.350}, {0.250, 0.450},
      {0.350, 0.550}, {0.450, 0.650}, {0.550, 0.750}, {0.650, 0.850}, {0.750, 0.950},
      {0.850, 0.050}, {0.950, 0.150}, {0.050, 0.250}, {0.150, 0.350}, {0.250, 0.450},
      {0.350, 0.550}, {0.450, 0.650}, {0.550, 0.750}, {0.650, 0.850}, {0.750, 0.950},
      {0.850, 0.050}, {0.950, 0.150}, {0.050, 0.250}, {0.150, 0.350}, {0.250, 0.450},
      {0.350, 0.550}, {0.450, 0.650}, {0.550, 0.750}, {0.650, 0.850}, {0.750, 0.950}
  };

  // Liste von Dateinamen ohne Duplikate
  /*const char *filenames[MAX_FILES] = {
      "bird.wav", "child.wav", "waterfall.wav", "samba.wav", "mixer.wav",
      "disco.wav", "leon.wav", "jonas.wav", "shimi.wav", "bass.wav",
      "smooth.wav", "car.wav", "ocean.wav", "forest.wav", "moon.wav",
      "planet.wav", "star.wav", "cloud.wav", "rain.wav", "storm.wav",
      "thunder.wav", "lightning.wav", "sun.wav", "moonlight.wav", "fire.wav",
      "flame.wav", "heat.wav", "freeze.wav", "ice.wav", "snow.wav",
      "blizzard.wav", "mountain.wav", "valley.wav", "river.wav", "lake.wav",
      "oasis.wav", "desert.wav", "jungle.wav", "savanna.wav", "grassland.wav",
      "field.wav", "meadow.wav", "flower.wav", "tree.wav", "bush.wav",
      "rock.wav", "cave.wav", "volcano.wav", "earthquake.wav", "tsunami.wav"
  };*/
  for (int i = 1; i < 11; i++) {
      addFile(&fm, fileNames[i], fileClasses[i]);
  }
  sortFiles();
  updateScreen();
}

void sortFiles(void){
	bool classFulfilled;
	bool fileExists;
	float classForWav, classFromFader;
	char* compareFile;
	char* shownFile;
	int res;

	for(int i = 0; i < MAX_FILES; i++){
		classFulfilled = true;
		fileExists = true;
		for(int j = 0; j < MAX_CLASSES; j++){
			classForWav = fm.files[i].classes[j];
			classFromFader = fm.fader_Class[j];
			if(fabsf(classForWav-classFromFader) > THREASHOLD){
				classFulfilled = false;
				break;
			}
		}
		if(classFulfilled){
			fileExists = false;
			compareFile = fm.files[i].filename;
			for(int k = 0; k<fm.num_matched_files; k++){
				shownFile = fm.shownFiles[k].filename;
				res = strcmp(compareFile, shownFile);
				if(res == EXISTS){
					fileExists = true;
					res = -1;
					break;
				}
			}
		}
		if(!fileExists){
			fm.shownFiles[fm.num_matched_files] = fm.files[i];
			fm.num_matched_files++;
		}
	}
	sortFilesFlag = false;
}

void updateScreen(void){
	char *filenames[MAX_FILES];
	char *currentFileName;
	for (int i = 0; i < fm.num_files; i++) {
		filenames[i] = fm.shownFiles[i].filename; // Extracting filenames from File structures
	}
	if(init){
		currentFileName = "";
	}
	else{
	currentFileName = fm.shownFiles[fm.current_file_index].filename;
	}
	displayStrings(&hi2c1, filenames, fm.num_matched_files, fm.cursor_index);
	renderSelectedFile(&hi2c1, currentFileName);
	drawFaderProzent(&hi2c1, faderProzent[0], 1);
	drawFaderProzent(&hi2c1, faderProzent[1], 2);


	// Update display
	ssd1306_UpdateScreen(&hi2c1);
}

void compareADCValues(void){
	bool faderSettingsChanged= false;

	for(int i = 0; i < NUM_CHANNELS; i++){
		if(pastClassPercentADC[i] != currentClassPercentADC[i]){
			pastClassPercentADC[i] = currentClassPercentADC[i];
			faderSettingsChanged = true;
			sortFilesFlag = true;
		}
	}
	if(faderSettingsChanged){
		safeCurrentFileName(&fm);
		resetShownFiles();
		faderSettingsChanged = false;
	}
}
void resetShownFiles(void) {
    memset(fm.shownFiles, 0, sizeof(fm.shownFiles)); // Setzt den gesamten Speicherbereich auf 0
    fm.num_matched_files = 0;
}

void listFiles(const char *path)
{
    // Öffnen des Verzeichnisses
    fres = f_opendir(&dir, "/SamplePool");
    if (fres == FR_OK)
    {
        while (fileCount < MAX_FILES)
        {
            // Lesen eines Eintrags
            fres = f_readdir(&dir, &fno);
            if (fres != FR_OK || fno.fname[0] == 0) break;

            // Überprüfen, ob es sich um eine Datei handelt
            if (!(fno.fattrib & AM_DIR))
            {
                // Es ist eine Datei
                char *fileName = fno.fname;

                // Überprüfen, ob die Datei die Erweiterung .wav hat
                if (strstr(fileName, ".wav") != NULL)
                {
                    // Datei mit .wav Erweiterung, speichern im Array
                    strncpy(fileNamesSDCard[fileCount + 1], fileName, MAX_FILENAME_LENGTH - 1);
                    fileNamesSDCard[fileCount][MAX_FILENAME_LENGTH - 1] = '\0'; // Null-terminieren
                    fileCount++;
                }
            }
        }
        f_closedir(&dir);
    }
}


void writeFileManagerOnSD(void)
{
    FIL file;
    FRESULT fres;
    UINT bytesWritten;

    // Öffnen der Datei im Schreibmodus, nur erstellen, wenn sie nicht existiert
    fres = f_open(&file, "/filemanager_data.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (fres != FR_OK)
    {
        // Fehler beim Öffnen/Erstellen der Datei
        printf("Fehler beim Öffnen/Erstellen der Datei: %d\n", fres);
        return;
    }

    // Schreiben der Daten in die Datei
    fres = f_write(&file, &fm, sizeof(FileManager), &bytesWritten);
    if (fres != FR_OK || bytesWritten != sizeof(FileManager))
    {
        // Fehler beim Schreiben der Daten
        printf("Fehler beim Schreiben der Daten\n");
        f_close(&file);
        return;
    }

    // Datei schließen
    f_close(&file);
}

void readFileManagerFromSD(void)
{
    FIL file;
    FRESULT fres;
    UINT bytesRead;

    // Öffnen der Datei im Lese-Modus
    fres = f_open(&file, "/filemanager_data.bin", FA_READ);
    if (fres != FR_OK)
    {
        // Fehler beim Öffnen der Datei
        printf("Fehler beim Öffnen der Datei: %d\n", fres);
        return;
    }

    // Lesen der Daten aus der Datei
    fres = f_read(&file, &fmCopy, sizeof(FileManager), &bytesRead);
    if (fres != FR_OK || bytesRead != sizeof(FileManager))
    {
        // Fehler beim Lesen der Daten
        printf("Fehler beim Lesen der Daten\n");
        f_close(&file);
        return;
    }

    // Datei schließen
    f_close(&file);
}






