
#define F_CPU 4915200
 
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "uart.h"
#include "external_memory.h"

int main(void) {
	uart_init(9600);
	external_memory_init();
	fdevopen(&uart_transmit, &uart_receive);
	external_memory_test();
	
    while (1) {
		external_memory_write(8, 0x800);
		unsigned char value = external_memory_read(0x800);
		printf("%i", value);
		_delay_ms(500);
    }
	return 0;
}

