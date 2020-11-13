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
 * @brief Defines the state of the joystick.
 */
typedef struct {
    int8_t x, y;
    JoystickDirection dir;
    uint8_t button_pressed;
} Joystick;

/**
 * @brief Defines the state of the touch input.
 */
typedef struct {
    int8_t left_slider, right_slider;
    uint8_t left_button, right_button;
} TouchInput;

/**
 * @brief Sets up the input pins and does an initial calibration of the joysticks.
 */
void input_init(void);

/**
 * @return The current joystick state.
 */
Joystick input_joystick(void);

/**
 * @return The current touch input.
 */
TouchInput input_touch(void);

/**
 * @brief Runs a while loop outputting the different inputs.
 */
void input_test(void);

#endif