#include "gui.h"

#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

static Menu* current_menu;
static MenuItem* current_item;
static JoystickDirection previous_direction = NEUTRAL;
static uint8_t previous_button_state = 0;
static uint8_t display_update_flag = 0;

static Menu* gui_construct_menu(Menu* parent_menu, MenuItem* parent_item, char* title) {
    Menu* menu = (Menu*)malloc(sizeof(Menu));

    menu->parent_menu = parent_menu;
    menu->parent_item = parent_item;
    menu->title = title;
    menu->top_item = NULL;

    return menu;
}

static MenuItem* gui_add_menu_item(Menu* menu, char* text, void (*function)()) {
    MenuItem* new_item = (MenuItem*)malloc(sizeof(MenuItem));

    new_item->text = text;
    new_item->func = function;
    new_item->next = NULL;
    new_item->sub_menu = NULL;

    if (menu->top_item) {
        // Iterate down the list until we find the last item
        MenuItem* last_item = menu->top_item;
        while (last_item->next) {
            last_item = last_item->next;
        }

        // Add the new item after the previous last item
        last_item->next = new_item;
        new_item->prev = last_item;
    } else {
        menu->top_item = new_item;
    }

    return new_item;
}

void print_button_pressed() { printf("Pressed\r\n"); }

ISR(TIMER1_COMPA_vect) {
    display_update_flag += 1;
}

static void gui_setup_update_display_timer() {
    // Set the timer to fire on 20 Hz
	// 60: 79
	// 30: 159
	// 20: 239
    // 10: 479
    OCR1A = 159;

    // Set mode to CTC, clear timer on compare with prescaling 1024.
    TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);

    // Disable interrupts
    cli();

    // Set up interrupt on compare
    TIMSK |= (1 << OCIE1A);

    // Enable interrupts
    sei();
}

void gui_init() {
    current_menu = gui_construct_menu(NULL, NULL, "Main Menu");
    gui_add_menu_item(current_menu, "Play", &print_button_pressed);
    MenuItem* settings_item = gui_add_menu_item(current_menu, "Settings", NULL);
    gui_add_menu_item(current_menu, "Score", NULL);
    MenuItem* credits_item = gui_add_menu_item(current_menu, "Credits", NULL);

    Menu* settings_menu = gui_construct_menu(current_menu, settings_item, "Settings");
    settings_item->sub_menu = settings_menu;

    Menu* credits_menu = gui_construct_menu(current_menu, credits_item, "Credits");
    gui_add_menu_item(credits_menu, "Lampe", NULL);
    gui_add_menu_item(credits_menu, "Tvinnereim", NULL);
    gui_add_menu_item(credits_menu, "Gangstad", NULL);
    gui_add_menu_item(credits_menu, "Copyright 2020", NULL);
    credits_item->sub_menu = credits_menu;

    current_item = current_menu->top_item;

    // Set up interrupt
    gui_setup_update_display_timer();
}

void gui_handle_input() {
    JoystickDirection current_direction = input_joystick_direction();

    if (current_direction != previous_direction) {
        switch (current_direction) {
            case UP:
                if (current_item->prev) {
                    current_item = current_item->prev;
                }
                break;
            case DOWN:
                if (current_item->next) {
                    current_item = current_item->next;
                }
                break;
            case RIGHT:
                if (current_item->sub_menu) {
                    current_menu = current_item->sub_menu;
                    current_item = current_item->sub_menu->top_item;
                } else if (current_item->func) {
                    current_item->func();
                }
                break;
            case LEFT:
                if (current_menu->parent_menu) {
                    current_item = current_menu->parent_item;
                    current_menu = current_menu->parent_menu;
                }
                break;
            default:
                break;
        }
        previous_direction = current_direction;
    }

    uint8_t current_button_state = input_joystick_button_pressed();

    if (previous_button_state != current_button_state) {
        if (current_item->func && current_button_state) {
            current_item->func();
        }

        previous_button_state = current_button_state;
    }
}

void gui_display() {

    oled_clear();

    // If we are in a submenu
    if (current_menu->parent_menu) {
        oled_pos(0, 10);
        oled_print_left_arrow(NON_INVERTED);
    }

    // Offset position half the length of the title menu from center
    oled_pos(0, COLUMN_SIZE / 2 - strlen(current_menu->title) * 8 / 2);
    oled_print(current_menu->title, LARGE, NON_INVERTED);

    uint8_t row = 2;
    MenuItem* iterator = current_menu->top_item;

    while (iterator) {
        oled_pos(row, 0);

        if (iterator == current_item) {
            if (iterator->sub_menu) {
                oled_print(iterator->text, FONT_SIZE, INVERTED);
                oled_print_right_arrow(INVERTED);
            } else {
                oled_print(iterator->text, FONT_SIZE, INVERTED);
                // oled_print_circle(INVERTED);
            }
        } else {
            oled_print(iterator->text, FONT_SIZE, NON_INVERTED);
        }

        iterator = iterator->next;
        row++;
    }
	
    oled_update();
	
	if (display_update_flag > 1) {
		display_update_flag -= 1;	
	}
	else {
		display_update_flag = 0;
	}
	
}

uint8_t gui_display_update_flag() { return display_update_flag > 0 ? 1 : 0; }