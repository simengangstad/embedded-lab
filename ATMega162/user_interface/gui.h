/**
 * @file gui.h
 * @brief Graphical user interface on the OLED consisting of a menu system.
 */

#ifndef GUI_H
#define GUI_H

#include <stdio.h>

#include "../drivers/oled.h"

#define FONT_SIZE LARGE

typedef struct MenuItem MenuItem;
typedef struct Menu Menu;

/**
 * @brief A single menu item.
 */
struct MenuItem {
    char* text;
    void (*func)();
    MenuItem* next;
    MenuItem* prev;
    Menu* sub_menu;
};

/**
 * @brief Menu, consisting of a title and a linked list of menu items.
 *
 * @note Parent item refers to the menu item which goes into this menu.
 */
struct Menu {
    char* title;
    MenuItem* top_item;
    MenuItem* parent_item;
    Menu* parent_menu;
};

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
void gui_display(void);

/**
 * @return 1 when the timer running at 30 Hz has triggered an interrupt to update the display.
 */
uint8_t gui_display_update_flag(void);

#endif