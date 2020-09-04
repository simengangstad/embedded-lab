
#define F_CPU 4915200
 
#include <avr/io.h>
#include "util/delay.h"
#include "uart.h"
#include <stdio.h>

int main(void) {
	uart_init(9600);
	fdevopen(&uart_transmit, &uart_receive);
	
    while (1) {
		printf("Hello world!\r\n");
		_delay_ms(500);
    }
	return 0;
}

