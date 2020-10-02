
#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include <stdio.h>

void can_controller_init(void);

void can_controller_transmit(uint8_t* data, uint8_t data_length);

#endif /* CAN_CONTROLLER_H_ */