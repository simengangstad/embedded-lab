#define F_CPU 4915200

#include <stdio.h>
#include <util/delay.h>

#include "drivers/adc.h"
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
    mcp2515_init();
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

    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);

    printf("Mode: %i\r\n", mcp2515_read(MCP_CANSTAT) & MODE_MASK);

    uint8_t data[1] = {0xE0};
    mcp2515_write(MCP_TXB0SIDH, &data, 1);
    mcp2515_rts();

    uint8_t value = mcp2515_read(MCP_RXB0SIDH);
    printf("Received: %i\r\n", value);

    while (1) {
        // gui_handle_input();
        // gui_display();
    }

    return 0;
}
