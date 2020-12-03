/**
 * @file player_input.c
 */

#include "player_input.h"

#include "can_identifiers.h"
#include "drivers/can_controller.h"
#include "sam.h"

static Joystick joystick;

static TouchInput touch_input;

static uint8_t game_start_flag = 0;

void player_input_init(void) { player_input_reset(); }

uint8_t player_input_game_start_flag(void) { return game_start_flag; }

void player_input_clear_game_start_flag(void) { game_start_flag = 0; }

void player_input_get_joystick(Joystick* js) {
    js->x = joystick.x;
    js->y = joystick.y;
    js->dir = joystick.dir;
    js->button_pressed = joystick.button_pressed;
}

void player_input_get_touch_input(TouchInput* ti) {
    ti->left_slider = touch_input.left_slider;
    ti->right_slider = touch_input.right_slider;
    ti->left_button = touch_input.left_button;
    ti->right_button = touch_input.right_button;
}

void player_input_reset() {
    touch_input.left_slider = 0;
    touch_input.right_slider = 0;
    touch_input.left_button = 0;
    touch_input.right_button = 0;

    joystick.x = 0;
    joystick.y = 0;
    joystick.dir = JS_NEUTRAL;
    joystick.button_pressed = 0;
}

/**
 * @brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 */
void CAN0_Handler(void) {
    char can_sr = CAN0->CAN_SR;

    // RX interrupt
    if (can_sr & (CAN_SR_MB1 | CAN_SR_MB2))  // Only mailbox 1 and 2 specified for receiving
    {
        CAN_MESSAGE message;

        if (can_sr & CAN_SR_MB1) {
            // Mailbox 1 event
            can_receive(&message, 1);

        } else if (can_sr & CAN_SR_MB2) {
            // Mailbox 2 event
            can_receive(&message, 2);
        } else {
            printf("CAN0 message arrived in non-used mailbox\n\r");
        }

        switch (message.id) {
            case CAN_JOYSTICK_MESSAGE_ID:
                joystick.x = (int8_t)message.data[0];
                joystick.y = (int8_t)message.data[1];
                joystick.dir = (JoystickDirection)message.data[2];
                joystick.button_pressed = message.data[3];
                break;
            case CAN_TOUCH_MESSAGE_ID:
                touch_input.left_slider = (int8_t)message.data[0];
                touch_input.right_slider = (int8_t)message.data[1];
                touch_input.left_button = (uint8_t)message.data[2];
                touch_input.right_button = (uint8_t)message.data[3];
                break;
            case CAN_GAME_START_ID:
                game_start_flag = 1;
                printf("start\n\r");
                break;
            default:
                break;
        }
    }

    if (can_sr & CAN_SR_MB0) {
        // Disable interrupt
        CAN0->CAN_IDR = CAN_IER_MB0;
    }

    if (can_sr & CAN_SR_ERRP) {
        printf("CAN0 ERRP error\n\r");
    }
    if (can_sr & CAN_SR_TOVF) {
        printf("CAN0 timer overflow\n\r");
    }

    NVIC_ClearPendingIRQ(ID_CAN0);
    // sei();*/
}
