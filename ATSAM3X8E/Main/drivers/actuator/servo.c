/**
 * @file servo.c
 */

#include "../uart_and_printf/printf-stdarg.h"
#include "pwm.h"
#include "stdio.h"

void servo_send_duty_cycle(int8_t slider_position) {
    float duty_cycle = 1 + slider_position / 100.0f;
    float percent = 5 * duty_cycle;

    if (duty_cycle < 1 || duty_cycle > 2) {
        printf("Duty_cycle out_of_range\n\r");
    } else {
        printf("Duty cycle: %d\n\r", duty_cycle);
        // pwm_set_duty_cycle(percent);
    }
}