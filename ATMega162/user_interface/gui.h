/**
 * @file gui.h
 * @brief Graphical user interface on the OLED consisting of a menu system.
 */

#ifndef GUI_H
#define GUI_H

#include <stdio.h>

#include "../drivers/oled/oled.h"

#define FONT_SIZE LARGE

/**
 * @brief Initializes the menu system.
 */
void gui_init(void);

/**
 * @brief Polls input from the multi function board.
 */
void gui_handle_input(void);

/**
 * @brief Writes the menu system to the OLED.
 */
void gui_display_menu(void);

/**
 * @brief Custom display function for in game play.
 */
void gui_display_game(char* current_player, uint8_t score);

/**
 * @return 1 when the timer running at 30 Hz has triggered an interrupt to update the display.
 */
uint8_t gui_display_update_flag(void);

#endif