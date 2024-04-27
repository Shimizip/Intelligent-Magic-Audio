#include "ssd1306.h"
#include "fonts.h"

void displayStrings(I2C_HandleTypeDef *hi2c1, char** strings, uint8_t numStrings, uint8_t cursor_index);
void renderSelectedFile(I2C_HandleTypeDef *hi2c1, const char *filename);