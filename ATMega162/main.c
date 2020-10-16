#define F_CPU 4915200

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "drivers/adc.h"
#include "drivers/can_controller.h"
#include "drivers/external_memory.h"
#include "drivers/mcp2515.h"
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
    can_controller_init();
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

    Message joystick_position_message = {1, 3, {0, 0, 0}};

    int count = 0;

    while (1) {
        if (gui_display_update_flag()) {
            gui_handle_input();
            gui_display();
        }

        JoystickPosition position = input_joystick_position();
        JoystickDirection direction = input_joystick_direction();
        joystick_position_message.data[0] = position.x;
        joystick_position_message.data[1] = position.y;
        joystick_position_message.data[2] = direction;

        // printf("%d", mcp2515_read_status() & 0x2);
        can_controller_transmit(&joystick_position_message);
    }

    return 0;
}
