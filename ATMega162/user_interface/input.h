#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

/**
 * @brief Defines the direction of the joystick.
 */
typedef enum { UP, DOWN, LEFT, RIGHT, NEUTRAL } JoystickDirection;

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
 * @return The current direction of the joystick.
 */
JoystickDirection input_joystick_direction();

/**
 * @return The current slider positions.
 */
SliderPosition input_slider_position();

/**
 * @return 0/1 when not pressed / pressed.
 */
uint8_t input_left_button_pressed();

/**
 * @return 0/1 when not pressed / pressed.
 */
uint8_t input_right_button_pressed();

/**
 * @return 0/1 when not pressed / pressed.
 */
uint8_t input_joystick_button_pressed();

/**
 * @brief Runs a while loop outputting the different inputs.
 */
void input_test();

#endif