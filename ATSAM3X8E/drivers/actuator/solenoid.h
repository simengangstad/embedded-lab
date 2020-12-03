
#ifndef SOLENOID_H_
#define SOLENOID_H_

#include <stdint.h>

void solenoid_init(void);

/**
 * @brief Activate the solenoid. 1 to turn on, 0 to turn off
 */
void solenoid_activate(uint8_t active);

#endif /* SOLENOID_H_ */