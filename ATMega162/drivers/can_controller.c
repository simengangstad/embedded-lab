
#include "mcp2515.h"

void can_controller_init() {
    mcp2515_init();
    // Set loopback mode:
    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);
}

void can_controller_transmit(uint16_t id, uint8_t* data, uint8_t length) {
    mcp2515_bit_modify(MCP_TXB0CTRL, 0x03, 0x03);
    // mcp2515_bit_modify(MCP_SID)
    // mcp2515_bit_modify(MPC)
}