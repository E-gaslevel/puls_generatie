/*
 * gpio.c
 *
 *  Created on: 3 oct 2025
 *      Author: krzysztof
 */

#include "em_gpio.h"
#include "em_cmu.h"

void EGAS_GPIO_Init()
{
  // Enable clock
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Set Pinmode needed for RX, TX
  GPIO_PinModeSet(gpioPortC, 8, gpioModePushPull, 0); //RX
  GPIO_PinModeSet(gpioPortC, 9, gpioModePushPull, 0); //TX
  GPIO_PinModeSet(gpioPortC, 10, gpioModePushPull, 0); //pwm
}


