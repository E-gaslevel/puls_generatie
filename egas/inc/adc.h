/*
 * adc.h
 *
 *  Created on: 25 sep 2025
 *      Author: bossie
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#define ADC_CLK_FREQ 16000000UL

#include <stdint.h>

void EGAS_ADC_Init(void);
void EGAS_ADC_Measure(uint16_t *buffer, uint16_t size);

#endif /* INC_ADC_H_ */
