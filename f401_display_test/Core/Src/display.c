#include "display.h" 

// Define dimensions for display sections
#define DISPLAY_WIDTH           128
#define DISPLAY_HEIGHT          64
#define LIST_SECTION_HEIGHT     51
#define SELECTED_FILE_HEIGHT    10
#define BORDER_WIDTH            1
#define LINE_HEIGHT             10
#define CURSOR                  '>'

void displayStrings(I2C_HandleTypeDef *hi2c1, char** strings, uint8_t numStrings, uint8_t cursor_index) {
    // Clear screen
    ssd1306_Fill(Black);

    // Write data to list section of the screen
    for (uint8_t k = 0; k < numStrings; k++) {
        ssd1306_SetCursor(BORDER_WIDTH, k * LINE_HEIGHT + BORDER_WIDTH); // Adjust cursor_index position based on line height
        if(k == cursor_index){
            // Write the line with cursor_index
            char line_with_cursor[DISPLAY_WIDTH - BORDER_WIDTH];
            snprintf(line_with_cursor, sizeof(line_with_cursor), "%c %s", CURSOR, strings[k]);
            ssd1306_WriteString(line_with_cursor, Font_7x10, White);
        } else {
        ssd1306_WriteString(strings[k], Font_7x10, White);
        }
    }

    // Draw border around the list section
    for (uint8_t i = 0; i < DISPLAY_WIDTH; i++) {
        for (uint8_t j = 0; j < LIST_SECTION_HEIGHT; j++) {
            if (j < BORDER_WIDTH || j >= LIST_SECTION_HEIGHT - BORDER_WIDTH || i < BORDER_WIDTH || i >= DISPLAY_WIDTH - BORDER_WIDTH) {
                ssd1306_DrawPixel(i, j, White);
            }
        }
    }

    // Update display
    ssd1306_UpdateScreen(hi2c1);
}

void renderSelectedFile(I2C_HandleTypeDef *hi2c1, const char *filename) {
    // Clear the selected file section of the screen
    for (uint8_t i = 0; i < DISPLAY_WIDTH; i++) {
        for (uint8_t j = LIST_SECTION_HEIGHT; j < DISPLAY_HEIGHT; j++) {
            ssd1306_DrawPixel(i, j, Black);
        }
    }

    // Write the selected file name in the selected file section
    ssd1306_SetCursor(0, LIST_SECTION_HEIGHT + 2); // Adjust Y position for text alignment
    ssd1306_WriteString(filename, Font_7x10, White);

    // Update display
    ssd1306_UpdateScreen(hi2c1);
}
