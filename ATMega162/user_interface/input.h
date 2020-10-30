/**
 * @file input.h
 * @brief High level interface for the inputs on the multifunction board.
 */

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
 * @brief Sets up the input pins and does an initial calibration of the joysticks.
 */
void input_init(void);

/**
 * @return The current joystick position.
 */
JoystickPosition input_joystick_position(void);

/**
 * @return The current direction of the joystick.
 */
JoystickDirection input_joystick_direction(void);

/**
 * @return The current slider positions.
 */
SliderPosition input_slider_position(void);

/**
 * @return 0/1 when not pressed / pressed.
 */
uint8_t input_left_button_pressed(void);

/**
 * @return 0/1 when not pressed / pressed.
 */
uint8_t input_right_button_pressed(void);

/**
 * @return 0/1 when not pressed / pressed.
 */
uint8_t input_joystick_button_pressed(void);

/**
 * @brief Runs a while loop outputting the different inputs.
 */
void input_test(void);

#endif