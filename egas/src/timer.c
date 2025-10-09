/*
 * timer.c
 *
 *  Created on: 9 okt 2025
 *      Author: Dave Bosman
 */

#include "timer.h"
#include "em_timer.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include <stdbool.h>

void EGAS_Timer_Init(void) {

  //CMU_ClockEnable(cmuClock_TIMER0, true);
  CMU_ClockEnable(cmuClock_TIMER1, true);


  // TIMER0 for counting the amount of pulses
  /*
  TIMER_Init_TypeDef timer0_Init_Struct;
  timer0_Init_Struct.enable = true;                      // Start counting when initialization completed.
  timer0_Init_Struct.debugRun = false;                   // Counter shall keep running during debug halt.
  timer0_Init_Struct.prescale = timerPrescale1;          // Prescaling factor, if HFPER / HFPERB clock used.
  timer0_Init_Struct.clkSel = timerClkSelCC1;            // Clock selection.
  timer0_Init_Struct.count2x = false;                    // 2x Count mode, counter increments/decrements by 2, meant for PWM mode.
  timer0_Init_Struct.ati = false;                        // ATI (Always Track Inputs) makes CCPOL always track the polarity of the inputs.
  timer0_Init_Struct.rssCoist = false;                   // Reload-Start Sets COIST When enabled, compare output is set to COIST value on a Reload-Start event.
  timer0_Init_Struct.fallAction = timerInputActionNone;  // Action on falling input edge.
  timer0_Init_Struct.riseAction = timerInputActionStop;  // Action on rising input edge.
  timer0_Init_Struct.mode = timerModeUp;                 // Counting mode.
  timer0_Init_Struct.dmaClrAct = false;                  // DMA request clear on active.
  timer0_Init_Struct.quadModeX4 = false;                 // Select X2 or X4 quadrature decode mode (if used).
  timer0_Init_Struct.oneShot = false;                    // Determines if only counting up or down once.
  timer0_Init_Struct.sync = false;                       // Timer can be start/stop/reload by other timers.
  timer0_Init_Struct.disSyncOut = false;                 // Disable ability of timer to start/stop/reload other timers that have their SYNC bit set.

  // CC0 for disabling timers
  TIMER_InitCC_TypeDef timer0_InitCC_struct;
  timer0_InitCC_struct.eventCtrl = timerEventEveryEdge;  // Input capture event control.
  timer0_InitCC_struct.edge = timerEdgeRising;           // Input capture edge select.
  timer0_InitCC_struct.prsSel = 0;                       // Peripheral reflex system trigger selection.
  timer0_InitCC_struct.cufoa = timerOutputActionNone;    // Counter underflow output action.
  timer0_InitCC_struct.cofoa = timerOutputActionNone;    // Counter overflow output action.
  timer0_InitCC_struct.cmoa = timerOutputActionNone;     // Counter match output action.
  timer0_InitCC_struct.mode = timerCCModeCompare;        // Compare/capture channel mode.
  timer0_InitCC_struct.filter = false;                   // Enable digital filter.
  timer0_InitCC_struct.prsInput = false;                 // Select TIMERnCCx (false) or PRS input (true).
  timer0_InitCC_struct.coist = false;                    // Compare output initial state.
  timer0_InitCC_struct.outInvert = false;                // Invert output from compare/capture channel.
  timer0_InitCC_struct.prsOutput = timerPrsOutputDefault;// PRS output configuration.
  */
  // TIMER1 for generating the pulse
  TIMER_Init_TypeDef timer1_Init_Struct;
  timer1_Init_Struct.enable = true;                     // Start counting when initialization completed.
  timer1_Init_Struct.debugRun = false;                   // Counter shall keep running during debug halt.
  timer1_Init_Struct.prescale = timerPrescale1;          // Prescaling factor, if HFPER / HFPERB clock used.
  timer1_Init_Struct.clkSel = timerClkSelHFPerClk;       // Clock selection.
  timer1_Init_Struct.count2x = false;                    // 2x Count mode, counter increments/decrements by 2, meant for PWM mode.
  timer1_Init_Struct.ati = false;                        // ATI (Always Track Inputs) makes CCPOL always track the polarity of the inputs.
  timer1_Init_Struct.rssCoist = false;                   // Reload-Start Sets COIST When enabled, compare output is set to COIST value on a Reload-Start event.
  timer1_Init_Struct.fallAction = timerInputActionNone;  // Action on falling input edge.
  timer1_Init_Struct.riseAction = timerInputActionNone;  // Action on rising input edge.
  timer1_Init_Struct.mode = timerModeUp;                 // Counting mode.
  timer1_Init_Struct.dmaClrAct = false;                  // DMA request clear on active.
  timer1_Init_Struct.quadModeX4 = false;                 // Select X2 or X4 quadrature decode mode (if used).
  timer1_Init_Struct.oneShot = false;                    // Determines if only counting up or down once.
  timer1_Init_Struct.sync = false;                        // Timer can be start/stop/reload by other timers.
  timer1_Init_Struct.disSyncOut = false;                 // Disable ability of timer to start/stop/reload other timers that have their SYNC bit set.

  // TIMER1 CC0 for pulse width
  TIMER_InitCC_TypeDef timer1_InitCC_struct;
  timer1_InitCC_struct.eventCtrl = timerEventEveryEdge;  // Input capture event control.
  timer1_InitCC_struct.edge = timerEdgeRising;           // Input capture edge select.
  timer1_InitCC_struct.prsSel = 0;                       // Peripheral reflex system trigger selection.
  timer1_InitCC_struct.cufoa = timerOutputActionNone;    // Counter underflow output action.
  timer1_InitCC_struct.cofoa = timerOutputActionNone;    // Counter overflow output action.
  timer1_InitCC_struct.cmoa = timerOutputActionNone;     // Counter match output action.
  timer1_InitCC_struct.mode = timerCCModePWM;        // Compare/capture channel mode.
  timer1_InitCC_struct.filter = false;                   // Enable digital filter.
  timer1_InitCC_struct.prsInput = false;                 // Select TIMERnCCx (false) or PRS input (true).
  timer1_InitCC_struct.coist = false;                    // Compare output initial state.
  timer1_InitCC_struct.outInvert = false;                // Invert output from compare/capture channel.
  timer1_InitCC_struct.prsOutput = timerPrsOutputDefault;// PRS output configuration.

  // Init timer 0
  /*
  TIMER_InitCC(TIMER0, 0, &timer0_InitCC_struct);
  TIMER_Init(TIMER0, &timer0_Init_Struct);
  */
  // Connect timer to pin
  TIMER1->ROUTELOC0 = TIMER_ROUTELOC0_CC0LOC_LOC15;
  TIMER1->ROUTEPEN = TIMER_ROUTEPEN_CC0PEN;

  // Init Timer 1
  TIMER_TopBufSet(TIMER1, 38);
  //TIMER_CounterSet(TIMER1, 5000);
  TIMER_CompareBufSet(TIMER1, 0, 19);
  TIMER_InitCC(TIMER1, 0, &timer1_InitCC_struct);
  TIMER_Init(TIMER1, &timer1_Init_Struct);

  //TIMER_Enable(TIMER1, true);


}
