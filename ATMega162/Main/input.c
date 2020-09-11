#include "input.h"
#include "adc.h"

#include <util/delay.h>

#define CALIBRATION_STEPS				5
#define CALIBRATION_STEP_INTERVAL		5
#define JOYSTICK_DEADZONE				5

static uint8_t midpoint_x = 0;
static uint8_t midpoint_y = 0;

static void input_calibrate_joystick() {
	
	int midpoint_x_total = 0;
	int midpoint_y_total = 0;
	
	for (uint8_t i = 0; i < CALIBRATION_STEPS; i++) {
		midpoint_x_total += adc_read(ADC_JOYSTICK_X_CHANNEL);
		midpoint_y_total += adc_read(ADC_JOYSTICK_Y_CHANNEL);
		_delay_ms(CALIBRATION_STEP_INTERVAL);
	}
	
	midpoint_x = midpoint_x_total / 5;
	midpoint_y = midpoint_y_total / 5;
	
	printf("Joystick calibration done, %d, %d", midpoint_x, midpoint_y);
}

void input_init() {
	input_calibrate_joystick();
}

static int input_correct_joystick_from_calibration(uint8_t value, uint8_t midpoint) {
	const float delta_left = midpoint - 0;
	const float delta_right = 255 - midpoint;

	const int corrected_value = value - midpoint;

	if (corrected_value < 0) {
		return 100 * corrected_value / delta_left;
	}
	else {
		return 100 * corrected_value / delta_right;
	}
}

JoystickPos input_joystick_position() {
	JoystickPos position;
	
	position.x = input_correct_joystick_from_calibration(adc_read(ADC_JOYSTICK_X_CHANNEL), midpoint_x);
	position.y = input_correct_joystick_from_calibration(adc_read(ADC_JOYSTICK_Y_CHANNEL), midpoint_y);	
	
	return position;
}

JoystickDirection input_joystick_horizontal_direction() {
	int8_t value = input_correct_joystick_from_calibration(adc_read(ADC_JOYSTICK_X_CHANNEL), midpoint_x);
	
	if (value < -JOYSTICK_DEADZONE) {
		return LEFT;
	}
	else if (value > JOYSTICK_DEADZONE) {
		return RIGHT;
	}
	
	return NEUTRAL;
}

JoystickDirection input_joystick_vertical_direction() {
	int8_t value = input_correct_joystick_from_calibration(adc_read(ADC_JOYSTICK_Y_CHANNEL), midpoint_y);
	
	if (value < -JOYSTICK_DEADZONE) {
		return DOWN;
	}
	else if(value > JOYSTICK_DEADZONE) {
		return UP;
	}
	
	return NEUTRAL;
}

SliderPos input_slider_position() {
	SliderPos position;
		
	position.left  = 100 * adc_read(ADC_LEFT_SLIDER_CHANNEL) / 255.0;
	position.right = 100 * adc_read(ADC_RIGHT_SLIDER_CHANNEL) / 255.0;
		
	return position;
}