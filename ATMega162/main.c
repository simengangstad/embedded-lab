#define F_CPU 4915200

#include <stdio.h>
#include <util/delay.h>

#include "../Misc/sram_test.c"
#include "drivers/adc.h"
#include "drivers/external_memory.h"
#include "drivers/oled.h"
#include "drivers/uart.h"
#include "user_interface/input.h"

void initialize_atmega() {
    uart_init(9600);
    external_memory_init();
    fdevopen(&uart_transmit, &uart_receive);
    adc_init();
    input_init();
    oled_init();
    SRAM_test();
}

char* get_string_from_joystick_direction(JoystickDirection joystick_direction) {
    switch (joystick_direction) {
        case UP:
            return "up";
        case DOWN:
            return "down";
        case LEFT:
            return "left";
        case RIGHT:
            return "right";
        default:
            return "neutral";
    }
}

int main(void) {
    initialize_atmega();

    input_test();

    return 0;
}
