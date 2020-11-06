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

void input_init(void) {
    input_calibrate_joystick();

    // Set up pins on PD4 and PD5 for button input, PB1 for joystick button.
    DDRD &= ~(1 << LEFT_TOUCH_BUTTON_PIN);
    DDRD &= ~(1 << RIGHT_TOUCH_BUTTON_PIN);
    DDRB &= ~(1 << JOYSTICK_BUTTON_PIN);
}

JoystickPosition input_joystick_position(void) {
    JoystickPosition position;

    position.x = input_correct_joystick_from_calibration(adc_read(ADC_JOYSTICK_X_CHANNEL), midpoint_x);

    if (abs(position.x) < JOYSTICK_DEADZONE_X) {
        position.x = 0;
    }

    position.y = input_correct_joystick_from_calibration(adc_read(ADC_JOYSTICK_Y_CHANNEL), midpoint_y);

    if (abs(position.y) < JOYSTICK_DEADZONE_Y) {
        position.y = 0;
    }

    return position;
}

JoystickDirection input_joystick_direction(void) {
    const int8_t value_x = input_correct_joystick_from_calibration(adc_read(ADC_JOYSTICK_X_CHANNEL), midpoint_x);
    const int8_t value_y = input_correct_joystick_from_calibration(adc_read(ADC_JOYSTICK_Y_CHANNEL), midpoint_y);

    if (abs(value_x) > abs(value_y)) {
        if (value_x < -JOYSTICK_DEADZONE_DIRECTION) {
            return LEFT;
        } else if (value_x > JOYSTICK_DEADZONE_DIRECTION) {
            return RIGHT;
        }
    } else {
        if (value_y < -JOYSTICK_DEADZONE_DIRECTION) {
            return DOWN;
        } else if (value_y > JOYSTICK_DEADZONE_DIRECTION) {
            return UP;
        }
    }

    return NEUTRAL;
}

SliderPosition input_slider_position(void) {
    SliderPosition position;

    position.left = 100 * adc_read(ADC_LEFT_SLIDER_CHANNEL) / 255.0;
    position.right = 100 * adc_read(ADC_RIGHT_SLIDER_CHANNEL) / 255.0;

    return position;
}

uint8_t input_left_button_pressed(void) { return (PIND & (1 << LEFT_TOUCH_BUTTON_PIN)) > 0 ? 1 : 0; }

uint8_t input_right_button_pressed(void) { return (PIND & (1 << RIGHT_TOUCH_BUTTON_PIN)) > 0 ? 1 : 0; }

uint8_t input_joystick_button_pressed(void) { return (PINB & (1 << JOYSTICK_BUTTON_PIN)) > 0 ? 0 : 1; }

char* input_get_string_from_joystick_direction(JoystickDirection joystick_direction) {
    switch (joystick_direction) {
        case UP:
            return "up";
        case DOWN:
            return "down";
        case LEFT:
            return "left";
        case RIGHT:
            return "right";
        default:
            return "neutral";
    }
}

void input_test(void) {
    while (1) {
        const JoystickPosition joystick_position = input_joystick_position();
        const SliderPosition slider_position = input_slider_position();
        const uint8_t left_button = input_left_button_pressed();
        const uint8_t right_button = input_right_button_pressed();
        const char* direction = input_get_string_from_joystick_direction(input_joystick_direction());
        const uint8_t joystick_button_pressed = input_joystick_button_pressed();

        printf("x: %d, y: %d, direction: %s ", joystick_position.x, joystick_position.y, direction);
        printf("left slider: %d, right slider: %d, left button: %i, right button: %i, joystick button: %i \r\n",
               slider_position.left, slider_position.right, left_button, right_button, joystick_button_pressed);
        _delay_ms(200);
    }
}
