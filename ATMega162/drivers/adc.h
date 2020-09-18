#ifndef ADC_H_
#define ADC_H_

#include <stdio.h>

#define ADC_JOYSTICK_X_CHANNEL			0b10000000
#define ADC_JOYSTICK_Y_CHANNEL			0b10000001
#define ADC_LEFT_SLIDER_CHANNEL			0b10000010
#define ADC_RIGHT_SLIDER_CHANNEL		0b10000011

/**
 * @brief Sets up a clock signal on PB0 with CTC mode for the ADC with a frequency of ~1.2 MHz.  
 */
void adc_init();

/**
 * @brief Grabs the output on a given channel of the ADC. 
 * 
 * @param channel The channel to read.
 * 
 * @return Digital value of the analog channel. 
 */
uint8_t adc_read(uint8_t channel);

#endif