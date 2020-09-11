#ifndef ADC_H_
#define ADC_H_

#include <stdio.h>

#define ADC_JOYSTICK_X_CHANNEL			0b10000000
#define ADC_JOYSTICK_Y_CHANNEL			0b10000001
#define ADC_LEFT_SLIDER_CHANNEL			0b10000010
#define ADC_RIGHT_SLIDER_CHANNEL		0b10000011


void adc_init();
uint8_t adc_read(uint8_t channel);

#endif