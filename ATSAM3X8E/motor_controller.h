/**
 * @file motor_controller.h
 * @brief A PI position controller for the motor.
 */

#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <stdint.h>

/**
 * @brief Sets up the motor interface and the timer for the control loop, so that it runs on a fixed
 *        time step.
 */
void motor_controller_init(void);

/**
 * @brief Starts the controller and motor. Resets the integrator
 */
void motor_controller_start(void);

/**
 * @brief Stops the controller and motor.
 */
void motor_controller_stop(void);

/**
 * @brief Sets the reference of the PI position controller based on the @p reference.
 * @param ref A position between -100 and 100. Mid point is 0.
 */
void motor_controller_set_reference(int8_t ref);

/**
 * @brief Sets the reference of an open loop controller, which is speed based.
 */
// void motor_controller_open_loop_update(Joystick* joystick);

#endif