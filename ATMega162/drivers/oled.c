#include "oled.h"

#include "../../Misc/fonts.h"
#include "external_memory.h"

#define COLUMN_SIZE 128
#define row_SIZE 8

static void oled_write_command(uint8_t command) {
    external_memory_write(command, OLED_COMMAND_OFFSET);
}

static void oled_write_data(uint8_t command) {
    external_memory_write(command, OLED_DATA_OFFSET);
}

static void oled_goto_row(uint8_t row_index) {
    oled_write_command(0xB0 + row_index);
}

static void oled_goto_column(uint8_t column) {
    oled_write_command(0x00 | (column & 0xF));
    oled_write_command(0x10 | ((column & 0xF0) >> 4));
}

static void oled_print_char(char ch, FontSize size) {
    switch (size) {
        case SMALL: {
            for (uint8_t i = 0; i < SMALL_FONT_SIZE; i++) {
                uint8_t byte = pgm_read_byte(&(font4[ch - ASCII_OFFSET][i]));
                oled_write_data(byte);
            }
            break;
        }
        case NORMAL: {
            for (uint8_t i = 0; i < NORMAL_FONT_SIZE; i++) {
                uint8_t byte = pgm_read_byte(&(font5[ch - ASCII_OFFSET][i]));
                oled_write_data(byte);
            }
            break;
        }
        case LARGE: {
            for (uint8_t i = 0; i < LARGE_FONT_SIZE; i++) {
                uint8_t byte = pgm_read_byte(&(font8[ch - ASCII_OFFSET][i]));
                oled_write_data(byte);
            }
            break;
        }
    }
}

void oled_init() {
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
    for (int i = 0; i < row_SIZE; i++) {
        oled_clear_row(i);
    }
}

void oled_clear_row(uint8_t row_index) {
    oled_goto_row(row_index);
    for (int i = 0; i < COLUMN_SIZE; i++) {
        oled_write_data(0x0);
    }
}

void oled_origin() { oled_pos(0, 0); }

void oled_pos(uint8_t row, uint8_t column) {
    oled_goto_row(row);
    oled_goto_column(column);
}

void oled_print(char* string, FontSize font_size) {
    for (uint8_t i = 0; string[i] != '\0'; i++) {
        oled_print_char(string[i], font_size);
    }
}

void oled_print_arrow(uint8_t row, uint8_t column) {
    oled_pos(row, column);
    oled_write_data(0b00011000);
    oled_write_data(0b00011000);
    oled_write_data(0b01111110);
    oled_write_data(0b00111100);
    oled_write_data(0b00011000);
}