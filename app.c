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
uint32_t params[3];
int counter = 0;

void app_init(void)
{
  EGAS_GPIO_Init();
  EGAS_ADC_Init();
  EGAS_UART_Init();
  EGAS_PWM_Init();
  while(counter < 3)
    {
      EGAS_UART_Receive_Params(params);
      EGAS_PWM_Start(params[0], params[1], params[2]); //confirm block golf!
      EGAS_ADC_Measure(buffer, sizeof(buffer) / sizeof(uint16_t));
      EGAS_UART_Send(buffer, sizeof(buffer) / sizeof(uint16_t));
      for(int i = 0; i < 500000; i++);
      counter++;
    }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
