/*
 * servo.c
 *
 * Created: 16.10.2020 15:53:57
 *  Author: gotvinne
 */
#include "pwm.h"
#include "../uart_and_printf/printf-stdarg.h"
#include "stdio.h" 

void servo_send_duty_cycle(int8_t joystick_position) {
    // assumes joystick x-position = [-100,100]
    float duty_cycle = 1 + (joystick_position + 100) / 200.0f;
    float percent = 5 * duty_cycle;

    if (duty_cycle < 1 || duty_cycle > 2) {
        printf("Duty_cycle out_of_range\n\r");
    }

    else {
        pwm_set_duty_cycle(percent);
    }
	
    printf("Pulse width percent: %d\n\r", (int) percent); 
}