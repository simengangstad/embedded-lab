#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define PLAYER_LAMPE_ID 0
#define PLAYER_TVINNEREIM_ID 1
#define PLAYER_GANGSTAD_ID 2
#define PLAYER_GUEST_ID 3

#define PLAYER_LAMPE_ADDRESS 0
#define PLAYER_TVINNEREIM_ADDRESS 1
#define PLAYER_GANGSTAD_ADDRESS 2
#define PLAYER_GUEST_ADDRESS 3

void game_play(uint8_t player_id);

void game_reset_score_board(void);

#endif