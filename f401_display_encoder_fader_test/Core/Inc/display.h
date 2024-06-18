#include <math.h>
#include <stdlib.h>
#include "ssd1306.h"
#include "fonts.h"

void displayStrings(I2C_HandleTypeDef *hi2c1, char** strings, uint8_t numStrings, uint8_t cursor_index);
void renderSelectedFile(I2C_HandleTypeDef *hi2c1, const char *filename);
void drawWaveform(I2C_HandleTypeDef *hi2c1, int16_t *samples, uint32_t numSamples);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawFaderProzent(I2C_HandleTypeDef *hi2c1, const char *filename);
