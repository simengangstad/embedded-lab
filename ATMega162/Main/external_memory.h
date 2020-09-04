#ifndef EXTERNAL_MEMORY
#define EXTERNAL_MEMORY

/**
 * @brief Sets up the to use external memory and the number of bits used for addressing.
 */
void external_memory_init();

/**
 * @brief Writes a byte of data to external memory. 
 * 
 * @param data The byte to write. 
 * @param offset_address The address to write to, offset by base address 0x1000. 
 */
void external_memory_write(unsigned char data, unsigned short offset_address);

/**
 * @brief Retrieve a value from memory.
 * 
 * @param offset_address The data address, offset by base address 0x1000.
 *
 * @return The data at the given address.
 */
unsigned char external_memory_read(unsigned short offset_address);

/**
 * @brief Tests the memory space by writing and reading to different locations.
 */
void external_memory_test();

#endif