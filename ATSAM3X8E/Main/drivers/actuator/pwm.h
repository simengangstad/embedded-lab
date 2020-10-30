/**
 * @file pwm.h
 * @brief Driver for setting up a specific PWM signal.
 */

#ifndef PWM_H
#define PWM_H

/**
 * @brief Sets up the specific pin (PC18) for PWM output.
 */
void pwm_init(void);

/**
 * @brief Sets the duty cycle of the PWM signal.
 */
void pwm_set_duty_cycle(float percent);

#endif