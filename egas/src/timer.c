/*
 * timer.c
 *
 *  Created on: 9 oct 2025
 *      Author: krzys
 */

#include "timer.h"

#define PWM_FREQ        1000000    // 1 MHz
#define PWM_DUTY_CYCLE  50           // 50%
#define N_PULSES        5

// ----------------------------------------------------------------
// This function sets two timers to send a specific amount of pwm pulses with a given frequency and duty cycle
// TIMER0 generates PWM signal, topvalue is amount of clockcyckles ran at specific rate, for 1 Mhz it's 37 clockcycles.
// Compare set is used to set a duty cycle
// TIMER0 is synced, so TIMER1 can start and stop it
//
// TIMER1 doesn't use CC, because it works in oneShot mode.
// TIMER1 will start both timers, count to its topvalue and stop both timers, without CPU.
// -----------------------------------------------------------------
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
  t0Init.sync = true;
  TIMER_Init(TIMER0, &t0Init);

  uint32_t hfperFreq = CMU_ClockFreqGet(cmuClock_TIMER0);
  uint32_t topValue = hfperFreq / PWM_FREQ - 1;
  TIMER_TopSet(TIMER0, topValue);

  // PWM duty cycle using CC0
  TIMER_InitCC_TypeDef t0ccInit = TIMER_INITCC_DEFAULT;
  t0ccInit.mode = timerCCModePWM;
  TIMER_InitCC(TIMER0, 0, &t0ccInit);
  TIMER_CompareSet(TIMER0, 0, (topValue * PWM_DUTY_CYCLE) / 100);

  TIMER0->ROUTELOC0 = TIMER_ROUTELOC0_CC0LOC_LOC15; // PC10
  TIMER0->ROUTEPEN  = TIMER_ROUTEPEN_CC0PEN;

  TIMER_Init_TypeDef t1Init = TIMER_INIT_DEFAULT;
  t1Init.enable = false;
  t1Init.prescale = timerPrescale1;
  t1Init.mode = timerModeUp;
  t1Init.oneShot = true;
  TIMER_Init(TIMER1, &t1Init);
  TIMER_TopSet(TIMER1, topValue * (N_PULSES+1));

  TIMER1->CMD = TIMER_CMD_START;
}



