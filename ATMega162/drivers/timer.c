#include "timer.h"

#include <avr/io.h>

void timer_init() {
    // Set mode to CTC, clear timer on compare (comparing timer counter, TCNT0, with compare register, OCR0).
    TCCR1B |= (1 << WGM12) | (0 << CS11);

    // Use clock source without prescaling
    TCCR0 |= (0 << CS02) | (0 << CS01) | (1 << CS00);

    // The compare value, will set the frequency to ~1.2 MHz according to this formula:
    // frequency = clock_frequency/(2 * prescale_factor * (1 + OCR0)). Where clock_frequency
    // is 4.9152 MHz and prescale factor is 1.
    OCR0 = 1;
}