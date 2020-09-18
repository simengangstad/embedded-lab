#ifndef OLED_H
#define OLED_H

#include <stdio.h>

typedef enum { SMALL, NORMAL, LARGE } FontSize;

void oled_init();

void oled_set_brightness(uint8_t brightness);

void oled_clear();

void oled_clear_row(uint8_t row_index);

void oled_origin();

void oled_pos(uint8_t row, uint8_t column);

void oled_print(char* string, FontSize font_size);

void oled_print_arrow(uint8_t row, uint8_t column);

#endif