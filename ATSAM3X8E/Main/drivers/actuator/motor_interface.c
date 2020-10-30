/**
 * @file motor_interface.c
 */

#include "motor_interface.h"

#include <stdlib.h>

#include "..\can\can_joystick.h"
#include "sam.h"

/**
 * @brief Delays by a given amount of micro seconds.
 */
static delay_us(volatile uint32_t t) {
    // cycles = t us * 84 MHz = 84*t / 21
    // Divided by 21 (which was found empirical), which is due to extra
    // cycles in the for loop

    // Using volatile around here just to make sure the compiler doesn't play
    // any tricks on us

    const volatile uint32_t cycles = 84 * t / 21;

    for (volatile uint32_t i = 0; i < cycles; i++) {
        asm volatile("nop");
    }
}

void motor_interface_init(void) {
    // Set up PIO
    // Enable PMC for PIOC and PIOD
    PMC->PMC_PCER0 |= PMC_PCER0_PID13;
    PMC->PMC_PCER0 |= PMC_PCER0_PID14;

    // Enable pins EN = PD9, DIR = PD10, SEL = PD2, !OE = PD0, !RESET = PD1, in that order
    PIOD->PIO_PER |= PIO_PER_P9;
    PIOD->PIO_PER |= PIO_PER_P10;
    PIOD->PIO_PER |= PIO_PER_P2;
    PIOD->PIO_PER |= PIO_PER_P0;
    PIOD->PIO_PER |= PIO_PER_P1;

    // Set the same pins to output, in the same order
    PIOD->PIO_OER |= PIO_OER_P9;
    PIOD->PIO_OER |= PIO_OER_P10;
    PIOD->PIO_OER |= PIO_OER_P2;
    PIOD->PIO_OER |= PIO_OER_P0;
    PIOD->PIO_OER |= PIO_OER_P1;

    // Set EN pin
    PIOD->PIO_SODR |= PIO_SODR_P9;

    // Toggle !RST pin
    PIOD->PIO_CODR |= PIO_CODR_P1;
    delay_us(20);  // Was to fast before, didn't reset
    PIOD->PIO_SODR |= PIO_SODR_P1;

    // Clear OE!
    PIOD->PIO_CODR |= PIO_CODR_P0;

    // Set up DACC
    // Enable PMC for DACC
    PMC->PMC_PCER1 |= PMC_PCER1_PID38;

    // Set mode register: Use channel 1 and half word transfer (16 bit)
    DACC->DACC_MR |= DACC_MR_USER_SEL_CHANNEL1 | DACC_MR_WORD_HALF;

    // Initializing DAC1
    DACC->DACC_CHER |= DACC_CHER_CH1;
}

void motor_interface_set_actuation(uint16_t speed) { DACC->DACC_CDR = speed; }

void motor_interface_set_direction(MotorDirection dir) {
    if (dir == DRIVE_LEFT) {
        // Clear DIR pin
        PIOD->PIO_CODR |= PIO_SODR_P10;
    } else if (dir == DRIVE_RIGHT) {
        // Set DIR pin
        PIOD->PIO_SODR |= PIO_CODR_P10;
    }
}

int16_t motor_interface_read_position() {
    // Encoder pins is on PC1 to PC8, so have to right shift one before masking
    int16_t out = 0;

    // Read low byte
    PIOD->PIO_SODR |= PIO_SODR_P2;
    delay_us(20);
    out |= (PIOC->PIO_PDSR >> 1) & 0x00FF;

    // Read high byte
    PIOD->PIO_CODR |= PIO_CODR_P2;
    delay_us(20);
    out |= ((PIOC->PIO_PDSR >> 1) & 0x00FF) << 8;

    return -out;  // temporary fix to make right side positive
}
