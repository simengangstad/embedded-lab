/**
 * @file game.c
 */

#include "game.h"

#include <stdint.h>

#include "drivers/adc.h"
#include "drivers/uart_and_printf/printf-stdarg.h"

// The goals we have let in
static uint32_t goals_conceded = 0;

static uint32_t last_ir_blocked_state = 0;

void game_update(void) {
    uint32_t ir_blocked_state = adc_ir_beam_blocked();

    if (last_ir_blocked_state != ir_blocked_state) {
        if (ir_blocked_state) {
            goals_conceded++;
        }
        last_ir_blocked_state = ir_blocked_state;
    }

    printf("Score: %d\r\n", goals_conceded);
}