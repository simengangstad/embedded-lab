#include "can_controller.h"

#include <avr/interrupt.h>
#include <stdlib.h>

#include "mcp2515.h"

#define MCP_INTERRUPT_PIN PD3

ISR(INT1_vect) {
    if (mcp2515_read_status() & MCP_RX0IF) {
        Message* message_ptr = can_controller_read();

        printf("%s\r\n", message_ptr->data);

        free(message_ptr);
    }
}

void can_controller_init() {
    mcp2515_init();

    // Set loopback mode
    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);

    // Set up interrupt on receive
    mcp2515_bit_modify(MCP_CANINTE, MCP_RX0IE, MCP_RX0IE);

    // Disable global interrupts
    cli();

    //// Interrupt on falling edge of INT1
    MCUCR |= (1 << ISC11);

    //// Enable interrupt on INT1
    GICR |= 1 << INT1;

    sei();
}

void can_controller_transmit(Message* message_ptr) {
    mcp2515_bit_modify(MCP_TXB0CTRL, MCP_TXB_PRIORITY_MASK, MCP_TXB_PRIORITY_HIGHEST);

    // Only use standard mode for identifier
    // Grab the upper 8 bits
    mcp2515_write(MCP_TXB0SIDH, (uint8_t)(message_ptr->id >> 3));
    // Grab the lower 3 bits
    mcp2515_write(MCP_TXB0SIDL, (uint8_t)((message_ptr->id & 0x7) << 5));

    // Set the lower four bits in the DLC register to the length of the message
    mcp2515_write(MCP_TXB0DLC, message_ptr->length & 0x0F);

    // Write the data
    mcp2515_write_array(MCP_TXB0D0, message_ptr->data, message_ptr->length);

    // Request to send
    mcp2515_rts();
}

Message* can_controller_read() {
    Message* message_ptr = (Message*)malloc(sizeof(Message));

    uint16_t id_high_byte = (uint16_t)mcp2515_read(MCP_RXB0SIDH);
    uint16_t id_low_byte = (uint16_t)mcp2515_read(MCP_RXB0SIDL);

    message_ptr->id = (id_high_byte << 3) | (id_low_byte >> 5);

    // Only grab the lower 4 bits, which specify the length of the message
    message_ptr->length = mcp2515_read(MCP_RXB0DLC) & 0x0F;

    mcp2515_read_array(MCP_RXB0D0, 8, message_ptr->data);

    mcp2515_bit_modify(MCP_CANINTE, MCP_RX0IF, 0x0);

    return message_ptr;
}