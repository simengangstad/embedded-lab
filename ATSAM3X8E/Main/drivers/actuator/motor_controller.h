
#include <stdint.h>

#ifndef MOTOR_CONTROLLER_H_
#define MOTOR_CONTROLLER_H_



void motor_init(void);

void motor_open_loop_speed_control(Joystick* joystick); 

#endif