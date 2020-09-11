
#define F_CPU 4915200
 
#include <stdio.h>
#include <util/delay.h>

#include "uart.h"
#include "external_memory.h"
#include "input.h"

void initialize_atmega() {
	uart_init(9600);
	external_memory_init(); 
	fdevopen(&uart_transmit, &uart_receive);
	adc_init();
	input_init();
}

int main(void) {
	initialize_atmega();

    while (1) {

		JoystickPos joystick_position = input_joystick_position();
		SliderPos slider_position = input_slider_position();
		//printf("x: %d \r\n", x);
		printf("x: %d, y: %d, left: %d, right: %d \r\n", joystick_position.x, joystick_position.y, slider_position.left, slider_position.right);
		_delay_ms(200);
	}
	return 0;
}

