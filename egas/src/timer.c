/*
 * timer.c
 *
 *  Created on: 9 oct 2025
 *      Author: krzys
 */

#include "timer.h"

<<<<<<< HEAD
=======
#define PWM_FREQ        50000    // 1 MHz
#define PWM_DUTY_CYCLE  100           // 50%
#define N_PULSES        1

>>>>>>> 781754b18fd20534e5efedade509c2c366452f74
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

  // PWM duty cycle using CC0
  TIMER_InitCC_TypeDef t0ccInit = TIMER_INITCC_DEFAULT;
  t0ccInit.mode = timerCCModePWM;
  t0ccInit.outInvert = true;
  TIMER_InitCC(TIMER0, 0, &t0ccInit);

  TIMER0->ROUTELOC0 |=  TIMER_ROUTELOC0_CC0LOC_LOC22;
  TIMER0->ROUTEPEN |= TIMER_ROUTEPEN_CC0PEN;

  TIMER_Init_TypeDef t1Init = TIMER_INIT_DEFAULT;
  t1Init.enable = false;
  t1Init.prescale = timerPrescale1;
  t1Init.mode = timerModeUp;
  t1Init.oneShot = true;
  TIMER_Init(TIMER1, &t1Init);
}

void EGAS_PWM_Start(uint32_t pwm_freq, uint8_t duty_cycle, uint8_t n_of_pulses)
{
  uint32_t hfperFreq = CMU_ClockFreqGet(cmuClock_TIMER0);
  uint32_t topValue = hfperFreq / pwm_freq - 1;
  TIMER_TopSet(TIMER0, topValue);

  TIMER_CompareSet(TIMER0, 0, (topValue * duty_cycle) / 100);

  TIMER_TopSet(TIMER1, topValue * (n_of_pulses+1));
  TIMER1->CMD = TIMER_CMD_START;
}



