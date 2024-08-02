#include "interface.h"

/**
 * @brief Global variables for debugging, debouncing, initialization, and managing fader settings.
 *
 * These variables are used for various purposes throughout the program, including debugging, debouncing encoder inputs,
 * initializing states, sorting file names, smoothing ADC values, and displaying fader settings on the LCD.
 */

uint32_t rotary_enc_count = 0; ///< Count variable for debugging purposes.

bool switch_push_button = false; ///< Boolean to verify if the encoder switch is pushed.

bool debounce = false; ///< Flag to debounce the encoder.

bool init = false; ///< Flag to check if initialization is done.

bool sortFilesFlag = false; ///< Flag to regroup the shown file names.

uint16_t cnt; ///< Counter to smooth the fader values.

uint32_t smoothValue[NUM_CHANNELS] = {0}; ///< Array to store smoothed values of the ADC buffer outputs.

uint8_t pastClassPercentADC[NUM_CHANNELS] = {0}; ///< Array to cache the previous fader settings.

uint8_t currentClassPercentADC[NUM_CHANNELS] = {0}; ///< Array to store the current fader settings in percentage.

char faderProzent[2][50]; ///< Array to hold fader percentage strings for display on the LCD.


/**
 * @brief Callback function that responds to encoder signals.
 *
 * This function handles encoder signals by adjusting the encoder count and cursor position on the display.
 * It also debounces the encoder switch using Timer5 and a debounce flag.
 * 
 * - If A is High and B is High:
 *   - Decrement the encoder count and increment the cursor position on the display.
 * - If A is High and B is Low:
 *   - Increment the encoder count and decrement the cursor position on the display.
 * - If the switch is pushed:
 *   - Select the file and debounce the encoder switch using Timer5 and the debounce flag.
 *
 * @param[in] GPIO_pin The GPIO pin signals from the MCU related to the encoder and switch.
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

    // Change the cnt the Position of Cursor and debounce the encoder with &tim5
    if (GPIO_Pin == enc_a_clk_in1_Pin && !debounce) {
          __HAL_TIM_CLEAR_FLAG(&htim5, TIM_FLAG_UPDATE);
          HAL_TIM_Base_Start_IT(&htim5);
          debounce = true;
       //wenn A High und B High, decrement Encoder Count and increment the cursor position of the display
        if (HAL_GPIO_ReadPin(enc_b_dt_in2_GPIO_Port, enc_b_dt_in2_Pin)) {
          rotary_enc_count--;
          cursorDown(&fm);
        } else {
          // wenn A High und B Low, increment Encoder Count and decrement the cursor position of the display
            rotary_enc_count++;
            cursorUp(&fm);
        }
    }
    // If the switch is pushed selectFile and debounce the encoder with &tim5
    if(GPIO_Pin == enc_switch_in3_Pin && !debounce){
      __HAL_TIM_CLEAR_FLAG(&htim5, TIM_FLAG_UPDATE);
      HAL_TIM_Base_Start_IT(&htim5);
      debounce = true;
      switch_push_button =true;
      selectFile(&fm);
      init = false;
    }
}

/**
 * @brief Callback function triggered when a timer expires.
 *
 * This function handles timer expirations by performing the following actions:
 * 
 * For Timer5 (tim5):
 * - Resets the debounce flag after Timer5 has elapsed.
 * - Stops Timer5.
 * 
 * For Timer3 (tim3):
 * - Sets flags to indicate DMA completion and screen update.
 * - Sets a new timer for the next refresh interval.
 *
 * @param[in] htim Pointer to the TIM_HandleTypeDef structure for the current timer.
 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // reset debounce flag nach Timer5 elapsed
    if (htim == &htim5) {
        HAL_TIM_Base_Stop_IT(&htim5);
        debounce = false;
    }
    // stop timer set flags for dma and screen update and set a new timer for new refresh interval
    if (htim == &htim3){
        HAL_TIM_Base_Stop_IT(&htim3);
    	updateScreenFlag = true;
    	adcDmaFlag = true;
        __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
        HAL_TIM_Base_Start_IT(&htim3);
    }
}

/**
 * @brief Callback function for the ADC conversion.
 *
 * This function is called when the ADC conversion is complete. It performs the following actions:
 * 
 * - Calculates the smooth values for all channels of the ADC.
 * - Computes the average values for display and comparison operations.
 *   - These average values are used for displaying on the screen and for the sorting algorithm's comparison operations.
 * - Initializes the character array that will be shown on the display.
 *
 * @param[in] hadc Pointer to the ADC handle structure.
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{	// Calculation of the smooth values for alle channels od the ADC
	if (hadc->Instance == ADC1){
		cnt++;
		for(int i = 0; i < NUM_CHANNELS; i++){
			smoothValue[i]+=adcBuffer[i];
		}
		// Calculation of the average values for the diplay and the compare operations
		if(cnt >= SMOOTHING_HEIGHT){
			HAL_ADC_Stop_DMA(&hadc1);
			for(int j = 0; j < NUM_CHANNELS; j++){
				//Shown values
				currentClassPercentADC[j]=(smoothValue[j]/SMOOTHING_HEIGHT)/41;
				//Compare values for the Sortalgorythm
				fm.fader_Class[j] = ((smoothValue[j]/SMOOTHING_HEIGHT)/4096.0f);
				smoothValue[j]=0;
			}
			cnt=0;
			//Initialisation of the char array that are shown on Display
			sprintf(faderProzent[0], "F1: %u F2: %u",currentClassPercentADC[0], currentClassPercentADC[1]);
			sprintf(faderProzent[1], "F3: %u F4: %u",currentClassPercentADC[2], currentClassPercentADC[3]);
		}
	}
}

//InIt Funktion for the Display

/**
 * @brief Initializes the screen and sets up the file manager with random class percentages.
 *
 * This function initializes the LCD-Display, fills it with black, and updates the screen. It also
 * initializes the file manager with file names and assigns random class percentages for system tests.
 * 
 * @param[in] fileNames Array of file names to be added to the file manager.
 */
void screenInit(char (*fileNames)[MAX_FILENAME_LENGTH]){
  if (ssd1306_Init(&hi2c1) != 0) {
  	Error_Handler();
  }
  HAL_Delay(1000);

  ssd1306_Fill(Black);
  ssd1306_UpdateScreen(&hi2c1);

  HAL_Delay(1000);

  init = true;
  //Initialisation of the Class Percentage for System Tests with random Values between 0.0-1.0
  float fileClasses[MAX_FILES][MAX_CLASSES];

  for (int i = 0; i < MAX_FILES; ++i) {
      for (int j = 0; j < MAX_CLASSES; ++j) {
          // Generiere eine Zufallszahl zwischen 0 und 1000 (für zwei Nachkommastellen)
          int random_value = rand() % 1001;
          float random_float = (float)random_value / 1000.0f; // Skaliere auf den Bereich 0.0 - 1.0

          // Setze den Wert im Array
          fileClasses[i][j] = random_float;
      }
  }
  // Add Files to the Filemanger
  for (int i = 1; i < MAX_FILES; i++) {
      addFile(&fm, fileNames[i], fileClasses[i]);
  }
  sortFiles();
  updateScreen();
}

/**
 * @brief Sorts filenames for display based on fader settings.
 *
 * This function sorts filenames by comparing file classes with the fader settings. It performs the following actions:
 * 
 * - Compares file classes against the fader settings with a threshold of 0.1.
 * - Compares the newly added filenames with the existing ones.
 * - Adds files from the existing file list to the `shownFiles` in the file manager.
 *
 * @param[in] void
 */
void sortFiles(void){
	bool classFulfilled;
	bool fileExists;
	float classForWav, classFromFader;
	char* compareFile;
	char* shownFile;
	int res;
	//Comparsion of the fileclasses and the fader settings
	//whether the class is fullfilled with a threashold of 0.1
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
		//Comparsion of the allready added filenames und the new one
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
		//Add of the file from all existing files to the filemanegers shownFiles
		if(!fileExists){
			fm.shownFiles[fm.num_matched_files] = fm.files[i];
			fm.num_matched_files++;
		}
	}
	sortFilesFlag = false;
}

/**
 * @brief Updates the display with filenames, the currently selected file, and fader settings.
 *
 * This function updates the display by performing the following tasks:
 * 
 * - Extracts filenames from the file manager's `shownFiles` array.
 * - Sets the current filename to be displayed based on the initialization state.
 * - Displays all filenames in the `shownFiles` array.
 * - Display the currently selected filename.
 * - Shows the fader settings on the display.
 *
 * @note The function relies on global variables and external functions to interact with the display.
 */
void updateScreen(void){
	char *filenames[MAX_FILES];
	char *currentFileName;
	for (int i = 0; i < fm.num_files; i++) {
		filenames[i] = fm.shownFiles[i].filename; // Extracting filenames from File structures
	}
	//Case if the init is not fullfiled
	if(init){
		currentFileName = "";
	}
	else{
	//To display the chosen filename
	currentFileName = fm.shownFiles[fm.current_file_index].filename;
	}
	//To show all filenames in the showFiles Array
	displayStrings(&hi2c1, filenames, fm.num_matched_files, fm.cursor_index);
	//To display the chosen filename
	renderSelectedFile(&hi2c1, currentFileName);
	//Show the Fader Settings on Display
	drawFaderProzent(&hi2c1, faderProzent[0], 1);
	drawFaderProzent(&hi2c1, faderProzent[1], 2);


	// Update display
	ssd1306_UpdateScreen(&hi2c1);
}

/**
 * @brief Checks if fader settings have changed and updates accordingly.
 *
 * This function compares the current fader settings with previous values to determine if any changes have occurred. If changes are detected, it performs the following actions:
 * 
 * - Compares the current and past fader settings.
 * - Updates the past fader settings with the current values if a change is detected.
 * - Sets a flag to indicate that the file list needs to be sorted.
 * - Saves the current filename and clears the `shownFiles` array if a change is detected.
 *
 * @note This function uses global variables for fader settings and file management.
 */
void compareADCValues(void){
	bool faderSettingsChanged= false;
    //Compare the current und the past fader settings
	for(int i = 0; i < NUM_CHANNELS; i++){
		if(pastClassPercentADC[i] != currentClassPercentADC[i]){
			pastClassPercentADC[i] = currentClassPercentADC[i];
			faderSettingsChanged = true;
			sortFilesFlag = true;
		}
	}
	//The current filename get safed and the shownFiles array get cleared
	if(faderSettingsChanged){
		safeCurrentFileName(&fm);
		resetShownFiles();
		faderSettingsChanged = false;
	}
}
/**
 * @brief Resets the `shownFiles` array and updates the number of matched files.
 *
 * This function sets all entries in the `shownFiles` array to zero and resets the count of matched files.
 * 
 * - Sets the memory area of `fm.shownFiles` to zero using `memset`.
 * - Resets the `num_matched_files` field to 0.
 *
 * @note This function is used to clear the list of files currently displayed on the screen.
 */
void resetShownFiles(void) {
    memset(fm.shownFiles, 0, sizeof(fm.shownFiles)); // Setzt den gesamten Speicherbereich auf 0
    fm.num_matched_files = 0;
}

/**
 * @brief Lists all filenames with a .wav extension from the specified directory on the SD card.
 *
 * This function opens the directory specified by the `path` parameter, reads through its contents, and stores filenames with a `.wav` extension in the `fileNamesSDCard` array. It performs the following actions:
 * 
 * - Opens the directory at the specified path.
 * - Iterates through directory entries until reaching the maximum file count or encountering an end-of-directory condition.
 * - Checks if each entry is a file (not a directory) and if it has a `.wav` extension.
 * - Stores valid filenames in the `fileNamesSDCard` array.
 * - Closes the directory after reading all entries.
 *
 * @param[in] path The directory path on the SD card to search for files.
 *
 * @note This function assumes that `fileNamesSDCard` and `fileCount` are global variables and that `MAX_FILES` and `MAX_FILENAME_LENGTH` are defined constants.
 */
void listFiles(const char *path)
{
    // Open the Directory
    fres = f_opendir(&dir, "/SamplePool");
    if (fres == FR_OK)
    {
        while (fileCount < MAX_FILES)
        {
            // Read the entry in the directory
            fres = f_readdir(&dir, &fno);
            if (fres != FR_OK || fno.fname[0] == 0) break;

            // Check whether it a file
            if (!(fno.fattrib & AM_DIR))
            {
                // It is a file
                char *fileName = fno.fname;

                // Check whether the file have a extension .wav
                if (strstr(fileName, ".wav") != NULL)
                {
                    // File with extension .wav get Safed
                    strncpy(fileNamesSDCard[fileCount + 1], fileName, MAX_FILENAME_LENGTH - 1);
                    fileNamesSDCard[fileCount][MAX_FILENAME_LENGTH - 1] = '\0'; // Null-terminieren
                    fileCount++;
                }
            }
        }
        f_closedir(&dir);
    }
}



//-------------------------------------------------------------------------------------------------------------------



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






