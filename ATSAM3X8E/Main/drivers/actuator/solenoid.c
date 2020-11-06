
#include "sam.h"

void solenoid_init(void) {
    // Set up pin for solenoid (A4 on shield)
    PMC->PMC_PCER0 = PMC_PCER0_PID11;
    PIOA->PIO_PER = PIO_PA6;
    PIOA->PIO_OER = PIO_PA6;
    PIOA->PIO_SODR = PIO_SODR_P6;
}

void solenoid_activate(uint8_t active) {
    if (active) {
        PIOA->PIO_CODR = PIO_SODR_P6;
    } else {
        PIOA->PIO_SODR = PIO_SODR_P6;
    }
}
