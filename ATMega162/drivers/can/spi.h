/**
 * @file spi.h
 * @brief Driver for interfacing with SPI.
 */
#ifndef SPI_H_
#define SPI_H_

#include <stdio.h>

/**
 * @brief Sets up pins for MOSI, SCK, SS and MISO and specifies that the ATSam is master in of
 *        the SPI.
 */
void spi_init(void);

/**
 * @brief Transmits @p data on the SPI.
 */
void spi_write(uint8_t data);

/**
 * @brief Reads data from the SPI.
 */
uint8_t spi_read(void);

/**
 * @brief Sets the slave select pin to high.
 */
void spi_slave_select(void);

/**
 * @brief Sets the slave select pin to low.
 */
void spi_slave_deselect(void);

#endif