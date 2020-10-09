#include "drivers/can/can_controller.h"
#include "drivers/uart_and_printf/printf-stdarg.h"
#include "drivers/uart_and_printf/uart.h"
#include "sam.h"

// Set baud rate prescaler to 41, PS1 = 7 Tq, PS2 = 6 Tq, PROPAG = 2 Tq, SJW = 1 Tq. 
#define ATSAM_CAN_BR 0x00290165

void ATSAM_leds(void) {
    // Since PIOA is of instance 11, we have to enable that clock
    PMC->PMC_PCER0 = PMC_PCER0_PID11;

    // Enable pins
    PIOA->PIO_PER = PIO_PA19;
    PIOA->PIO_PER = PIO_PA20;

    // Set to output
    PIOA->PIO_OER = PIO_PA19;
    PIOA->PIO_OER = PIO_PA20;

    // Set to logical high
    PIOA->PIO_SODR = PIO_SODR_P19;
    PIOA->PIO_SODR = PIO_SODR_P20;
}

void ATSAM_INIT(void) {
    SystemInit();
    configure_uart();

    // Disable watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
}

int main(void) {
    ATSAM_INIT();	
	can_init_def_tx_rx_mb(ATSAM_CAN_BR);

    while (1) {
        CAN_MESSAGE can_message;
        if (!can_receive(&can_message, 0)) {
			//printf("Error receiving message\n\r");
		}
		else {
			if (can_message.id == 1) {
				
				printf("X: %d, Y: %d\n\r", (int8_t) can_message.data[0], (int8_t) can_message.data[1]);
				//printf("Direction %i\n\r", can_message.data[2]);
			}
		}
    }

    return 0;
}