/*
 * motor_controller.c
 *
 * Created: 23.10.2020 16:19:46
 *  Author: gotvinne
 */

#include <stdlib.h>

#include "..\can\can_joystick.h"
#include "sam.h"

#define SCALING_OPENLOOP 655

typedef enum { DRIVE_LEFT, DRIVE_RIGHT } MotorDirection;

void motor_init(void) {
    // Initialising clk for PIOC, PIOD and DAC, ID 1-31 equals channel 0
    PMC->PMC_PCER0 |= PMC_PCER0_PID13;
    PMC->PMC_PCER0 |= PMC_PCER0_PID14;
    PMC->PMC_PCER1 |= PMC_PCER1_PID38;

    // Enable pins, DIR =PD10, EN = PD9, SEL = PD2, !OE = PD0, !RESET = PD1
    PIOD->PIO_PER |= PIO_PER_P9;
    PIOD->PIO_PER |= PIO_PER_P10;
    PIOD->PIO_PER |= PIO_PER_P2;
    PIOD->PIO_PER |= PIO_PER_P0;
    PIOD->PIO_PER |= PIO_PER_P1;

    // set to output
    PIOD->PIO_OER |= PIO_OER_P9;
    PIOD->PIO_OER |= PIO_OER_P10;
    PIOD->PIO_OER |= PIO_OER_P20;
    PIOD->PIO_OER |= PIO_OER_P0;
    PIOD->PIO_OER |= PIO_OER_P1;

    // Set EN high
    PIOD->PIO_SODR |= PIO_SODR_P19;
    PIOD->PIO_SODR |= PIO_SODR_P2;

    // Set !OE and !RESET low
    PIOD->PIO_CODR |= PIO_CODR_P0;
    PIOD->PIO_CODR |= PIO_CODR_P1;

    // initiazise DACC

    // Set mode register: Use channel 1 and half word transfer (16 bit)
    DACC->DACC_MR |= DACC_MR_USER_SEL_CHANNEL1 | DACC_MR_WORD_HALF;

    // Initializing DAC1
    DACC->DACC_CHER |= DACC_CHER_CH1;
}

static void motor_set_actuation(uint16_t speed) { DACC->DACC_CDR = speed; }

static void motor_set_direction(MotorDirection dir) {
    if (dir == DRIVE_LEFT) {
        PIOD->PIO_SODR |= PIO_SODR_P10;
    } else if (dir == DRIVE_RIGHT) {
        PIOD->PIO_CODR |= PIO_CODR_P10;
    }
}

void motor_open_loop_speed_control(Joystick* joystick) {
    if (joystick->dir == JS_RIGHT) {
        motor_set_direction(DRIVE_RIGHT);
    } else if (joystick->dir == JS_LEFT) {
        motor_set_direction(DRIVE_LEFT);
    }

    uint16_t absolute_x = (uint16_t)abs(joystick->x);
    uint16_t speed = absolute_x * SCALING_OPENLOOP;

    motor_set_actuation(speed);

    printf("speed: %d\n\r", speed);

}
