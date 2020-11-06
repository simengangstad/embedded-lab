/**
 * @file can_joystick.c
 */

#include "can_input.h"

#include "can_controller.h"

void can_input_read(Joystick* joystick, TouchInput* touch_input) {
    CAN_MESSAGE can_message;
    if (!can_receive(&can_message, 0)) {
        if (can_message.id == JOYSTICK_ID) {
            joystick->x = (int8_t)can_message.data[0];
            joystick->y = (int8_t)can_message.data[1];
            joystick->dir = (JoystickDirection)can_message.data[2];
            joystick->button_pressed = can_message.data[3];
        }
        else if (can_message.id == TOUCH_INPUT_ID) {
            touch_input->left_slider = (int8_t)can_message.data[0];
            touch_input->right_slider = (int8_t)can_message.data[1];
            touch_input->left_button = (uint8_t)can_message.data[2];
            touch_input->right_button = (uint8_t)can_message.data[3];
        }
    }
}