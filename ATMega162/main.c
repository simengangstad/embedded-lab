#define F_CPU 4915200
 
#include <stdio.h>
#include <util/delay.h>

#include "drivers/uart.h"
#include "drivers/external_memory.h"
#include "drivers/adc.h"
#include "input.h"

void initialize_atmega() {
	uart_init(9600);
	external_memory_init(); 
	fdevopen(&uart_transmit, &uart_receive);
	adc_init();
	input_init();
}

char* get_string_from_joystick_direction(JoystickDirection joystick_direction) {
	switch (joystick_direction) {
		case UP:
			return "up";
		case DOWN:
			return "down";
		case LEFT:
			return "left";
		case RIGHT:
			return "right";
		default:
			return "neutral";
	}

}

int main(void) {
	initialize_atmega();

    while (1) {

		JoystickPosition joystick_position = input_joystick_position();
		SliderPosition slider_position = input_slider_position();
		uint8_t left_button = input_left_button_pressed();
		uint8_t right_button = input_right_button_pressed();
		const char* horizontal_direction = get_string_from_joystick_direction(input_joystick_horizontal_direction());
		const char* vertical_direction = get_string_from_joystick_direction(input_joystick_vertical_direction());

		printf("x: %d, y: %d, vertical: %s, horizontal: %s, ", joystick_position.x, joystick_position.y, vertical_direction, horizontal_direction);
		printf("left slider: %d, right slider: %d, left button: %i, right button: %i \r\n", slider_position.left, slider_position.right, left_button, right_button);
		_delay_ms(200);
	}
	return 0;
}

