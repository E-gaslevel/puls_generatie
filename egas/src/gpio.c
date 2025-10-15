/*
 * gpio.c
 *
 *  Created on: 3 oct 2025
 *      Author: krzysztof
 */

#include "gpio.h"

void EGAS_GPIO_Init()
{
  // Enable clock
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Set Pinmode needed for RX, TX
  GPIO_PinModeSet(gpioPortC, 8, gpioModePushPull, 0); //RX
  GPIO_PinModeSet(gpioPortC, 9, gpioModePushPull, 0); //TX
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


