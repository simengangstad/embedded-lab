/**
 * @file motor_controller.c
 */

#include "motor_controller.h"

#include <stdlib.h>

#include "motor_interface.h"
#include "sam.h"

// Assume initial central position
#define JOYSTICK_TO_POSITION_SCALE 45
#define POSITION_BOUNDRY_RIGHT 4500
#define POSITION_BOUNDRY_LEFT -4500

#define SCALING_OPENLOOP_MAX_GAIN 655  // ~(2^16-1)/100
#define OPEN_LOOP_GAIN 15

/**
 * @brief Update the controller every 1 ms
 */
#define TIME_STEP 1.0e-3

/**
 * @brief Count to 1 ms, (MCK/128) / TRC.
 */
#define TIMER_COMPARE_THRESHOLD 65625

#define SMOOTHING_FACTOR 500.0;

/**
 * @brief Gains for the PI controller.
 */
static const float kp = 0.3, ki = 0.6;

static int32_t reference = 0;

/**
 * @brief The control law for the PI position controller.
 */
static void motor_controller_update() {
    static float integrated_error = 0;

    int16_t position = motor_interface_read_position();

    // There were some high noisy values from the encoder; cut them off:
    if (position < POSITION_BOUNDRY_LEFT) {
        position = POSITION_BOUNDRY_LEFT;
    } else if (position > POSITION_BOUNDRY_RIGHT) {
        position = POSITION_BOUNDRY_RIGHT;
    }

    // Controller error
    float error = reference - position;

    integrated_error += error * TIME_STEP;

    float actuation = kp * error + ki * integrated_error;

    MotorDirection dir = (actuation < 0) ? DRIVE_LEFT : DRIVE_RIGHT;
    motor_interface_set_direction(dir);
    motor_interface_set_actuation((uint16_t)abs(actuation));
    printf("Reference: %d Position: %d Actuation %d\n\r", reference, position, (int)actuation);
}

void TC0_Handler(void) {
    // Clear the status register, to acknowledge interrupt
    TC0->TC_CHANNEL[0].TC_SR;

    motor_controller_update();
}

void motor_controller_init() {
    motor_interface_init();
    // Set up timer so that the control loop is executed on a fixed interval
    // Enable PMC for timer counter channel 0
    PMC->PMC_PCER0 |= PMC_PCER0_PID27;

    // Divide MCK by 128 for timer clock
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_CPCTRG;

    // Set timer compare value
    TC0->TC_CHANNEL[0].TC_RC = TIMER_COMPARE_THRESHOLD;

    // Enable interrupt on RC compare
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
    NVIC_EnableIRQ(TC0_IRQn);

    // Enable clock
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void motor_controller_set_reference(Joystick* joystick) { reference = JOYSTICK_TO_POSITION_SCALE * (joystick->x); }

void motor_controller_open_loop_update(Joystick* joystick) {
    if (joystick->dir == JS_RIGHT) {
        motor_interface_set_direction(DRIVE_RIGHT);
    } else if (joystick->dir == JS_LEFT) {
        motor_interface_set_direction(DRIVE_LEFT);
    }

    uint16_t absolute_x = (uint16_t)abs(joystick->x);
    uint16_t speed = absolute_x * OPEN_LOOP_GAIN;

    motor_interface_set_actuation(speed);
}
