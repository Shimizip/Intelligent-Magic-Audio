#include "display.h" 

void displayStrings(I2C_HandleTypeDef *hi2c1, char** strings, uint8_t numStrings) {
    // Clear screen
    ssd1306_Fill(Black);

    // Write data to local screen buffer
    for (uint8_t k = 0; k < numStrings; k++) {
        ssd1306_SetCursor(0, k * 8); // Assuming each line of text is 8 pixels high
        ssd1306_WriteString(strings[k], Font_7x10, White);
    }

    // Draw rectangle border on screen
    for (uint8_t i = 0; i < 128; i++) {
        for (uint8_t j = 0; j < 64; j++) {
            // Draw top and bottom borders
            if (j == 0 || j == 63) {
                ssd1306_DrawPixel(i, j, White);
            }
            // Draw left and right borders
            else if (i == 0 || i == 127) {
                ssd1306_DrawPixel(i, j, White);
            }
        }
    }

    // Update display
    ssd1306_UpdateScreen(hi2c1);
}