/*
 * timer.c
 *
 *  Created on: 9 oct 2025
 *      Author: krzys
 */

#include "em_cmu.h"
#include "gpio.h"
#include "em_timer.h"
#include "em_prs.h"

#define PWM_FREQ        1000000    // 1 MHz
#define PWM_DUTY_CYCLE  50         // 50%
#define START_DELAY_MS  500        // 500 ms delay before PWM starts

void initTimersPWM(void)
{
    // -------------------------
    // Enable clocks
    // -------------------------
    //CMU_ClockEnable(cmuClock_GPIO,  true);
    //CMU_ClockEnable(cmuClock_TIMER0, true);
    CMU_ClockEnable(cmuClock_TIMER1, true);
    //CMU_ClockEnable(cmuClock_PRS,   true);

    // -------------------------
    // TIMER0 PWM output (PA0)
    // -------------------------
    GPIO_PinModeSet(gpioPortC, 10, gpioModePushPull, 0);
    TIMER1->ROUTELOC0 = TIMER_ROUTELOC0_CC0LOC_LOC15; // check datasheet for correct LOC
    TIMER1->ROUTEPEN  = TIMER_ROUTEPEN_CC0PEN;

    // -------------------------
    // Configure TIMER0 for PWM
    // -------------------------
    uint32_t timer0Freq = CMU_ClockFreqGet(cmuClock_TIMER1);
    uint32_t topValue   = (timer0Freq / PWM_FREQ) - 1;
    uint32_t compValue  = ((topValue + 1) * PWM_DUTY_CYCLE) / 100;

    TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;
    timerCCInit.mode = timerCCModePWM;
    TIMER_InitCC(TIMER1, 0, &timerCCInit);

    TIMER_TopBufSet(TIMER1, topValue);
    TIMER_CompareBufSet(TIMER1, 0, compValue);

    TIMER_Init_TypeDef timer1Init = TIMER_INIT_DEFAULT;
    //timer0Init.oneShot = true;   // one-shot: stops automatically at TOP
    timer1Init.enable  = true;
    TIMER_Init(TIMER1, &timer1Init);

    TIMER_Enable(TIMER0, true);
}



