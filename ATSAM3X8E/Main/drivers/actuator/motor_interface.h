/**
 * @file motor_interface.h
 * @brief Driver for the game board motor.
 */

#include <stdint.h>

#ifndef MOTOR_INTERFACE_H
#define MOTOR_INTERFACE_H

typedef enum { DRIVE_LEFT, DRIVE_RIGHT } MotorDirection;

/**
 * @brief Sets up the MJ1 and MJ2 ports for the control signals and encoder input.
 *        Also sets up the DAC which provides the input for the motor.
 *
 */
void motor_interface_init(void);

/**
 * @brief Modifies the DIR pin in MJ1.
 */
void motor_interface_set_direction(MotorDirection dir);

/**
 * @brief Sets the speed of the motor, where 0xFFFFFFFF corresponds to 5 V.
 */
void motor_interface_set_actuation(uint16_t speed);

/**
 * @brief Reads the motor encoder.
 */
int16_t motor_interface_read_position(void);

#endif
