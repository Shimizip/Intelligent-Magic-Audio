#include "ssd1306.h"


// Screenbuffer
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// Screen object
static SSD1306_t SSD1306;


//
//  Send a byte to the command register
//
static uint8_t ssd1306_WriteCommand(I2C_HandleTypeDef *hi2c, uint8_t command)
{
    return HAL_I2C_Mem_Write(hi2c, SSD1306_I2C_ADDR, 0x00, 1, &command, 1, 10);
}


//
//  Initialize the oled screen
//
uint8_t ssd1306_Init(I2C_HandleTypeDef *hi2c)
{
    // Wait for the screen to boot
    HAL_Delay(100);
    int status = 0;

    // Init LCD

    status += ssd1306_WriteCommand(hi2c, SH110X_DISPLAYOFF);               // 0xAE

    status += ssd1306_WriteCommand(hi2c, SH110X_SETDISPLAYCLOCKDIV);
    status += ssd1306_WriteCommand(hi2c, 0x51); // 0xd5, 0x51,

    status += ssd1306_WriteCommand(hi2c, SH110X_MEMORYMODE);
status += ssd1306_WriteCommand(hi2c, 0x00); // 0x00 for Horizontal addressing mode

    status += ssd1306_WriteCommand(hi2c, SH110X_SETCONTRAST); 
    status += ssd1306_WriteCommand(hi2c, 0x4F);       // 0x81, 0x4F

    status += ssd1306_WriteCommand(hi2c, SH110X_DCDC) ;
    status += ssd1306_WriteCommand(hi2c, 0x8A);               // 0xAD, 0x8A

    status += ssd1306_WriteCommand(hi2c, SH110X_SEGREMAP);                 // 0xA0

    status += ssd1306_WriteCommand(hi2c, SH110X_COMSCANINC);               // 0xC0

    status += ssd1306_WriteCommand(hi2c, SH110X_SETDISPSTARTLINE);
    status += ssd1306_WriteCommand(hi2c, 0x0);    // 0xDC 0x00

    status += ssd1306_WriteCommand(hi2c, SH110X_SETDISPLAYOFFSET); 
    status += ssd1306_WriteCommand(hi2c, 0x60);   // 0xd3, 0x60,

    status += ssd1306_WriteCommand(hi2c, SH110X_SETPRECHARGE); 
    status += ssd1306_WriteCommand(hi2c, 0x22);       // 0xd9, 0x22,

    status += ssd1306_WriteCommand(hi2c, SH110X_SETVCOMDETECT); 
    status += ssd1306_WriteCommand(hi2c, 0x35);      // 0xdb, 0x35,

    status += ssd1306_WriteCommand(hi2c, SH110X_SETPAGEADDR);                  // 0xb0

    // status += ssd1306_WriteCommand(hi2c, H110X_SETCOMPINS);
    // status += ssd1306_WriteCommand(hi2c,  0x12);             // 0xda, 0x12,
    status += ssd1306_WriteCommand(hi2c, SH110X_DISPLAYALLON_RESUME); // 0xa4

    status += ssd1306_WriteCommand(hi2c, SH110X_NORMALDISPLAY);       // 0xa6

    status += ssd1306_WriteCommand(hi2c, SH110X_SETDISPLAYOFFSET); 
    status += ssd1306_WriteCommand(hi2c, 0x00);

    status += ssd1306_WriteCommand(hi2c, SH110X_SETMULTIPLEX);
    status += ssd1306_WriteCommand(hi2c, 0x7F);

    status += ssd1306_WriteCommand(hi2c, SH110X_DISPLAYON);

    if (status != 0) {
        return 1;
    }

    // Clear screen
    ssd1306_Fill(Black);

    // Flush buffer to screen
    ssd1306_UpdateScreen(hi2c);

    // Set default values for screen object
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;

    SSD1306.Initialized = 1;

    return 0;
}

//
//  Fill the whole screen with the given color
//
void ssd1306_Fill(SSD1306_COLOR color)
{
    // Fill screenbuffer with a constant value (color)
    uint32_t i;

    for(i = 0; i < sizeof(SSD1306_Buffer); i++)
    {
        SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
    }
}

//
//  Write the screenbuffer with changed to the screen
//
void ssd1306_UpdateScreen(I2C_HandleTypeDef *hi2c)
{
    uint8_t i;

    for (i = 0; i < 16; i++) {
        ssd1306_WriteCommand(hi2c, 0xB0 + i);
        ssd1306_WriteCommand(hi2c, 0x00);
        ssd1306_WriteCommand(hi2c, 0x10);

        HAL_I2C_Mem_Write(hi2c, SSD1306_I2C_ADDR, 0x40, 1, &SSD1306_Buffer[SSD1306_WIDTH * i], SSD1306_WIDTH, 100);
    }
}

//
//  Draw one pixel in the screenbuffer
//  X => X Coordinate
//  Y => Y Coordinate
//  color => Pixel color
//
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        // Don't write outside the buffer
        return;
    }

    // Check if pixel should be inverted
    if (SSD1306.Inverted)
    {
        color = (SSD1306_COLOR)!color;
    }

    // Draw in the correct color
    if (color == White)
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    }
    else
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}


//
//  Draw 1 char to the screen buffer
//  ch      => Character to write
//  Font    => Font to use
//  color   => Black or White
//
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color)
{
    uint32_t i, b, j;

    // Check remaining space on current line
    if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
        SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))
    {
        // Not enough space on current line
        return 0;
    }

    // Translate font to screenbuffer
    for (i = 0; i < Font.FontHeight; i++)
    {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for (j = 0; j < Font.FontWidth; j++)
        {
            if ((b << j) & 0x8000)
            {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
            }
            else
            {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
            }
        }
    }

    // The current space is now taken
    SSD1306.CurrentX += Font.FontWidth;

    // Return written char for validation
    return ch;
}

//
//  Write full string to screenbuffer
//
char ssd1306_WriteString(const char* str, FontDef Font, SSD1306_COLOR color)
{
    // Write until null-byte
    while (*str)
    {
        if (ssd1306_WriteChar(*str, Font, color) != *str)
        {
            // Char could not be written
            return *str;
        }

        // Next char
        str++;
    }

    // Everything ok
    return *str;
}

//
//  Invert background/foreground colors
//
void ssd1306_InvertColors(void)
{
    SSD1306.Inverted = !SSD1306.Inverted;
}

//
//  Set cursor_index position
//
void ssd1306_SetCursor(uint8_t x, uint8_t y)
{
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}
