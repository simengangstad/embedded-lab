#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

/**
 * @brief Delays by a given amount of micro seconds.
 * @note Home made
 */
void delay_us(volatile uint32_t t);

#endif /* DELAY_H_ */