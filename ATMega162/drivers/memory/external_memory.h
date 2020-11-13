/**
 * @file external_memory.h
 * @brief Driver for interfacing with the SRAM.
 */

#ifndef EXTERNAL_MEMORY
#define EXTERNAL_MEMORY

#include <stdio.h>

#define SRAM_OFFSET 0x800
#define ADC_OFFSET 0x400
#define OLED_COMMAND_OFFSET 0x000
#define OLED_DATA_OFFSET 0x200

/**
 * @brief Sets up the to use external memory and the number of bits used for addressing.
 */
void external_memory_init(void);

/**
 * @brief Writes a byte of data to external memory.
 *
 * @param data The byte to write.
 * @param offset_address The address to write to, offset by base address 0x1000.
 */
void external_memory_write(uint8_t data, uint16_t offset_address);

/**
 * @brief Retrieve a value from memory.
 *
 * @param offset_address The data address, offset by base address 0x1000.
 */
uint8_t external_memory_read(uint16_t offset_address);

/**
 * @brief Tests the memory space by writing and reading to different locations.
 */
void external_memory_sram_test();

#endif