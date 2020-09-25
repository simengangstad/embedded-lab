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
void gui_init();

/**
 * @brief Polls input from the joystick.
 */
void gui_handle_input();

/**
 * @brief Overwrites the OLED buffer with a buffer located in SRAM.
 */
void gui_display();

#endif