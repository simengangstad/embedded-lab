#ifndef SPI_H_
#define SPI_H_

#include <stdio.h>

void spi_init(void);

void spi_write(uint8_t data);

uint8_t spi_read(void);

void spi_slave_deselect(void);

void spi_slave_select(void);

#endif /* SPI_H_ */