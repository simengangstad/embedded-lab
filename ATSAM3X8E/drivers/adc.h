/**
 * @file adc.h
 * @brief Driver for the ADC on the ATSAM.
 */

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/**
 * @brief Sets up the ADC on channel 0.
 */
void adc_init(void);

/**
 * @return 1 if the IR beam is blocked.
 */
uint32_t adc_ir_beam_blocked(void);

#endif