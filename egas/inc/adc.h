/*
 * adc.h
 *
 *  Created on: 25 sep 2025
 *      Author: bossie
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "em_adc.h"
#include "em_cmu.h"

#define ADC_CLK_FREQ 16000000UL

void EGAS_ADC_Init(void);

#endif /* INC_ADC_H_ */
