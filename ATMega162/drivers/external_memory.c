#include "external_memory.h"

#include <avr/io.h>
#include <stdlib.h>

#define BASE_ADDRESS 0x1000

void external_memory_init() {
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM2);
}

void external_memory_write(uint8_t data, uint16_t offset_address) {
    volatile uint8_t* external_memory = (uint8_t *) BASE_ADDRESS;
    external_memory[offset_address] = data;
}

uint8_t external_memory_read(uint16_t offset_address) {
    volatile uint8_t* external_memory = (uint8_t *) BASE_ADDRESS;
    uint8_t value = external_memory[offset_address];
    return value;
}