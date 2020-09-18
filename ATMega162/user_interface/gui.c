#include "gui.h"

#include "../driver/oled.h"
#include "input.h"

static MenuItem* main_menu = malloc(sizeof(MenuItem));

void gui_add_menu_item(MenuItem* current_item, char* text, void (*function)()) {
    MenuItem* new_item = malloc(size_of(MenuItem));

    new_item->text = text;
    new_item->func = function;
    new_item->prev = current_item;
    new_item->next = NULL;

    current_item->next = new_item;
}

void gui_add_submenu_item(MenuItem* current_item, char* text,
                          void (*function)()) {
    MenuItem* sub_item = malloc(size_of(MenuItem));

    sub_item->text = text;
    sub_item->func = function;
    sub_item->parent = current_item;
    sub_item->next = NULL;

    if (current_item->children != NULL) {
        MenuItem* child = current_item->children->next;

        while (child != NULL) {
            child = child->next;
        }

        child->next = sub_item;

    } else {
        current_item->children = sub_item;
    }
}

void gui_display(MenuItem* menu_item) {
    do {
        oled_print(menu_item->text);
        // TODO:  Keep track of rows
        menu_item = menu_item->next;
    } while (menu_item->next)
}