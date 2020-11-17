/**
 * @mainpage
 * Interfaces with the ATMega, the actuators and the encoder. Responds to game logic input
 */

/**
 * @file main.c
 */

#include "drivers/can_controller.h"
#include "drivers/uart_and_printf/printf-stdarg.h"
#include "drivers/uart_and_printf/uart.h"
#include "game.h"
#include "player_input.h"
#include "sam.h"

// Set baud rate prescaler to 41, PS1 = 7 TQ, PS2 = 6 TQ, PROPAG = 2 TQ, SJW = 1 TQ.
#define ATSAM_CAN_BR 0x00290165

void ATSAM_INIT(void) {
    SystemInit();
    configure_uart();
    can_init_def_tx_rx_mb(ATSAM_CAN_BR);
    game_init();

    // Disable watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
}

int main(void) {
    ATSAM_INIT();

    while (1) {
        if (player_input_game_start_flag()) {
            game_loop();
            player_input_clear_game_start_flag();
        }
    }

    return 0;
}
