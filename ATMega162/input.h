#ifndef INPUT_H_
#define INPUT_H_

#include <stdio.h>

/**
 * @brief Defines the direction of the joystick.
 */
typedef enum {
	UP, DOWN, LEFT, RIGHT, NEUTRAL
} JoystickDirection;

/**
 * @brief Defines the position of the joystick.
 */
typedef struct {
	int8_t x, y;
} JoystickPosition;

/**
 * @brief Defines the position of the sliders. 
 */
typedef struct {
	int8_t left, right;
} SliderPosition;

/**
 * @brief Sets up the input and does an initial calibration of the joysticks.
 */
void input_init();

/**
 * @return The current joystick position. 
 */
JoystickPosition input_joystick_position();

/**
 * @return The current horizontal direction of the joystick.
 */
JoystickDirection input_joystick_horizontal_direction();

/**
 * @return The current vertical direction of the joystick.
 */
JoystickDirection input_joystick_vertical_direction();

/**
 * @return The current slider positions.
 */
SliderPosition input_slider_position();

/**
 * @return Whether the left touch button is pressed.
 */
uint8_t input_left_button_pressed();

/**
 * @return Whether the right touch button is pressed.
 */
uint8_t input_right_button_pressed();


#endif