#include "adc.h"

#include <stdint.h>

#include "sam.h"

// Uses the AD0 signal pin (PA2), which uses channel 0
#define CHANNEL0 0

#define THRESHOLD 1500

/**
 * @brief Reads the ADC register.
 */
static uint32_t adc_read(void) { return ADC->ADC_CDR[CHANNEL0]; }

void adc_init(void) {
    // Enable the clock for ADC
    PMC->PMC_PCER1 |= PMC_PCER1_PID37;

    // Set freerun mode
    ADC->ADC_MR |= ADC_MR_FREERUN_ON;

    // Enable Channel 0. Automatically selects ADC (extra function) for the multiplexed PA2 pin
    ADC->ADC_CHER |= ADC_CHER_CH0;

    // Begin conversion
    ADC->ADC_CR |= ADC_CR_START;
}

uint32_t adc_ir_beam_blocked(void) { return adc_read() < THRESHOLD ? 1 : 0; }
