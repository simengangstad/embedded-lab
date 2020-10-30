/**
 * @file uart.h
 * @brief Driver for UART.
 */

#ifndef UART_H
#define UART_H

#include <stdio.h>

/**
 * @brief Initializes an UART connection with a given baud rate.
 *
 * @param baud_rate The baud rate (bits/s)
 */
void uart_init(unsigned int baud_rate);

/**
 * @brief Transmits a single byte.
 *
 * @param file File stream, not used within function, but required by fdevopen to use this function as a stream for
 printf function.

 * @return 0. Not essential for this function, required by fdevopen to use this function as a output function for
 printf.
 */
int uart_transmit(char data, FILE* file);

/**
 * @brief Receieves a byte from the UART line.
 *
 * @param file Not used, required by fdevopen to use this function as standard input stream.
 */
int uart_receive(FILE* file);

/**
 * @brief Transmits a whole string in the form of a char pointer.
 */
void uart_transmit_string(char* data);

#endif