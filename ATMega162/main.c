
#include <stdio.h>

#include "can_identifiers.h"
#include "drivers/can/can_controller.h"
#include "drivers/memory/external_memory.h"
#include "drivers/uart.h"
#include "user_interface/gui.h"
#include "user_interface/input.h"

void initialize_atmega() {
    uart_init(9600);
    external_memory_init();
    fdevopen(&uart_transmit, &uart_receive);
    gui_init();
    can_controller_init();
    external_memory_sram_test();
}

int main(void) {
    initialize_atmega();

    Message touch_message = {CAN_TOUCH_MESSAGE_ID, 4, {0, 0, 0, 0}};

    while (1) {
        if (gui_display_update_flag()) {
            gui_handle_input();
            gui_display_menu();
            TouchInput touch_input = input_touch();
            touch_message.data[0] = touch_input.left_slider;
            touch_message.data[1] = touch_input.right_slider;
            touch_message.data[2] = touch_input.left_button;
            touch_message.data[3] = touch_input.right_button;
            can_controller_transmit(&touch_message);
        }
    }
    return 0;
}
