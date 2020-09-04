#ifndef UART_H
#define UART_H

#include <stdio.h>

void uart_init(unsigned int baud_rate);
int uart_transmit(char data, FILE* file);
int uart_receive(FILE* file);
void uart_transmit_string(char* data);

#endif