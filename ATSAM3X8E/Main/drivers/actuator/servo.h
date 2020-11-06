/**
 * @file servo.h
 * @brief Driver for the servo motor on the game board.
 */

#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

/**
 * @brief Sends a duty cycle to the servo by transforming the @p slider_position.
 */
void servo_send_duty_cycle(int8_t slider_position);

#endif