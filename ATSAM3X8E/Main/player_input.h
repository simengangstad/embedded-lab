/**
 * @file player_input.h
 * @brief Interface for reading joystick and touch messages from the controlled area network.
 */
#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include <stdio.h>

/**
 * @brief Message ID of the joystick.
 */
#define JOYSTICK_ID 1

/**
 * @brief Message ID of the touch inputs.
 */
#define TOUCH_INPUT_ID 2

/**
 * @brief Defines the direction of the joystick.
 */
typedef enum { JS_UP, JS_DOWN, JS_LEFT, JS_RIGHT, JS_NEUTRAL } JoystickDirection;

/**
 * @brief Defines the position of the joystick.
 */
typedef struct {
    int8_t x, y;
    JoystickDirection dir;
    uint8_t button_pressed;
} Joystick;

/**
 * @brief Defines the touch input.
 */
typedef struct {
    int8_t left_slider, right_slider;
    uint8_t left_button, right_button;
} TouchInput;

/**
 * @brief update current joystick-values 
 * 
 * @param js 
 */
void player_input_get_joystick(Joystick* js);

/**
 * @brief update current touch 
 * 
 * @param ti 
 */
void player_input_get_touch_input(TouchInput* ti);

#endif