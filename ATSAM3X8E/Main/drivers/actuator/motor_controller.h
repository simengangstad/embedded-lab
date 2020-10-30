/**
 * @file motor_controller.h
 * @brief A PI position controller for the motor.
 */

#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "../can/can_joystick.h"

/**
 * @brief Sets up the motor interface and the timer for the control loop, so that it runs on a fixed
 *        time step.
 */
void motor_controller_init(void);

/**
 * @brief Sets the reference of the PI position controller based on the @p joystick.
 */
void motor_controller_set_reference(Joystick* joystick);

/**
 * @brief Sets the reference of an open loop controller, which is a speed based.
 */
void motor_controller_open_loop_update(Joystick* joystick);

#endif