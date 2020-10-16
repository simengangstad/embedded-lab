#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include <stdio.h>

typedef struct Message {
    uint16_t id;
    uint8_t length;
    uint8_t data[8];
} Message;

void can_controller_init(void);

void can_controller_transmit(Message* message_ptr);

Message* can_controller_read(uint8_t buffer_id);

#endif