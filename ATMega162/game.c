#include "game.h"

#include <avr/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "can_identifiers.h"
#include "drivers/can/can_controller.h"
#include "user_interface/gui.h"
#include "user_interface/input.h"

typedef struct {
    uint8_t player_id;
    char* name;
    uint8_t high_score;
    uint8_t current_score;
} Player;

static Player current_player;

static void game_construct_player(Player* player_ptr, uint8_t player_id) {
    player_ptr->player_id = player_id;
    switch (player_id) {
        case PLAYER_LAMPE_ID:
            player_ptr->name = "Lampe";
            player_ptr->high_score = eeprom_read_byte((uint8_t*)PLAYER_LAMPE_ADDRESS);
            break;
        case PLAYER_TVINNEREIM_ID:
            player_ptr->name = "Tvinnereim";
            player_ptr->high_score = eeprom_read_byte((uint8_t*)PLAYER_TVINNEREIM_ADDRESS);
            break;
        case PLAYER_GANGSTAD_ID:
            player_ptr->name = "Gangstad";
            player_ptr->high_score = eeprom_read_byte((uint8_t*)PLAYER_GANGSTAD_ADDRESS);
            break;
        case PLAYER_GUEST_ID:
            player_ptr->name = "Siawash";
            player_ptr->high_score = eeprom_read_byte((uint8_t*)PLAYER_GUEST_ADDRESS);
            break;
        default:
            break;
    }

    player_ptr->current_score = 0;
}

ISR(INT1_vect) {
    const uint8_t status = mcp2515_read_status();
    Message message;
    message.id = CAN_NO_MESSAGE_ID;

    if (status & MCP_RX0IF) {
        can_controller_read(0, &message);
    } else if (status & MCP_RX1IF) {
        can_controller_read(1, &message);
    }

    switch (message.id) {
        case CAN_SCORE_MESSAGE_ID:
            current_player.current_score = message.data[0];
            break;
        default:
            break;
    }
}

void game_play(uint8_t player_id) {
    game_construct_player(&current_player, player_id);

    // Tell Atsam to start playing
    Message game_start_message = {CAN_GAME_START_ID, 1, {0}};

    can_controller_transmit(&game_start_message);

    Message joystick_message = {CAN_JOYSTICK_MESSAGE_ID, 4, {0, 0, 0, 0}};
    Message touch_message = {CAN_TOUCH_MESSAGE_ID, 4, {0, 0, 0, 0}};

    TouchInput touch_input = input_touch();

    while (!touch_input.left_button) {
        if (gui_display_update_flag()) {
            gui_display_game(current_player.name, current_player.current_score);
            // Send joystick data over CAN
            Joystick joystick = input_joystick();
            joystick_message.data[0] = joystick.x;
            joystick_message.data[1] = joystick.y;
            joystick_message.data[2] = joystick.dir;
            joystick_message.data[3] = joystick.button_pressed;
            can_controller_transmit(&joystick_message);

            // Send touch data over CAN
            touch_input = input_touch();
            touch_message.data[0] = touch_input.left_slider;
            touch_message.data[1] = touch_input.right_slider;
            touch_message.data[2] = touch_input.left_button;
            touch_message.data[3] = touch_input.right_button;
            can_controller_transmit(&touch_message);
        }
    }

    // Save the score if it is a new highscore. A lower score is better
    if (current_player.current_score < current_player.high_score) {
        switch (current_player.player_id) {
            case PLAYER_LAMPE_ID:
                eeprom_update_byte((uint8_t*)PLAYER_LAMPE_ADDRESS, current_player.current_score);
                break;
            case PLAYER_TVINNEREIM_ID:
                eeprom_update_byte((uint8_t*)PLAYER_TVINNEREIM_ADDRESS, current_player.current_score);
                break;
            case PLAYER_GANGSTAD_ID:
                eeprom_update_byte((uint8_t*)PLAYER_GANGSTAD_ADDRESS, current_player.current_score);
                break;
            case PLAYER_GUEST_ID:
                eeprom_update_byte((uint8_t*)PLAYER_GUEST_ADDRESS, current_player.current_score);
                break;
            default:
                break;
        }
    }
}

void game_reset_score_board(void) {
    // 256 is the worst score we can have in a byte in memory
    eeprom_update_byte((uint8_t*)PLAYER_LAMPE_ADDRESS, 255);
    eeprom_update_byte((uint8_t*)PLAYER_TVINNEREIM_ADDRESS, 255);
    eeprom_update_byte((uint8_t*)PLAYER_GANGSTAD_ADDRESS, 255);
    eeprom_update_byte((uint8_t*)PLAYER_GUEST_ADDRESS, 255);
}