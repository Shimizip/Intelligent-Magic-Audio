#ifndef UTIL_H
#define UTIL_H

#include "main.h"
#include <stdarg.h> //for va_list var arg functions
#include <stdio.h>
#include <string.h>
// #include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;

void uart_printf(const char *fmt, ...);

#endif