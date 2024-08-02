#include "display.h" 
#include <stdio.h>

// Define dimensions for display sections
#define DISPLAY_WIDTH           128
#define DISPLAY_HEIGHT          128
#define LIST_SECTION_HEIGHT     81
#define SELECTED_FILE_HEIGHT    10
#define BORDER_WIDTH            1
#define LINE_HEIGHT             10
#define CURSOR                  '>'
#define WAVEFORM_TOP        0
#define WAVEFORM_BOTTOM     (LIST_SECTION_HEIGHT - BORDER_WIDTH)

#define SAMPLE_SKIP             2

// Global variables to keep track of the visible portion of the list
static uint8_t first_visible_index = 0;
static uint8_t num_visible_lines = LIST_SECTION_HEIGHT / LINE_HEIGHT;

/**
 * @brief Displays a list of strings on the OLED screen with cursor highlighting.
 *
 * This function renders a list of strings on the OLED display, highlighting the current selection with a cursor. 
 * It also manages the display of strings based on the current cursor position and adjusts the visible portion of the list as needed.
 *
 * - Clears the screen before drawing new content.
 * - Calculates the visible range of strings based on the `cursor_index` and ensures that the cursor is within the visible range.
 * - Draws each string on the display, with the current selection highlighted by a cursor.
 * - Draws a border around the list section of the screen.
 *
 * @param[in] hi2c1 Pointer to the I2C handle used for communication with the OLED display.
 * @param[in] strings Array of string pointers to be displayed.
 * @param[in] numStrings Total number of strings in the array.
 * @param[in] cursor_index Index of the currently selected string, which will be highlighted on the display.
 *
 * @note Ensure that the `strings` array is correctly populated and `numStrings` reflects the actual number of valid strings. The display dimensions and font sizes should be configured to match the display hardware.
 */
void displayStrings(I2C_HandleTypeDef *hi2c1, char** strings, uint8_t numStrings, uint8_t cursor_index) {
    // Clear screen
    ssd1306_Fill(Black);

    // Calculate the last visible index based on the first visible index and the number of visible lines
    uint8_t last_visible_index = first_visible_index + num_visible_lines - 1;

    // Ensure cursor is within the visible range
    if (cursor_index < first_visible_index) {
        first_visible_index = cursor_index;
    } else if (cursor_index > last_visible_index) {
        first_visible_index = cursor_index - num_visible_lines + 1;
    }

    // Write data to list section of the screen
    for (uint8_t k = first_visible_index; k < numStrings && k < first_visible_index + num_visible_lines; k++) {
        ssd1306_SetCursor(BORDER_WIDTH, (k - first_visible_index) * LINE_HEIGHT + BORDER_WIDTH); // Adjust cursor_index position based on line height
        if (k == cursor_index) {
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
}

/**
 * @brief Scrolls the list up by one line.
 *
 * This function updates the `first_visible_index` to scroll the displayed list upward by one line. 
 * It ensures that the `first_visible_index` does not go below zero, which would be out of bounds for the visible portion of the list.
 *
 * @note If `first_visible_index` is already at the top of the list (i.e., 0), 
 * the function does nothing to avoid invalid index access.
 */
void scrollUp() {
    if (first_visible_index > 0) {
        first_visible_index--;
    }
}

/**
 * @brief Scrolls the list down by one line.
 *
 * This function updates the `first_visible_index` to scroll the displayed list downward by one line. 
 * 
 * It ensures that the `first_visible_index` does not exceed the number of available strings (`numStrings`), 
 * preventing it from scrolling beyond the end of the list.
 *
 * @param[in] numStrings Total number of strings available in the list.
 *
 * @note If scrolling down would cause the `first_visible_index` to go beyond the last item in the list, 
 * the function does nothing to avoid invalid index access.
 */

void scrollDown(uint8_t numStrings) {
    if (first_visible_index + num_visible_lines < numStrings) {
        first_visible_index++;
    }
}
/**
 * @brief Renders the selected file name on the OLED screen.
 *
 * This function updates the display to show the currently selected file name in a dedicated section of the screen. 
 * It first clears the area designated for displaying the selected file name, and then writes the file name at the appropriate position.
 *
 * @param[in] hi2c1 Pointer to the I2C handle used for communication with the OLED display.
 * @param[in] filename Pointer to a null-terminated string representing the name of the selected file to be displayed.
 *
 * @note Ensure that the `filename` is properly null-terminated and that the display dimensions 
 * and font settings are configured correctly to match the hardware.
 */

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
}

/*void drawWaveform(I2C_HandleTypeDef *hi2c1, int16_t *samples, uint32_t numSamples) {
    // Clear the waveform area
    for (uint8_t i = 0; i < DISPLAY_WIDTH; i++) {
        for (uint8_t j = WAVEFORM_TOP; j < WAVEFORM_BOTTOM; j++) {
            ssd1306_DrawPixel(i, j, Black);
        }
    }

    // Draw waveform
    // uint32_t x_step = DISPLAY_WIDTH / numSamples;
    uint8_t x_step = numSamples / DISPLAY_WIDTH;

    for (uint32_t i = 0; i < DISPLAY_WIDTH; i ++) {

        int8_t x1 = i;
        int8_t y1 = (samples[i * x_step] * (WAVEFORM_BOTTOM - WAVEFORM_TOP) ) / 65536 + (WAVEFORM_BOTTOM - WAVEFORM_TOP)/2;
        // uint32_t y2 = (samples[i + SAMPLE_SKIP] * (WAVEFORM_BOTTOM - WAVEFORM_TOP) / 32767) + WAVEFORM_TOP;
        // drawLine(x1, y1, x1, y2, White);
        ssd1306_DrawPixel(x1, y1, White);
        // ssd1306_UpdateScreen(hi2c1);
    }

    // Draw border around the waveform area
    for (uint8_t i = 0; i < DISPLAY_WIDTH; i++) {
        for (uint8_t j = WAVEFORM_TOP; j < WAVEFORM_BOTTOM; j++) {
            if (j == WAVEFORM_TOP || j == WAVEFORM_BOTTOM - 1 || i == 0 || i == DISPLAY_WIDTH - 1) {
                ssd1306_DrawPixel(i, j, White);
            }
        }
    }

    // Update display
    ssd1306_UpdateScreen(hi2c1);
}*/

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t dx = abs(x1 - x0);
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0);
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy;
    int16_t e2;

    while (1) {
        ssd1306_DrawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

/**
 * @brief Draws the fader percentage on the OLED screen at a specified vertical position.
 *
 * This function updates the display to show the fader percentage in a designated area of the screen. 
 * It first clears the region where the fader percentage will be displayed, and then writes the percentage at a vertical position determined by the `multiplikator`.
 *
 * The `multiplikator` parameter is used to calculate the vertical offset from a base position on the screen. 
 * This allows for the display of multiple fader percentages at different vertical positions.
 *
 * @param[in] hi2c1 Pointer to the I2C handle used for communication with the OLED display.
 * @param[in] prozent Pointer to a null-terminated string representing the fader percentage to be displayed.
 * @param[in] multiplikator Integer value used to adjust the vertical position on the screen, affecting where the percentage is drawn.
 *
 * @note Ensure that the `prozent` string is properly null-terminated and that the display dimensions and font settings are configured correctly to match the hardware. 
 * The `multiplikator` should be set appropriately to ensure that the displayed text does not overlap with other screen elements.
 */
void drawFaderProzent(I2C_HandleTypeDef *hi2c1, const char *prozent, int multiplikator){
    // Clear the selected file section of the screen
    for (uint8_t i = 0; i < DISPLAY_WIDTH; i++) {
        for (uint8_t j = LIST_SECTION_HEIGHT + (18 * multiplikator); j < DISPLAY_HEIGHT; j++) {
            ssd1306_DrawPixel(i, j, Black);
        }
    }
    // Write the selected file name in the selected file section
    ssd1306_SetCursor(0, LIST_SECTION_HEIGHT + (18 * multiplikator)); // Adjust Y position for text alignment
    ssd1306_WriteString(prozent, Font_7x10, White);
}
