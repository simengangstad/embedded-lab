/**
 * @file servo.h
 * @brief Driver for controlling servo with PWM
 */

#ifndef SERVO_H
#define SERVO_H

/**
 * @brief Sets up the specific pin (PC18) for PWM output.
 */
void servo_init(void);

/**
 * @brief Sets the duty cycle of the PWM signal.
 */
void servo_set_duty_cycle(float percent);

#endif