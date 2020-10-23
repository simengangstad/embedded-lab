
#ifndef CAN_JOYSTICK_H_
#define CAN_JOYSTICK_H_

#include <stdio.h>

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

void can_joystick_read(Joystick* joystick);

#endif /* CAN_JOYSTICK_H_ */