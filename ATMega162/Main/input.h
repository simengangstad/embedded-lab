#ifndef INPUT_H_
#define INPUT_H_

#include <stdio.h>

typedef enum {
	UP, DOWN, LEFT, RIGHT, NEUTRAL
} JoystickDirection;

typedef struct {
	int8_t x, y;
} JoystickPos;

typedef struct {
	int8_t left, right;
} SliderPos;


void input_init();
JoystickPos input_joystick_position();
JoystickDirection input_joystick_horizontal_direction();
JoystickDirection input_joystick_vertical_direction();
SliderPos input_slider_position();

#endif