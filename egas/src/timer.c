/*
 * timer.c
 *
 *  Created on: 9 oct 2025
 *      Author: krzys
 */

#include "timer.h"

#define PWM_FREQ        1000000    // 1 MHz
#define PWM_DUTY_CYCLE  50         // 50%
#define N_PULSES        4

uint32_t topValue;

void TIMER1_IRQHandler(void)
{
  TIMER_IntClear(TIMER1, TIMER_IF_CC0);
  TIMER0->CMD = TIMER_CMD_STOP;
  TIMER1->CMD = TIMER_CMD_STOP;
}

void EGAS_PWM_Init(void)
{
  // -----------------------------
  // Clock setup
  // -----------------------------
  CMU_ClockEnable(cmuClock_TIMER0, true);
  CMU_ClockEnable(cmuClock_TIMER1, true);

  // -----------------------------
  // TIMER0 — PWM Generator
  // -----------------------------
  TIMER_Init_TypeDef t0Init = TIMER_INIT_DEFAULT;
  t0Init.enable = false;
  t0Init.prescale = timerPrescale1;
  t0Init.mode = timerModeUp;
  TIMER_Init(TIMER0, &t0Init);

  // Compute TOP for 1 MHz frequency
  uint32_t hfperFreq = CMU_ClockFreqGet(cmuClock_TIMER0);
  uint32_t topValue = hfperFreq / PWM_FREQ - 1;
  TIMER_TopSet(TIMER0, topValue);

  // PWM duty cycle using CC0
  TIMER_InitCC_TypeDef t0ccInit = TIMER_INITCC_DEFAULT;
  t0ccInit.mode = timerCCModePWM;
  TIMER_InitCC(TIMER0, 0, &t0ccInit);
  TIMER_CompareSet(TIMER0, 0, (topValue * PWM_DUTY_CYCLE) / 100);

  TIMER0->ROUTELOC0 = TIMER_ROUTELOC0_CC0LOC_LOC15; // Check datasheet for correct LOC
  TIMER0->ROUTEPEN  = TIMER_ROUTEPEN_CC0PEN;

  // -----------------------------
  // TIMER1 — Pulse Counter
  // -----------------------------
  // Configure compare to detect desired pulse count
  TIMER_InitCC_TypeDef t1ccInit = TIMER_INITCC_DEFAULT;
  t1ccInit.mode = timerCCModeCompare;
  TIMER_InitCC(TIMER1, 0, &t1ccInit);

  TIMER_Init_TypeDef t1Init = TIMER_INIT_DEFAULT;
  t1Init.enable = false;
  t1Init.prescale = timerPrescale1;
  t1Init.mode = timerModeUp;
  TIMER_Init(TIMER1, &t1Init);

  // Enable compare interrupt
  TIMER_IntEnable(TIMER1, TIMER_IEN_CC0);
  NVIC_EnableIRQ(TIMER1_IRQn);

  // Check N_PULSES, if one, small compare value because will overshoot and send 2 pulses
    if (N_PULSES < 0) return; // Wrong value
    else if (N_PULSES == 1) TIMER_CompareSet(TIMER1, 0, topValue-10);
    else TIMER_CompareSet(TIMER1, 0, (topValue * N_PULSES)-10);

  // -----------------------------
  // Start timers
  // -----------------------------

  // Start both at (almost) the same time
  TIMER0->CMD = TIMER_CMD_START;
  TIMER1->CMD = TIMER_CMD_START;
}



