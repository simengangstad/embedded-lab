/**
 * @file game.c
 */

#include "game.h"

#include <stdint.h>

#include "drivers/actuator/servo.h"
#include "drivers/actuator/solenoid.h"
#include "drivers/adc.h"
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
        // printf("Duty cycle: %d\n\r", duty_cycle);
        servo_set_duty_cycle(percent);
    }
}

void game_init(void) {
    servo_init();
    solenoid_init();
    adc_init();
    motor_controller_init();
}

void game_loop(void) {
    uint8_t goals_conceded = 0;

    // Used to see if the state have changed. 1 if blocked, 0 else.
    uint32_t last_ir_blocked_state = 0;
    uint32_t ir_blocked_state = 0;

    Joystick joystick;
    TouchInput touch_input;

    motor_controller_start();

    do {
        player_input_get_joystick(&joystick);
        player_input_get_touch_input(&touch_input);

        game_set_servo_position(touch_input.right_slider);
        motor_controller_set_reference(joystick.x);
        solenoid_activate(joystick.button_pressed);
        // solenoid_activate(touch_input.right_button);

        ir_blocked_state = adc_ir_beam_blocked();
        // Only count goals when the state changes
        if (last_ir_blocked_state != ir_blocked_state) {
            if (ir_blocked_state) {
                goals_conceded++;
                printf("SCORE!\n\r");
            }
            last_ir_blocked_state = ir_blocked_state;
        }
    } while (!touch_input.left_button);

    motor_controller_stop();
    printf("Score: %d\r\n", -goals_conceded);
}
