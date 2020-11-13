/**
 * @file oled.h
 * @brief Driver for interfacing the with OLED on the multifunction board.
 *        The driver uses double buffering, where the display data is stored
 *        in SRAM before it is flushed to the OLED.
 */

#ifndef OLED_H
#define OLED_H

#include <stdio.h>

#define COLUMN_SIZE 128
#define ROW_SIZE 8

typedef enum { SMALL, NORMAL, LARGE } FontSize;

typedef enum { NON_INVERTED = 0x00, INVERTED = 0xFF } Style;

/**
 * @brief Sets up the configuartion for the OLED and enables column row mode.
 */
void oled_init(void);

/**
 * @brief Sets contrast. @p brightness of 255 will result in full contrast.
 */
void oled_set_brightness(uint8_t brightness);

/**
 * @brief Clears the display.
 */
void oled_clear(void);

/**
 * @brief Clears a specific @p row_index.
 */
void oled_clear_row(uint8_t row_index);

/**
 * @brief Sets the position where the next write should be to (0, 0).
 */
void oled_origin();

/**
 * @brief Sets the position where the next write should be, defined by
 *        a @p row and @p column.
 */
void oled_pos(uint8_t row, uint8_t column);

/**
 * @brief Writes a @p string to the OLED.
 *
 * @param style Whether the string should be written in inverted or normal style.
 */
void oled_print(char* string, FontSize font_size, Style style);

/**
 * @brief Writes a right arrow to the OLED.
 *
 * @param style Whether the arrow should be inverted or not.
 */
void oled_print_right_arrow(Style style);

/**
 * @brief Writes a left arrow to the OLED.
 *
 * @param style Whether the arrow should be inverted or not.
 */
void oled_print_left_arrow(Style style);

/**
 * @brief Writes a circle to the OLED.
 *
 * @param style Whether the circle should be inverted or not.
 */
void oled_print_circle(Style style);

/**
 * @brief Flushes the buffer in SRAM to the OLED. This function has to be called
 *        after writes for the OLED to update.
 */
void oled_update();

#endif