/**
 * @file can_controller.h
 * @brief Driver for interfacing with the MCP2515 with transmits and receives on the
 *        controller area network.
 */

#ifndef CAN_CONTROLLER_H
#define CAN_CONTROLLER_H

#include <stdio.h>

/**
 * @brief Structure for a CAN message.
 */
typedef struct Message {
    /**
     * @note Only the 11 LSB bits are used on the network.
     */
    uint16_t id;

    /**
     * @note Even though the data is set to 8 bytes, the length specifices how many of these bytes
     *        we use in the CAN message transfer
     */
    uint8_t length;

    uint8_t data[8];
} Message;

/**
 * @brief Sets up registers in the MCP2515 for the CAN interface.
 */
void can_controller_init(void);

/**
 * @brief Sends the CAN message to the MCP2515 over SPI, which is sent from
 *        the MCP onto the network.
 */
void can_controller_transmit(Message* message_ptr);

/**
 * @brief Reads the message buffer at the specific @p buffer_id and stores it in
 *        @p message_ptr.
 */
void can_controller_read(uint8_t buffer_id, Message* message_ptr);

#endif