#ifndef OLED_H
#define OLED_H

#include <stdio.h>

#define COLUMN_SIZE 128
#define ROW_SIZE 8

typedef enum { SMALL, NORMAL, LARGE } FontSize;

typedef enum { NON_INVERTED = 0x00, INVERTED = 0xFF } Style;

void oled_init();

void oled_set_brightness(uint8_t brightness);

void oled_clear();

void oled_clear_row(uint8_t row_index);

void oled_origin();

void oled_pos(uint8_t row, uint8_t column);

void oled_print(char* string, FontSize font_size, Style style);

void oled_print_right_arrow(Style style);

void oled_print_left_arrow(Style style);

void oled_print_circle(Style style);

void oled_update();

#endif