/*
 * timer.h
 *
 *  Created on: 9 oct 2025
 *      Author: krzys
 */

#ifndef EGAS_INC_TIMER_H_
#define EGAS_INC_TIMER_H_

#include "em_cmu.h"
#include "em_timer.h"
#include "em_device.h"

#include "gpio.h"

void EGAS_PWM_Init(void);
void EGAS_PWM_Start(uint32_t pwm_freq, uint8_t duty_cycle, uint8_t n_of_pulses);

#endif /* EGAS_INC_TIMER_H_ */
