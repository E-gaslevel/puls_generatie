/**
 * @file gpio.c
 *
 * @date Jan 11, 2021
 * @author marcel
 */

#include "gpio.h"

void gpio_init(){
	// Configure PULSE_EN, OPAMP_EN and ENVELOP_EN as output
	GPIO_PinModeSet (PULSE_EN_PORT, PULSE_EN_PIN, gpioModePushPull, 0);
	GPIO_PinModeSet (OPAMP_EN_PORT, OPAMP_EN_PIN, gpioModePushPull, 0);
	GPIO_PinModeSet (ENVELOP_EN_PORT, ENVELOP_EN_PIN, gpioModePushPull, 0);
	GPIO_PinModeSet (PULSE_PORT, PULSE_PIN, gpioModePushPull, 1);
	GPIO_PinModeSet (DEBUG_PORT, DEBUG_PIN, gpioModePushPull, 0);

	//Set PULSE_EN, OPAMP_EN and ENVELOP_EN  high
	GPIO_PinOutClear (PULSE_EN_PORT, PULSE_EN_PIN);
	GPIO_PinOutClear (OPAMP_EN_PORT, OPAMP_EN_PIN);
	GPIO_PinOutClear (ENVELOP_EN_PORT, ENVELOP_EN_PIN);

	//Set OUT_A, OUT_B and ENVELOP_IN to input, only ENVELOP is currently used
	GPIO_PinModeSet(OUT_A_IN_PORT, OUT_A_IN_PIN, gpioModeInput,1);
	GPIO_PinModeSet(OUT_B_IN_PORT, OUT_B_IN_PIN, gpioModeInput,1);
	GPIO_PinModeSet(ENVELOP_IN_PORT, ENVELOP_IN_PIN, gpioModeInput,1);
}


void gpio_sleep(){
/*#if defined(DEBUG) && !DEBUG
	GPIO_PinOutSet (PULSE_EN_PORT, PULSE_EN_PIN);
	GPIO_PinOutSet (OPAMP_EN_PORT, OPAMP_EN_PIN);
	GPIO_PinOutSet (ENVELOP_EN_PORT, ENVELOP_EN_PIN);
#endif*/
}

void gpio_wakeup(){
	//Set PULSE_EN, OPAMP_EN and ENVELOP_EN  high
  /*
#if defined(DEBUG) && !DEBUG
	GPIO_PinOutClear (PULSE_EN_PORT, PULSE_EN_PIN);
	GPIO_PinOutClear (OPAMP_EN_PORT, OPAMP_EN_PIN);
	GPIO_PinOutClear (ENVELOP_EN_PORT, ENVELOP_EN_PIN);
#endif*/
}
