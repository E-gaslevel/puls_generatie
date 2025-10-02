/**
 * @file timer.h
 *
 * @date Jan 6, 2021
 * @author marcel
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include <stdbool.h>
#include <stdint.h>

#include "em_cryotimer.h"

#define EXT_SIGNAL_TIMER_EXPIRED_FLAG  0x01
static const uint32_t clock_freq = 38400000;    //used in timer calculations


/**************************************************************************//**
 * pulse timer
 *****************************************************************************/
void timer_pulse_init(uint32_t frequency);
void timer_pwm_pulse_init(uint32_t frequency, uint32_t dutycycle, uint8_t pulses);
bool timer_get_pulse_done(void);


/**************************************************************************//**
 * Tick tock timer
 *****************************************************************************/
uint32_t timer_get_overflow_counter(void);
void timer_tick(void);
uint32_t timer_tock(void);
void timer_set_tictoc_ms(const uint32_t time);
void timer_set_tictoc_us(void);


/**************************************************************************//**
 * Cryotimer Init.
 *****************************************************************************/
void reset_cryo_timer();
void timer_set_cryto(CRYOTIMER_Period_TypeDef timer);
void timer_go_to_sleep(void);
void timer_go_to_deep_sleep(void);
void timer_start_cryotimer(void);
void timer_stop_cryotimer(void);

#endif /* SRC_TIMER_H_ */
