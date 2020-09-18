#include "adc.h"
#include "external_memory.h"

#include <avr/io.h>
#include <util/delay.h>

#define ADC_OFFSET_ADDRESS				0x400

void adc_init() {
	
	DDRB |= (1 << PB0);
	
	// Set mode to CTC, clear timer on compare (comparing timer counter, TCNT0, with compare register, OCR0).
	TCCR0 |= (1 << WGM01) | (0 << WGM00);
	
	// Toggle pin OC0 (PB0) when we reach the compare value.
	TCCR0 |= (0 << COM01) | (1 << COM00);
	
	// Use clock source without prescaling
	TCCR0 |= (0 << CS02)  | (0 << CS01)  | (1 << CS00);
	
	// The compare value, will set the frequency to ~1.2 MHz according to this formula:
	// frequency = clock_frequency/(2 * prescale_factor * (1 + OCR0)). Where clock_frequency
	// is 4.9152 MHz and prescale factor is 1.
	OCR0 = 1;
}

uint8_t adc_read(uint8_t channel) {
	external_memory_write(channel, ADC_OFFSET_ADDRESS);
	_delay_ms(1);
	volatile uint8_t value = external_memory_read(ADC_OFFSET_ADDRESS);
	return value;
}
