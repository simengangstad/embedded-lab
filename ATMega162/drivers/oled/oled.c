/**
 * @file oled.c
 */

#include "oled.h"

#include "../memory/external_memory.h"
#include "fonts.h"

/**
 * @brief Current screen position in buffer in SRAM.
 */
static uint8_t oled_row = 0, oled_column = 0;

/**
 * @brief Writes a single command to the OLED's internal registers.
 */
static void oled_write_command(uint8_t command) { external_memory_write(command, OLED_COMMAND_OFFSET); }

/**
 * @brief Writes data to the OLED's internal memory.
 *
 * @param style Whether the written data should be inverted or not.
 */
static void oled_write_data(uint8_t data, Style style) {
    external_memory_write(data ^ style, SRAM_OFFSET + oled_row * 128 + oled_column);
    oled_column = (oled_column + 1) % 128;
}

/**
 * @brief Moves to @p row.
 */
static void oled_goto_row(uint8_t row) { oled_row = row; }

/**
 * @brief Moves to @p column.
 */
static void oled_goto_column(uint8_t column) { oled_column = column; }

/**
 * @brief Writes a single charater to the OLED.
 */
static void oled_print_char(char ch, FontSize size, Style style) {
    switch (size) {
        case SMALL: {
            for (uint8_t i = 0; i < SMALL_FONT_SIZE; i++) {
                uint8_t byte = pgm_read_byte(&(font4[ch - ASCII_OFFSET][i]));
                oled_write_data(byte, style);
            }
            break;
        }
        case NORMAL: {
            for (uint8_t i = 0; i < NORMAL_FONT_SIZE; i++) {
                uint8_t byte = pgm_read_byte(&(font5[ch - ASCII_OFFSET][i]));
                oled_write_data(byte, style);
            }
            break;
        }
        case LARGE: {
            for (uint8_t i = 0; i < LARGE_FONT_SIZE; i++) {
                uint8_t byte = pgm_read_byte(&(font8[ch - ASCII_OFFSET][i]));
                oled_write_data(byte, style);
            }
            break;
        }
    }
}

void oled_init(void) {
    oled_write_command(0xae);  // display off
    oled_write_command(0xa1);  // segment remap
    oled_write_command(0xda);  // common pads hardware: alternative
    oled_write_command(0x12);
    oled_write_command(0xc8);  // common output scan direction:com63~com0
    oled_write_command(0xa8);  // multiplex ration mode:63
    oled_write_command(0x3f);
    oled_write_command(0xd5);  // display divide ratio/osc. freq. mode
    oled_write_command(0x80);
    oled_write_command(0x81);  // contrast control
    oled_write_command(0x50);
    oled_write_command(0xd9);  // set pre-charge period
    oled_write_command(0x21);
    oled_write_command(0x20);  // set Memory Addressing Mode: row adressing
    oled_write_command(0x02);
    oled_write_command(0xdb);  // VCOM deselect level mode
    oled_write_command(0x30);
    oled_write_command(0xad);  // master configuration
    oled_write_command(0x00);
    oled_write_command(0xa4);  // out follows RAM content
    oled_write_command(0xa6);  // set normal display
    oled_write_command(0xaf);  // display on
    oled_clear();
}

void oled_set_brightness(uint8_t brightness) {
    oled_write_command(0x81);
    oled_write_command(brightness);
}

void oled_clear() {
    for (uint8_t row = 0; row < ROW_SIZE; row++) {
        for (uint8_t column = 0; column < COLUMN_SIZE; column++) {
            external_memory_write(0x0, SRAM_OFFSET + row * 128 + column);
        }
    }

    oled_pos(0, 0);
}

void oled_clear_row(uint8_t row_index) {
    for (int i = 0; i < COLUMN_SIZE; i++) {
        external_memory_write(0x0, SRAM_OFFSET + row_index * 128 + i);
    }
}

void oled_origin() { oled_pos(0, 0); }

void oled_pos(uint8_t row, uint8_t column) {
    oled_goto_row(row);
    oled_goto_column(column);
}

void oled_print(char* string, FontSize font_size, Style style) {
    for (uint8_t i = 0; string[i] != '\0'; i++) {
        oled_print_char(string[i], font_size, style);
    }
}

void oled_print_right_arrow(Style style) {
    oled_write_data(0b00000000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b01111110, style);
    oled_write_data(0b01111110, style);
    oled_write_data(0b00111100, style);
    oled_write_data(0b00111100, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00000000, style);
}

void oled_print_left_arrow(Style style) {
    oled_write_data(0b00000000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00111100, style);
    oled_write_data(0b00111100, style);
    oled_write_data(0b01111110, style);
    oled_write_data(0b01111110, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00011000, style);
    oled_write_data(0b00000000, style);
}

void oled_print_circle(Style style) {
    oled_write_data(0b00011100, style);
    oled_write_data(0b00111110, style);
    oled_write_data(0b01111111, style);
    oled_write_data(0b01111111, style);
    oled_write_data(0b01111111, style);
    oled_write_data(0b00111110, style);
    oled_write_data(0b00011100, style);
    oled_write_data(0b00000000, style);
}

void oled_update() {
    // Set column to 0
    oled_write_command(0);
    oled_write_command(0x10);
    for (uint8_t row = 0; row < ROW_SIZE; row++) {
        // Choose row. Column will be set to 0 automatically since OLED keeps an internal
        // pointer
        oled_write_command(0xB0 + row);

        for (uint8_t column = 0; column < COLUMN_SIZE; column++) {
            const uint8_t data = external_memory_read(SRAM_OFFSET + row * 128 + column);
            external_memory_write(data, OLED_DATA_OFFSET);
        }
    }
}
