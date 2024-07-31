#include "cpu_time.h"
#include "stm32f4xx_hal.h"  // Adjust the header according to your specific STM32 series

// Define pointers to the DWT and other relevant registers
volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)0xE0001004;
volatile unsigned int *DWT_CONTROL  = (volatile unsigned int *)0xE0001000;
volatile unsigned int *DWT_LAR      = (volatile unsigned int *)0xE0001FB0;
volatile unsigned int *SCB_DEMCR    = (volatile unsigned int *)0xE000EDFC;

// Global variables to store start and end cycle counts
static uint32_t start_cycles = 0;
static uint32_t end_cycles = 0;

/**
 * @brief Configures and enables the Data Watchpoint and Trace (DWT) unit.
 * 
 * Enables DWT and ITM, unlocks DWT registers, resets the cycle counter, 
 * and enables the cycle counter.
 */
void EnableDWT(void)
{
  *SCB_DEMCR |= 0x01000000;      // Enable DWT and ITM
  *DWT_LAR = 0xC5ACCE55;         // Unlock DWT registers
  *DWT_CYCCNT = 0;               // Reset the cycle counter
  *DWT_CONTROL |= 1;             // Enable the cycle counter
}

/**
 * @brief Starts cycle measurement.
 * 
 * Records the current cycle count from the DWT cycle counter to `start_cycles`.
 */
void StartCycleMeasurement(void)
{
  start_cycles = *DWT_CYCCNT;    // Store the cycle count at the start
}

/**
 * @brief Stops cycle measurement.
 * 
 * Records the current cycle count from the DWT cycle counter to `end_cycles`.
 */
void StopCycleMeasurement(void)
{
  end_cycles = *DWT_CYCCNT;      // Store the cycle count at the end
}

/**
 * @brief Retrieves the measured number of cycles.
 * 
 * Calculates the difference between `end_cycles` and `start_cycles` to return
 * the number of cycles elapsed.
 * 
 * @return The number of cycles measured.
 */
uint32_t GetMeasuredCycles(void)
{
  return end_cycles - start_cycles; // Return the difference in cycle counts
}

/**
 * @brief Converts cycle count to milliseconds.
 * 
 * Converts a given number of cycles to milliseconds based on the system clock frequency.
 * 
 * @param cycles The number of cycles to convert.
 * @return The equivalent time in milliseconds.
 */
uint32_t CyclesToMilliseconds(uint32_t cycles)
{
  uint32_t cpu_clock_hz = HAL_RCC_GetSysClockFreq(); // Get the system clock frequency
  return (cycles / (cpu_clock_hz / 1000));           // Convert to milliseconds
}

/**
 * @brief Converts cycle count to microseconds.
 * 
 * Converts a given number of cycles to microseconds based on the system clock frequency.
 * 
 * @param cycles The number of cycles to convert.
 * @return The equivalent time in microseconds.
 */
uint32_t CyclesToMicroseconds(uint32_t cycles)
{
  uint32_t cpu_clock_hz = HAL_RCC_GetSysClockFreq(); // Get the system clock frequency
  return (cycles / (cpu_clock_hz / 1000000));   // Convert to microseconds
}