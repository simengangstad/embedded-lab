#include "input.h"

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "../drivers/adc.h"

#define CALIBRATION_STEPS 50
#define CALIBRATION_STEP_INTERVAL 5
#define JOYSTICK_DEADZONE 20

#define LEFT_TOUCH_BUTTON_PIN PD5
#define RIGHT_TOUCH_BUTTON_PIN PD4

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
static void input_calibrate_joystick() {
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
static int input_correct_joystick_from_calibration(uint8_t value,
                                                   uint8_t midpoint) {
    const float delta_left = midpoint - 0;
    const float delta_right = 255 - midpoint;

    const int corrected_value = value - midpoint;

    if (corrected_value < 0) {
        return 100 * corrected_value / delta_left;
    } else {
        return 100 * corrected_value / delta_right;
    }
}

void input_init() {
    input_calibrate_joystick();

    // Set up pins on PD4 and PD5 for button input
    DDRD &= ~(1 << LEFT_TOUCH_BUTTON_PIN);
    DDRD &= ~(1 << RIGHT_TOUCH_BUTTON_PIN);
}

JoystickPosition input_joystick_position() {
    JoystickPosition position;

    position.x = input_correct_joystick_from_calibration(
        adc_read(ADC_JOYSTICK_X_CHANNEL), midpoint_x);

    if (abs(position.x) < JOYSTICK_DEADZONE) {
        position.x = 0;
    }

    position.y = input_correct_joystick_from_calibration(
        adc_read(ADC_JOYSTICK_Y_CHANNEL), midpoint_y);

    if (abs(position.y) < JOYSTICK_DEADZONE) {
        position.y = 0;
    }

    return position;
}

JoystickDirection input_joystick_horizontal_direction() {
    int8_t value = input_correct_joystick_from_calibration(
        adc_read(ADC_JOYSTICK_X_CHANNEL), midpoint_x);

    if (value < -JOYSTICK_DEADZONE) {
        return LEFT;
    } else if (value > JOYSTICK_DEADZONE) {
        return RIGHT;
    }

    return NEUTRAL;
}

JoystickDirection input_joystick_vertical_direction() {
    int8_t value = input_correct_joystick_from_calibration(
        adc_read(ADC_JOYSTICK_Y_CHANNEL), midpoint_y);

    if (value < -JOYSTICK_DEADZONE) {
        return DOWN;
    } else if (value > JOYSTICK_DEADZONE) {
        return UP;
    }

    return NEUTRAL;
}

SliderPosition input_slider_position() {
    SliderPosition position;

    position.left = 100 * adc_read(ADC_LEFT_SLIDER_CHANNEL) / 255.0;
    position.right = 100 * adc_read(ADC_RIGHT_SLIDER_CHANNEL) / 255.0;

    return position;
}

uint8_t input_left_button_pressed() {
    return (PIND & (1 << LEFT_TOUCH_BUTTON_PIN)) > 0 ? 1 : 0;
}

uint8_t input_right_button_pressed() {
    return (PIND & (1 << RIGHT_TOUCH_BUTTON_PIN)) > 0 ? 1 : 0;
}

void input_test() {
    while (1) {
        JoystickPosition joystick_position = input_joystick_position();
        SliderPosition slider_position = input_slider_position();
        uint8_t left_button = input_left_button_pressed();
        uint8_t right_button = input_right_button_pressed();
        const char* horizontal_direction = get_string_from_joystick_direction(
            input_joystick_horizontal_direction());
        const char* vertical_direction = get_string_from_joystick_direction(
            input_joystick_vertical_direction());

        printf("x: %d, y: %d, vertical: %s, horizontal: %s, ",
               joystick_position.x, joystick_position.y, vertical_direction,
               horizontal_direction);
        printf(
            "left slider: %d, right slider: %d, left button: %i, right button: "
            "%i \r\n",
            slider_position.left, slider_position.right, left_button,
            right_button);
        _delay_ms(200);
    }
}