#define F_CPU 4915200

#include <stdio.h>
#include <util/delay.h>

#include "drivers/adc.h"
#include "drivers/external_memory.h"
#include "drivers/oled.h"
#include "drivers/uart.h"
#include "user_interface/gui.h"
#include "user_interface/input.h"

void initialize_atmega() {
    uart_init(9600);
    external_memory_init();
    fdevopen(&uart_transmit, &uart_receive);
    adc_init();
    input_init();
    oled_init();
    external_memory_sram_test();
    gui_init();
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
    while (1) {
        gui_handle_input();
        gui_display();
    }
    // struct timespec

    //     while (1) {
    //     if (clock_gettime(CLOCK_REALTIME, &stop) == -1) {
    //         perror("clock gettime");
    //         return EXIT_FAILURE;
    //     }

    //     counter += (stop.tv_sec - start.tv_sec) * 1000000 + (unsigned long)(stop.tv_nsec - start.tv_nsec) / 1000.0;

    //     if (counter > 16667) {
    //         gui_handle_input();
    //         gui_display();

    //         counter = 0;
    //     }
    // }
    return 0;
}
