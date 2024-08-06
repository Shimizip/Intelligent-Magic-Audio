/**
  ******************************************************************************
  * @file           : display.h
  * @brief          : Header file for display functions and graphics rendering
  ******************************************************************************
  * @attention
  *
  * This file contains the function prototypes for managing and rendering graphics
  * on the display. It includes functions for:
  * - Displaying a list of strings on the screen (`displayStrings`).
  * - Rendering the currently selected file name (`renderSelectedFile`).
  * - Drawing waveforms based on sample data (`drawWaveform`).
  * - Drawing lines on the display (`drawLine`).
  * - Displaying fader percentage values (`drawFaderProzent`).
  *
  * These functions interact with the SSD1306 display using I2C communication and
  * are crucial for visual representation of data and user interface elements.
  *
  *******************************************************************************
  */


#include <math.h>
#include <stdlib.h>
#include "ssd1306.h"
#include "fonts.h"

void displayStrings(I2C_HandleTypeDef *hi2c1, char** strings, uint8_t numStrings, uint8_t cursor_index);
void renderSelectedFile(I2C_HandleTypeDef *hi2c1, const char *filename);
void drawWaveform(I2C_HandleTypeDef *hi2c1, int16_t *samples, uint32_t numSamples);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawFaderProzent(I2C_HandleTypeDef *hi2c1, const char *filename, int multiplikator);
