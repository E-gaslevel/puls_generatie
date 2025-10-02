/**
 * @file adc.c
 *
 * @date Jan 11, 2021
 * @author marcel
 */

#include "adc.h"
#include "gpio.h"

#include "em_adc.h"
#include "em_cmu.h"

#include "app_assert.h"

void adc_init(void)
{
  // Enable ADC0 clock
  CMU_ClockEnable(cmuClock_ADC0, true);

  // Declare init structs
  ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
  ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

  // Modify init structs and initialize
  init.prescale = ADC_PrescaleCalc(ADC_FREQ, 0); // Init to max ADC clock for Series 1
  init.warmUpMode = adcWarmupKeepADCWarm;

  initSingle.diff       = false;        // single ended
  initSingle.reference  = adcRefVDD;    // internal Vdd reference
  initSingle.resolution = adcRes12Bit;  // 12-bit resolution
  initSingle.acqTime    = adcAcqTime1;  // set acquisition time to meet minimum requirement
  initSingle.rep        = true;         // Continuous conversion

  // Select ADC input. See README for corresponding EXP header pin.
  initSingle.posSel = adcPosSelAPORT1YCH21; //PF5
  //initSingle.posSel = adcPosSelAPORT1YCH23; //PF7
  init.timebase = ADC_TimebaseCalc(0);

  ADC_Init(ADC0, &init);
  ADC_InitSingle(ADC0, &initSingle);

  // Start ADC conversion
  ADC_Start(ADC0, adcStartSingle);
}

void adc_measure(uint16_t *buffer, uint16_t size){
    app_assert(size == sampleCount, "The buffer size is incorrect!");

    GPIO_PinOutSet(DEBUG_PORT, DEBUG_PIN);
    for (int i = 0; i < size; i++)
    {
        //while ((ADC0->STATUS & ADC_STATUS_SINGLEDV) != ADC_STATUS_SINGLEDV);
        buffer[i] = ADC_DataSingleGet(ADC0);// * 3440 / 4096;
    }
    GPIO_PinOutClear(DEBUG_PORT, DEBUG_PIN);
}
