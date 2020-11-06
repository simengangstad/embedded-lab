/**
 * @file game.h
 * @brief Main game logic module.
 */

#ifndef GAME_H
#define GAME_H

#include "drivers/can/can_input.h"

/**
 * @brief Updates the game state based on @p joystick_ptr and @p touch_input_ptr.
 */
void game_update(Joystick* joystick_ptr, TouchInput* touch_input_ptr);

#endif