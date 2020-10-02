#define F_CPU 4915200

#include <stdio.h>
#include <util/delay.h>

#include "drivers/adc.h"
#include "drivers/can_controller.h"
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

    Message message = {123, 4, "Hei"};
    can_controller_transmit(&message);

    // Message* recived = can_controller_read();
    // printf("%s", recived->data);

    while (1) {
        // gui_handle_input();
        // gui_display();
    }

    return 0;
}
