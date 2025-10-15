/*
 * gpio.h
 *
 *  Created on: 3 oct 2025
 *      Author: krzys
 */

#ifndef GPIO_H
#define GPIO_H

#include "em_cmu.h"
#include "em_gpio.h"


void EGAS_GPIO_Init();

#define PULSE_EN_PORT gpioPortA
#define PULSE_EN_PIN 4
#define OPAMP_EN_PORT gpioPortB
#define OPAMP_EN_PIN 11
#define ENVELOP_EN_PORT gpioPortA
#define ENVELOP_EN_PIN 5
#define PULSE_PORT gpioPortD
#define PULSE_PIN 14

#define ENVELOP_IN_PORT gpioPortF
#define ENVELOP_IN_PIN 5
#define OUT_B_IN_PORT gpioPortF
#define OUT_B_IN_PIN 6
#define OUT_A_IN_PORT gpioPortF
#define OUT_A_IN_PIN 7

#define DEBUG_PORT gpioPortD
#define DEBUG_PIN 15

#define RX_PORT gpioPortC
#define RX_PIN 8
#define RX_LOCATION 12
#define TX_PORT gpioPortC
#define TX_PIN 9
#define TX_LOCATION 14

void gpio_init(void);
void gpio_sleep(void);
void gpio_wakeup(void);

#endif
