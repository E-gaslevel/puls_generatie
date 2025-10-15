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

void TIMER1_IRQHandler(void);
void EGAS_PWM_Init(void);
void EGAS_PWM_Start(void);

#endif /* EGAS_INC_TIMER_H_ */
