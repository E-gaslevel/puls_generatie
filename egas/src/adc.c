/*
 * adc.c
 *
 *  Created on: 25 sep 2025
 *      Author: Dave Bosman
 */

#include "adc.h"
#include "em_adc.h"
#include "em_cmu.h"

void EGAS_ADC_Init(void) {

  CMU_ClockEnable(cmuClock_ADC0, true); // Enable ADC0 clock

  // Initialize initialization struct with default values
  ADC_Init_TypeDef ADC_Init_Struct = ADC_INIT_DEFAULT;
  ADC_Init_Struct.prescale        = ADC_PrescaleCalc(ADC_CLK_FREQ, 0); // ADC clock prescaler, let em_lib calculate automatically
  ADC_Init_Struct.timebase        = ADC_TimebaseCalc(0);               // ADC clock, let em_lib calculate automatically
  ADC_Init_Struct.warmUpMode      = adcWarmupKeepADCWarm;              // Keep ADC warm (uses more power)
  ADC_Init_Struct.em2ClockConfig  = adcEm2Disabled;                    // ADC turns off in Energy Mode 2
  ADC_Init_Struct.ovsRateSel      = adcOvsRateSel2;                    // Lowest over sampling for maximum sample rate
  ADC_Init_Struct.tailgate        = false;                             // Only relevant for scan mode

  ADC_InitSingle_TypeDef ADC_InitSingle_Struct = ADC_INITSINGLE_DEFAULT;
  ADC_InitSingle_Struct.acqTime         = adcAcqTime1;          // Fastest acquisition time
  ADC_InitSingle_Struct.diff            = false;                // Disable differential mode
  ADC_InitSingle_Struct.fifoOverwrite   = false;                // Don't overwrite FIFO buffer
  ADC_InitSingle_Struct.leftAdjust      = false;                // Don't left adjust
  ADC_InitSingle_Struct.negSel          = adcNegSelVSS;         // GND
  ADC_InitSingle_Struct.posSel          = adcPosSelAPORT1YCH21; // PF5
  ADC_InitSingle_Struct.prsEnable       = false;                // Don't start via Peripheral Reflex System
  ADC_InitSingle_Struct.prsSel          = false;                // Only relevant if prsEnable is on
  ADC_InitSingle_Struct.reference       = adcRef1V25;           // 1,25V reference
  ADC_InitSingle_Struct.rep             = true;                 // Enable repeat mode
  ADC_InitSingle_Struct.resolution      = adcRes12Bit;          // Use 12 bit resolution (max)
  ADC_InitSingle_Struct.singleDmaEm2Wu  = false;                // Don't use DMA

  // Initialize ADC0 peripheral
  ADC_Init(ADC0, &ADC_Init_Struct);
  ADC_InitSingle(ADC0, &ADC_InitSingle_Struct);

  ADC_Start(ADC0, adcStartSingle);
}

void EGAS_ADC_Measure(uint16_t *buffer, uint16_t size){

    for (int i = 0; i < size; i++)
    {
        while ((ADC0->STATUS & ADC_STATUS_SINGLEDV) != ADC_STATUS_SINGLEDV);
        buffer[i] = ADC_DataSingleGet(ADC0);// * 3440 / 4096;
    }

}

