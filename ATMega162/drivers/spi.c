#include "spi.h"

#include <avr/io.h>

void spi_init() {
    // Set MOSI, SCK and SS (PB4) output
    DDRB |= (1 << DDB5) | (1 << DDB7) | (1 << DDB4);

    // Set MISO to input
    DDRB &= ~(1 << DDB6);

    // Enable SPI, Master, set clock rate fck/16. CPOL and CPHA is zero, makes SPI mode 0
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_write(uint8_t data) {
    // Start transmisson
    SPDR = data;

    // Wait for transmission to complete
    while (!(SPSR & (1 << SPIF)))
        ;
}

uint8_t spi_read(void) {
    // Write dummy byte and starts transmission
    SPDR = 0xFF;

    // Wait for transmission to complete
    while (!(SPSR & (1 << SPIF)))
        ;

    return SPDR;
}

void spi_slave_deselect(void) { PORTB |= (1 << PB4); }

void spi_slave_select(void) { PORTB &= ~(1 << PB4); }