/**
 * @file game.c
 */

#include "game.h"

#include <stdint.h>

#include "can_identifiers.h"
#include "delay.h"
#include "drivers/actuator/servo.h"
#include "drivers/actuator/solenoid.h"
#include "drivers/adc.h"
#include "drivers/can_controller.h"
#include "drivers/uart_and_printf/printf-stdarg.h"
#include "motor_controller.h"
#include "player_input.h"
#include "sam.h"

/**
 * @brief Sends a duty cycle to the servo by transforming the @p slider_position.
 */
static void game_set_servo_position(int8_t slider_position) {
    float duty_cycle = 1 + slider_position / 100.0f;
    float percent = 5 * duty_cycle;

    if (duty_cycle < 1 || duty_cycle > 2) {
        printf("Duty_cycle out_of_range\n\r");
    } else {
        servo_set_duty_cycle(percent);
    }
}

void game_init(void) {
    servo_init();
    solenoid_init();
    adc_init();
    motor_controller_init();
    player_input_init();
}

void game_loop(void) {
    uint8_t goals_conceded = 0;

    // Used to see if the state have changed. 1 if blocked, 0 else.
    uint32_t last_ir_blocked_state = 0;
    uint32_t ir_blocked_state = 0;

    // Used to only reset on change of state. Necessary to avoid the score messeage from spamming the CAN bus
    uint8_t last_right_button = 0;

    // User inputs
    Joystick joystick;
    TouchInput touch_input;

    // A score message to Atmega162 for it to print it
    CAN_MESSAGE score_message = {CAN_SCORE_MESSAGE_ID, 1, {goals_conceded}};

    motor_controller_start();

    do {
        player_input_get_joystick(&joystick);
        printf("Joystick: %d\r\n", joystick.x);
        player_input_get_touch_input(&touch_input);

        game_set_servo_position(touch_input.right_slider);
        motor_controller_set_reference(joystick.x);
        solenoid_activate(joystick.button_pressed);

        ir_blocked_state = adc_ir_beam_blocked();
        // Only count goals when the state changes
        if (last_ir_blocked_state != ir_blocked_state) {
            if (ir_blocked_state) {
                goals_conceded++;
                score_message.data[0] = goals_conceded;
                can_send(&score_message, 0);
            }
            last_ir_blocked_state = ir_blocked_state;
        }

        // Delay to avoid counting multiple goals because of an flickering IR beam
        // delay_us(1000);

        // Right button resets the score
        /*if (touch_input.right_button != last_right_button) {
            if (touch_input.right_button) {
                goals_conceded = 0;
                score_message.data[0] = goals_conceded;
                can_send(&score_message, 0);
            }
            last_right_button = touch_input.right_button;
        }*/

    } while (!touch_input.left_button);  // Left button quits the game

    motor_controller_stop();

    // Reset the input such that we don't have left_button = 1 next time we want to play
    // player_input_reset();
}
