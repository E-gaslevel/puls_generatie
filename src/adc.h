/**
 * @file adc.h
 *
 * @date Jan 11, 2021
 * @author marcel
 */

#ifndef SRC_ADC_H_
#define SRC_ADC_H_

#include <stdint.h>

#define ADC_FREQ   16000000                    //fastest possible ADC speed
#define sampleCount 5000		               //amount of analog samples to be taken

void adc_init(void);
void adc_measure(uint16_t *buffer, uint16_t size);

#endif /* SRC_ADC_H_ */
