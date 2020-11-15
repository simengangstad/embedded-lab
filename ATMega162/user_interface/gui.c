/**
 * @file gui.c
 */

#include "gui.h"

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#include "../game.h"
#include "input.h"

// Declare first to make them accesable in the structs themselves
typedef struct MenuItem MenuItem;
typedef struct Menu Menu;

/**
 * @brief A single menu item.
 */
struct MenuItem {
    char* text;
    void (*action)();
    MenuItem* next;
    MenuItem* prev;
    Menu* sub_menu;
    uint8_t show_arrow;
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
 * @brief Holds the current menu in the menu system.
 */
static Menu* current_menu;

/**
 * @brief Holds the current menu item selected.
 */
static MenuItem* current_item;

/**
 * @brief Gets set to 1 when the timer running at the display refresh rate fires an interrupt.
 *        Is used for checking whether a new gui_display call should be issued.
 */
static uint8_t display_update_flag = 0;

/**
 * @brief Sets up a menu with a @p parent_menu (if any) and a @p parent_item (if any),
 *        which is item which will go into this menu when clicked.
 *
 * @note The function mallocs the menu.
 */
static Menu* gui_construct_menu(Menu* parent_menu, MenuItem* parent_item, char* title) {
    Menu* menu = (Menu*)malloc(sizeof(Menu));

    if (menu == NULL) {
        printf("1\r\n");
    }

    menu->parent_menu = parent_menu;
    menu->parent_item = parent_item;
    menu->title = title;  // malloc((strlen(title) + 1) * sizeof(char));
    // strcpy(menu->title, title);
    menu->top_item = NULL;

    return menu;
}

/**
 * @brief Adds a menu item to a menu.
 *
 * @param function Will be executed when the item is clicked.
 *
 * @note The function mallocs the menu item.
 */
static MenuItem* gui_add_menu_item(Menu* menu, char* text, uint8_t show_arrow, void (*function)()) {
    MenuItem* new_item = malloc(sizeof(MenuItem));

    if (new_item == NULL) {
        printf("2\r\n");
    }

    new_item->text = text;  // = malloc((strlen(text) + 1) * sizeof(char));
    // strcpy(new_item->text, text);
    new_item->action = function;
    new_item->next = NULL;
    new_item->sub_menu = NULL;
    new_item->show_arrow = show_arrow;
    new_item->prev = NULL;

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

ISR(TIMER1_COMPA_vect) { display_update_flag = 1; }

/**
 * @brief Sets up a timer which updates the display update flag at a given frequency.
 */
static void gui_setup_update_display_timer(void) {
    // Set the timer to fire on 30 Hz
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

static void gui_start_game_user_lampe(void);
static void gui_start_game_user_tvinnereim(void);
static void gui_start_game_user_gangstad(void);
static void gui_start_game_user_guest(void);
static void gui_scores();
static void gui_credits();
static void gui_reset_settings();

static void gui_start(void) {
    current_menu = gui_construct_menu(NULL, NULL, "Main Menu");

    // Construct screen where one chooses the player
    MenuItem* play_item = gui_add_menu_item(current_menu, "Play", 1, NULL);
    Menu* choose_player_menu = gui_construct_menu(current_menu, play_item, "Log in");

    gui_add_menu_item(choose_player_menu, "Lampe", 0, &gui_start_game_user_lampe);
    gui_add_menu_item(choose_player_menu, "Tvinnereim", 0, &gui_start_game_user_tvinnereim);
    gui_add_menu_item(choose_player_menu, "Gangstad", 0, &gui_start_game_user_gangstad);
    gui_add_menu_item(choose_player_menu, "Guest", 0, &gui_start_game_user_guest);

    play_item->sub_menu = choose_player_menu;

    // Scores
    gui_add_menu_item(current_menu, "Score", 1, &gui_scores);

    // Settings
    MenuItem* settings_item = gui_add_menu_item(current_menu, "Settings", 1, NULL);
    Menu* settings_menu = gui_construct_menu(current_menu, settings_item, "Settings");
    settings_item->sub_menu = settings_menu;
    gui_add_menu_item(settings_menu, "Reset scores", 0, &gui_reset_settings);

    // Credits
    gui_add_menu_item(current_menu, "Credits", 1, &gui_credits);

    current_item = current_menu->top_item;
}

static void gui_stop(void) {
    while (current_menu->parent_menu != NULL) {
        current_menu = current_menu->parent_menu;
    }
    current_item = current_menu->top_item;

    // Iterative Depth-First search to free every menu and item
    Menu* menu_temp;
    MenuItem* item_temp;

    while (current_item != NULL) {
        if (current_item->sub_menu != NULL) {
            current_menu = current_item->sub_menu;
            current_item = current_item->sub_menu->top_item;
        } else {
            item_temp = current_item;
            current_item = current_item->next;
            current_menu->top_item = current_item;
            free(item_temp);
        }
        if (current_item == NULL) {
            menu_temp = current_menu;
            current_menu = current_menu->parent_menu;
            free(menu_temp);

            if (current_menu != NULL) {
                current_item = current_menu->top_item->next;
                free(current_menu->top_item);
                current_menu->top_item = current_item;
            }
        }
    }
}

/**************************************************************************************************************/
/*                                  Actions for the GUI items                                                 */
/**************************************************************************************************************/

static void gui_start_game_user_lampe(void) {
    gui_stop();
    game_play(PLAYER_LAMPE_ID);
    gui_start();
}

static void gui_start_game_user_tvinnereim(void) {
    gui_stop();
    game_play(PLAYER_TVINNEREIM_ID);
    gui_start();
}

static void gui_start_game_user_gangstad(void) {
    gui_stop();
    game_play(PLAYER_GANGSTAD_ID);
    gui_start();
}

static void gui_start_game_user_guest(void) {
    gui_stop();
    game_play(PLAYER_GUEST_ID);
    gui_start();
}

static void gui_scores() {
    oled_clear();

    oled_pos(0, 10);
    oled_print_left_arrow(NON_INVERTED);

    // Offset position half the length of the title menu from center
    // Take the center position of the oled screen and subtract half the size of the text.
    // Multiply with 8 because the font size large is 8 pixels per character, divide by 2
    // to get the center position.
    char* highscore = "Highscore";
    oled_pos(0, COLUMN_SIZE / 2 - strlen(highscore) * 8 / 2);
    oled_print(highscore, LARGE, NON_INVERTED);

    char score_buffer[3];
    oled_pos(3, 0);
    oled_print("Lampe: ", LARGE, NON_INVERTED);
    itoa(eeprom_read_byte((uint8_t*)PLAYER_LAMPE_ADDRESS), score_buffer, 10);
    oled_print(score_buffer, LARGE, NON_INVERTED);

    oled_pos(4, 0);
    oled_print("Tvinnereim: ", LARGE, NON_INVERTED);
    itoa(eeprom_read_byte((uint8_t*)PLAYER_TVINNEREIM_ADDRESS), score_buffer, 10);
    oled_print(score_buffer, LARGE, NON_INVERTED);

    oled_pos(5, 0);
    oled_print("Gangstad: ", LARGE, NON_INVERTED);
    itoa(eeprom_read_byte((uint8_t*)PLAYER_GANGSTAD_ADDRESS), score_buffer, 10);
    oled_print(score_buffer, LARGE, NON_INVERTED);

    oled_pos(6, 0);
    oled_print("Guest: ", LARGE, NON_INVERTED);
    itoa(eeprom_read_byte((uint8_t*)PLAYER_GUEST_ADDRESS), score_buffer, 10);
    oled_print(score_buffer, LARGE, NON_INVERTED);

    oled_update();

    Joystick joystick;

    do {
        joystick = input_joystick();
        display_update_flag = 0;
    } while (joystick.dir != LEFT);
}

static void gui_credits() {
    oled_clear();

    oled_pos(0, 10);
    oled_print_left_arrow(NON_INVERTED);

    // Offset position half the length of the title menu from center
    // Take the center position of the oled screen and subtract half the size of the text.
    // Multiply with 8 because the font size large is 8 pixels per character, divide by 2
    // to get the center position.
    char* credits = "Credits";
    oled_pos(0, COLUMN_SIZE / 2 - strlen(credits) * 8 / 2);
    oled_print(credits, LARGE, NON_INVERTED);

    oled_pos(2, 0);
    oled_print("Lampe", LARGE, NON_INVERTED);

    oled_pos(3, 0);
    oled_print("Tvinnereim", LARGE, NON_INVERTED);

    oled_pos(4, 0);
    oled_print("Gangstad", LARGE, NON_INVERTED);

    oled_pos(5, 0);
    oled_print("Guest: ", LARGE, NON_INVERTED);

    oled_pos(7, 0);
    oled_print("Copyright 2020", LARGE, NON_INVERTED);

    oled_update();

    Joystick joystick;

    do {
        joystick = input_joystick();
        display_update_flag = 0;
    } while (joystick.dir != LEFT);
}

static void gui_reset_settings() { game_reset_score_board(); }

/**************************************************************************************************************/

void gui_init(void) {
    oled_init();
    input_init();
    gui_start();

    // Set up interrupt
    gui_setup_update_display_timer();
}

void gui_handle_input(void) {
    // Used to detect a change in button state.
    static uint8_t previous_button_state = 0;

    // Used to detect a change in direction.
    static JoystickDirection previous_direction = NEUTRAL;

    Joystick joystick = input_joystick();

    if (joystick.dir != previous_direction) {
        switch (joystick.dir) {
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
                } else if (current_item->action) {
                    current_item->action();
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
        previous_direction = joystick.dir;
    }

    if (previous_button_state != joystick.button_pressed) {
        if (current_item->action && joystick.button_pressed) {
            current_item->action();
        }

        previous_button_state = joystick.button_pressed;
    }
}

void gui_display_menu(void) {
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
            oled_print(iterator->text, FONT_SIZE, INVERTED);
            if (iterator->show_arrow) {
                oled_print_right_arrow(INVERTED);
            }
        } else {
            oled_print(iterator->text, FONT_SIZE, NON_INVERTED);
        }

        iterator = iterator->next;
        row++;
    }

    oled_update();

    display_update_flag = 0;
}

void gui_display_game(char* current_player, uint8_t score) {
    oled_clear();

    // Concatenate the score number with a string
    char score_buffer[3];
    itoa(score, score_buffer, 10);

    char score_text[11];
    strcpy(score_text, "Score: -");
    strcpy(score_text, score_buffer);

    // Offset position half the length of the title menu from center
    // Take the center position of the oled screen and subtract half the size of the text.
    // Multiply with 8 because the font size large is 8 pixels per character, divide by 2
    // to get the center position.
    oled_pos(0, COLUMN_SIZE / 2 - strlen(current_player) * 8 / 2);
    oled_print(current_player, LARGE, NON_INVERTED);

    // TODO: fix, don't need strcopy
    oled_pos(3, COLUMN_SIZE / 2 - strlen(score_text) * 8 / 2);
    oled_print(score_text, LARGE, NON_INVERTED);

    oled_pos(7, 20);
    oled_print("Quit", LARGE, NON_INVERTED);

    oled_pos(7, COLUMN_SIZE - strlen("Reset") * 8 - 20);
    oled_print("Reset", LARGE, NON_INVERTED);

    oled_update();

    display_update_flag = 0;
}

uint8_t gui_display_update_flag(void) { return display_update_flag; }