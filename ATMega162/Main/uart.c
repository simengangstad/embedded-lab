#include "uart.h"

#include <avr/io.h>

#define FOSC 4915200

void uart_init(unsigned int baud_rate) {
	unsigned int ubrr = FOSC/16/baud_rate - 1;
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: 8 data, 2 stop bit
	UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
}

int uart_transmit(char data, FILE* file) {
	// Wait for empty transmit buffer
	while( !( UCSR0A & (1 << UDRE0)) );
	// Put data into buffer, sends the data
	UDR0 = data;
	
	return 0;
}

int uart_receive(FILE* file) {
	// Wait for data to be received
	while( !(UCSR0A & (1 << RXC0)) );
	// Get and return received data from buffer
	return UDR0;
}

void uart_transmit_string(char* data) {
	while (*data != '\0'){
		uart_transmit(*data, NULL);
		++data;
	}
}