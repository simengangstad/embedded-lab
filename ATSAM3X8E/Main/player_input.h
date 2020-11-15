/**
 * @file player_input.h
 * @brief Interface for reading inputs from the CAN bus. The inputs are joystick and touch messages, as well as a
 * message for starting the game. The module keeps the current inputs in static variables. An interrupt is called on
 * receive, which updates the relevant input variable. The get functions returns the static variables, and the reset
 * function makes the inputs ready for the next game. Keeps a flag telling the game to start, which must be reset after
 * the game is started.
 */

#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include <stdio.h>

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
 * @brief Initializes the input.
 */
void player_input_init(void);

/**
 * @brief Resets all inputs.
 */
void player_input_reset(void);

/**
 * @brief Get current joystick values.
 */
void player_input_get_joystick(Joystick* js);

/**
 * @brief Get current touch input.
 */
void player_input_get_touch_input(TouchInput* ti);

/**
 * @brief Checks the game start flag.
 */
uint8_t player_input_game_start_flag(void);

/**
 * @brief Clears the game start flag.
 */
void player_input_clear_game_start_flag(void);

#endif