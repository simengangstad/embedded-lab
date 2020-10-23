#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void adc_init(void);
uint32_t adc_ir_beam_blocked(void);

#endif