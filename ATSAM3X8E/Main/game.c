/**
 * @file game.c
 */

#include "game.h"

#include <stdint.h>

#include "drivers/adc.h"
#include "drivers/uart_and_printf/printf-stdarg.h"
#include "sam.h"

// The goals we have let in
static uint32_t goals_conceded = 0;

static uint32_t last_ir_blocked_state = 0;

void game_init(void) {
    // Set up pin for solenoid (A4 on shield)
    PMC->PMC_PCER0 = PMC_PCER0_PID11;
    PIOA->PIO_PER = PIO_PA6;
    PIOA->PIO_OER = PIO_PA6;
}

void game_update(Joystick* joystick_ptr, TouchInput* touch_input_ptr) {
    uint32_t ir_blocked_state = adc_ir_beam_blocked();

    if (last_ir_blocked_state != ir_blocked_state) {
        if (ir_blocked_state) {
            goals_conceded++;
        }
        last_ir_blocked_state = ir_blocked_state;
    }

    servo_send_duty_cycle(touch_input_ptr->right_slider);
    motor_controller_set_reference(&joystick_ptr);

    // Trigger solenoid
    if (joystick_ptr->button_pressed) {
        PIOA->PIO_SODR = PIO_SODR_P6;
    } else {
        PIOA->PIO_CODR = PIO_SODR_P6;
    }

    // printf("Score: %d\r\n", goals_conceded);
}