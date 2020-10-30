/**
 * @file can_controller.c
 */

#include "can_controller.h"

#include <avr/interrupt.h>
#include <stdlib.h>

#include "mcp2515.h"

#define MCP_INTERRUPT_PIN PD3

ISR(INT1_vect) {
    const uint8_t status = mcp2515_read_status();
    Message* message_ptr;

    if (status & MCP_RX0IF) {
        message_ptr = (Message*)malloc(sizeof(Message));
        can_controller_read(0, message_ptr);
    } else if (status & MCP_RX1IF) {
        message_ptr = (Message*)malloc(sizeof(Message));
        can_controller_read(1, message_ptr);
    }

    if (message_ptr) {
        printf("%s\r\n", message_ptr->data);

        free(message_ptr);
    }
}

void can_controller_init() {
    mcp2515_init();

    // Set BRP to 3 for at bit rate of 125 kbit/s. The formula is given by:
    // BRP = T_q*F_osc/2 - 1 where T_q is set to 1/16*125 kbit/s and F_osc is 16 MHz
    mcp2515_write(MCP_CNF1, 0x03);
    mcp2515_write(MCP_CNF2, 0b10110001);
    mcp2515_write(MCP_CNF3, 0b00000101);

    // Set normal mode
    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);

    // Set up interrupt on receive
    mcp2515_bit_modify(MCP_CANINTE, MCP_RX0IE | MCP_RX1IE, MCP_RX0IE | MCP_RX1IE);

    // Disable global interrupts
    cli();

    // Interrupt on falling edge of INT1
    MCUCR |= (1 << ISC11);

    // Enable interrupt on INT1
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

void can_controller_read(uint8_t buffer_id, Message* message_ptr) {
    uint8_t id_low_byte_address, id_high_byte_address, data_length_address, data_start_address, interrupt_flag_address;

    if (buffer_id == 0) {
        id_low_byte_address = MCP_RXB0SIDL;
        id_high_byte_address = MCP_RXB0SIDH;
        data_length_address = MCP_RXB0DLC;
        data_start_address = MCP_RXB0D0;
        interrupt_flag_address = MCP_RX0IF;

    } else if (buffer_id == 1) {
        id_low_byte_address = MCP_RXB1SIDL;
        id_high_byte_address = MCP_RXB1SIDH;
        data_length_address = MCP_RXB1DLC;
        data_start_address = MCP_RXB1D0;
        interrupt_flag_address = MCP_RX1IF;
    }

    const uint16_t id_high_byte = (uint16_t)mcp2515_read(id_high_byte_address);
    const uint16_t id_low_byte = (uint16_t)mcp2515_read(id_low_byte_address);

    message_ptr->id = (id_high_byte << 3) | (id_low_byte >> 5);

    // Only grab the lower 4 bits, which specify the length of the message
    message_ptr->length = mcp2515_read(data_length_address) & 0x0F;

    mcp2515_read_array(data_start_address, 8, message_ptr->data);

    mcp2515_bit_modify(MCP_CANINTF, interrupt_flag_address, 0x0);
}