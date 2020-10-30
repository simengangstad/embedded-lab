
#include "mcp2515.h"

#include "spi.h"

uint8_t mcp2515_init() {
    spi_init();
    mcp2515_reset();

    uint8_t value = mcp2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_CONFIG) {
        printf("MCP2515 is NOT in configuration mode after reset!\r\n");
        return 1;
    }

    return 0;
}

void mcp2515_reset(void) {
    spi_slave_select();
    spi_write(MCP_RESET);
    spi_slave_deselect();
}

uint8_t mcp2515_read(uint8_t address) {
    spi_slave_select();

    spi_write(MCP_READ);
    spi_write(address);
    uint8_t result = spi_read();

    spi_slave_deselect();

    return result;
}

void mcp2515_read_array(uint8_t address, uint8_t length, uint8_t* result) {
    spi_slave_select();

    spi_write(MCP_READ);
    spi_write(address);

    for (int i = 0; i < length; i++) {
        result[i] = spi_read();
    }

    spi_slave_deselect();
}

void mcp2515_write(uint8_t address, uint8_t data) {
    spi_slave_select();
    spi_write(MCP_WRITE);
    spi_write(address);
    spi_write(data);
    spi_slave_deselect();
}

void mcp2515_write_array(uint8_t address, uint8_t* data, uint8_t length) {
    spi_slave_select();
    spi_write(MCP_WRITE);

    spi_write(address);

    // Address will be incremented in the MCP2515 automatically when slave select is held low,
    // so we don't have to explicitly increment the address
    for (uint8_t i = 0; i < length; i++) {
        spi_write(data[i]);
    }

    spi_slave_deselect();
}

void mcp2515_rts() {
    spi_slave_select();
    spi_write(MCP_RTS_TX0);
    spi_slave_deselect();
}

uint8_t mcp2515_read_status() {
    spi_slave_select();
    spi_write(MCP_READ_STATUS);
    uint8_t status = spi_read();
    spi_slave_deselect();
    return status;
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    spi_slave_select();
    spi_write(MCP_BITMOD);
    spi_write(address);
    spi_write(mask);
    spi_write(data);
    spi_slave_deselect();
}