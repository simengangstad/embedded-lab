#include "pwm.h"

#include "sam.h"

void pwm_init(void) {	

    // Disable PIO PC18
	PIOC->PIO_PDR |= PIO_PDR_P18;

    // PC19 is pin 45 on the shield and the arduino
    PIOC->PIO_ABSR |= PIO_ABSR_P18;

    // Since PC18 is of instance 13, we have to enable that clock and enable the clock for PWM as well
    PMC->PMC_PCER0 |= PMC_PCER0_PID13;
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;

	// Set the clock to 4 MHz
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);

	// Set to dual slope, and use CLKA
    PWM->PWM_CH_NUM[6].PWM_CMR = PWM_CMR_CALG | PWM_CMR_CPRE_CLKA;

    // Set channel period, CPRD = 20000 to obtain 20ms
	// in the equation (2 * CPRD * DIVA) / MCK (on page 1048 in the 
	// data sheet).
    PWM->PWM_CH_NUM[6].PWM_CPRD = CHANNEL_DUTY_CYCLE_MAX;

	PWM->PWM_ENA |= PWM_ENA_CHID6;
	
	pwm_set_duty_cycle(SERVO_MID_POINT_DUTY_CYCLE);
}

void pwm_set_duty_cycle(float percent) {
	
	// We have to do 100 - percent here since the signal is active low
    uint32_t duty_cycle = (uint32_t)(CHANNEL_DUTY_CYCLE_MAX * ((100.0f - percent) / 100.0f));
	PWM->PWM_CH_NUM[6].PWM_CDTY = duty_cycle;
}