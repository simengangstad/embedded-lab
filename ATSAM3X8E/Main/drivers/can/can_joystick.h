/**
 * @file can_joystick.h
 * @brief Interface for reading Joystick messages from the controlled area network.
 */
#ifndef CAN_JOYSTICK_H
#define CAN_JOYSTICK_H

#include <stdio.h>

/**
 * @brief Message ID of the joystick.
 */
#define JOYSTICK_ID 1

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
} Joystick;

/**
 * @brief Reads the CAN bus for the joystick message and places it in @p joystick.
 */
void can_joystick_read(Joystick* joystick);

#endif