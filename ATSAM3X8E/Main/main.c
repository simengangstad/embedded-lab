#include <string.h>

#include "drivers/actuator/motor_controller.h"
#include "drivers/actuator/motor_interface.h"
#include "drivers/actuator/pwm.h"
#include "drivers/actuator/servo.h"
#include "drivers/adc.h"
#include "drivers/can/can_controller.h"
#include "drivers/can/can_joystick.h"
#include "drivers/uart_and_printf/printf-stdarg.h"
#include "drivers/uart_and_printf/uart.h"
#include "game.h"
#include "sam.h"

#include <math.h>

// Set baud rate prescaler to 41, PS1 = 7 TQ, PS2 = 6 TQ, PROPAG = 2 TQ, SJW = 1 TQ.
#define ATSAM_CAN_BR 0x00290165

void ATSAM_leds(void) {
    // Since PIOA is of instance 11, we have to enable that clock
    PMC->PMC_PCER0 = PMC_PCER0_PID11;

    // Enable pins
    PIOA->PIO_PER = PIO_PA19;  // PIO:PER_P19
    PIOA->PIO_PER = PIO_PA20;  // PIO_PER_P20

    // Set to output
    PIOA->PIO_OER = PIO_PA19;
    PIOA->PIO_OER = PIO_PA20;

    //// Set to logical high
    PIOA->PIO_SODR = PIO_SODR_P19;
    PIOA->PIO_SODR = PIO_SODR_P20;
}

void ATSAM_INIT(void) {
    SystemInit();
    configure_uart();
    pwm_init();
    adc_init();
    motor_interface_init();
	motor_controller_init();
    ATSAM_leds();

    // Disable watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
}

int main(void) {
    ATSAM_INIT();
    can_init_def_tx_rx_mb(ATSAM_CAN_BR);
    // motor_set_actuation(0xFFFF);

	PIOA->PIO_PER = PIO_PA6;
	PIOA->PIO_OER = PIO_PA6;
	PIOA->PIO_SODR = PIO_SODR_P6;
	
    Joystick joystick;
    while (1) {
        can_joystick_read(&joystick);
        // servo_send_duty_cycle(joystick.y);

		motor_controller_set_reference(&joystick);

        // game_update();
    }

    return 0;
}
