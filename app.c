/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

#include <stdint.h>

#include "app.h"

uint16_t buffer[5000]; // gebruik buffer buiten stack

void app_init(void)
{
  EGAS_GPIO_Init();
  EGAS_ADC_Init();
  EGAS_ADC_Measure(buffer, sizeof(buffer) / sizeof(uint16_t));
  EGAS_PWM_Init();
  //EGAS_PWM_Start();
  EGAS_UART_Init();
  EGAS_UART_Send(buffer, sizeof(buffer) / sizeof(uint16_t));
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
