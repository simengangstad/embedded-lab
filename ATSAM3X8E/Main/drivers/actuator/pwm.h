/*
 * pwm.h
 *
 * Created: 16.10.2020 09:13:02
 *  Author: gotvinne
 */ 


#ifndef PWM_H_
#define PWM_H_

#define PIO_WPMR_DISABLE 0x50494F00
#define CHANNEL_DUTY_CYCLE_MAX 0x00004E20
#define SERVO_MID_POINT_DUTY_CYCLE 7.5

void pwm_init(void);

void pwm_set_duty_cycle(float percent); 


#endif /* PWM_H_ */