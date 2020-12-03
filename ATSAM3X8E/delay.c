
#include "delay.h"

void delay_us(volatile uint32_t t) {
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