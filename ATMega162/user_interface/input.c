/**
 * @file input.c
 */

#include "input.h"

#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>

#include "../drivers/adc.h"

#define CALIBRATION_STEPS 50
#define CALIBRATION_STEP_INTERVAL 5
#define JOYSTICK_DEADZONE_X 15
#define JOYSTICK_DEADZONE_Y 20
#define JOYSTICK_DEADZONE_DIRECTION 50

#define LEFT_TOUCH_BUTTON_PIN DDD5
#define RIGHT_TOUCH_BUTTON_PIN DDD4
#define JOYSTICK_BUTTON_PIN DDB1

/**************************************************************************************************************/
/*                                              STATIC FUNCTIONALITY                                          */
/**************************************************************************************************************/

/**
 * @brief Joystick midpoint/offset in x.
 */
static uint8_t midpoint_x = 0;

/**
 * @brief Joystick midpoint/offset in y.
 */
static uint8_t midpoint_y = 0;

/**
 * @brief Performs CALIBRATION_STEPS reads of the joystick position and
 * calculates the offset midpoint.
 */
static void input_calibrate_joystick(void) {
    int midpoint_x_total = 0;
    int midpoint_y_total = 0;

    for (uint8_t i = 0; i < CALIBRATION_STEPS; i++) {
        midpoint_x_total += adc_read(ADC_JOYSTICK_X_CHANNEL);
        midpoint_y_total += adc_read(ADC_JOYSTICK_Y_CHANNEL);
        _delay_ms(CALIBRATION_STEP_INTERVAL);
    }

    midpoint_x = midpoint_x_total / CALIBRATION_STEPS;
    midpoint_y = midpoint_y_total / CALIBRATION_STEPS;

    printf("Joystick calibration done, midpoint: %d, %d\r\n", midpoint_x, midpoint_y);
}

/**
 * @brief Corrects bias and sets up the correct scale for the joystick input.
 *
 * @param value Joystick input value to correct.
 * @param midpoint The calculated midpoint from calibration.
 *
 * @return A value from -100 to 100.
 */
static int input_correct_joystick_from_calibration(uint8_t value, uint8_t midpoint) {
    const float delta_left = midpoint - 0;
    const float delta_right = 255 - midpoint;

    const int corrected_value = value - midpoint;

    if (corrected_value < 0) {
        return 100 * corrected_value / delta_left;
    } else {
        return 100 * corrected_value / delta_right;
    }
}

/**
 * @return 0/1 when not pressed / pressed.
 */
static uint8_t input_joystick_button_pressed(void) { return (PINB & (1 << JOYSTICK_BUTTON_PIN)) > 0 ? 0 : 1; }

/**
 * @return 0/1 when not pressed / pressed.
 */
static uint8_t input_left_button_pressed(void) { return (PIND & (1 << LEFT_TOUCH_BUTTON_PIN)) > 0 ? 1 : 0; }

/**
 * @return 0/1 when not pressed / pressed.
 */
static uint8_t input_right_button_pressed(void) { return (PIND & (1 << RIGHT_TOUCH_BUTTON_PIN)) > 0 ? 1 : 0; }

/**************************************************************************************************************/
/*                                              INTERFACE                                                     */
/**************************************************************************************************************/

void input_init(void) {
    adc_init();
    input_calibrate_joystick();

    // Set up pins on PD4 and PD5 for button input, PB1 for joystick button.
    DDRD &= ~(1 << LEFT_TOUCH_BUTTON_PIN);
    DDRD &= ~(1 << RIGHT_TOUCH_BUTTON_PIN);
    DDRB &= ~(1 << JOYSTICK_BUTTON_PIN);
}

Joystick input_joystick(void) {
    Joystick joystick;

    joystick.x = input_correct_joystick_from_calibration(adc_read(ADC_JOYSTICK_X_CHANNEL), midpoint_x);

    if (abs(joystick.x) < JOYSTICK_DEADZONE_X) {
        joystick.x = 0;
    }

    joystick.y = input_correct_joystick_from_calibration(adc_read(ADC_JOYSTICK_Y_CHANNEL), midpoint_y);

    if (abs(joystick.y) < JOYSTICK_DEADZONE_Y) {
        joystick.y = 0;
    }

    joystick.dir = NEUTRAL;
    joystick.button_pressed = input_joystick_button_pressed();

    if (abs(joystick.x) > abs(joystick.y)) {
        if (joystick.x < -JOYSTICK_DEADZONE_DIRECTION) {
            joystick.dir = LEFT;
        } else if (joystick.x > JOYSTICK_DEADZONE_DIRECTION) {
            joystick.dir = RIGHT;
        }
    } else {
        if (joystick.y < -JOYSTICK_DEADZONE_DIRECTION) {
            joystick.dir = DOWN;
        } else if (joystick.y > JOYSTICK_DEADZONE_DIRECTION) {
            joystick.dir = UP;
        }
    }

    return joystick;
}

TouchInput input_touch(void) {
    TouchInput touch_input;

    touch_input.left_slider = 100 * adc_read(ADC_LEFT_SLIDER_CHANNEL) / 255.0;
    touch_input.right_slider = 100 * adc_read(ADC_RIGHT_SLIDER_CHANNEL) / 255.0;

    touch_input.left_button = input_left_button_pressed();
    touch_input.right_button = input_right_button_pressed();

    return touch_input;
}

void input_test(void) {
    while (1) {
        const Joystick joystick = input_joystick();
        const TouchInput touch_input = input_touch();

        printf("x: %d, y: %d, direction: %d ", joystick.x, joystick.y, joystick.dir);
        printf("left slider: %d, right slider: %d, left button: %i, right button: %i, joystick button: %i \r\n",
               touch_input.left_slider, touch_input.right_slider, touch_input.left_button, touch_input.right_button,
               joystick.button_pressed);
        _delay_ms(200);
    }
}
