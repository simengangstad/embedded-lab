/**
 * @file can_joystick.c
 */

#include "can_joystick.h"

#include "can_controller.h"

void can_joystick_read(Joystick* joystick) {
    CAN_MESSAGE can_message;
    if (!can_receive(&can_message, 0)) {
        if (can_message.id == JOYSTICK_ID) {
            joystick->x = (int8_t)can_message.data[0];
            joystick->y = (int8_t)can_message.data[1];
            joystick->dir = (JoystickDirection)can_message.data[2];
        }
    }
}