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

    while (1) {
        if (gui_display_update_flag()) {
            gui_handle_input();
            gui_display_menu();
        }
    }
    return 0;
}
