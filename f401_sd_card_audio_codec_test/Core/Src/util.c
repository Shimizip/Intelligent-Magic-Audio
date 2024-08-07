/**
 * @file util.c
 * @brief Utility functions for the project.
 * 
 * This file contains utility functions used across the project. Currently, it includes
 * a function to format and send strings over UART.
 * 
 * Functions provided:
 * - `void uart_printf(const char *fmt, ...)`: Formats and sends a string over UART.
 * 
 * The `uart_printf` function:
 * - Uses `vsnprintf` to format a string.
 * - Uses `HAL_UART_Transmit` to send the formatted string over UART2.
 */
#include "util.h"

/**
 * @brief Formats and sends a string over UART.
 * 
 * Uses `vsnprintf` to format the string and `HAL_UART_Transmit` to send it over UART2.
 * 
 * @param fmt Format string.
 * @param ... Arguments for formatting.
 */
void uart_printf(const char *fmt, ...) {
  static char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, -1);
}