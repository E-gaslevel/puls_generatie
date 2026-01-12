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
#define MAX_PULSES 30
#define N 1200
#define OFFSET 12.7025
#define FS 730000
#define Ms 787

uint32_t buffer[N]; // gebruik buffer buiten stack
uint32_t temp_buf[N];

void app_init(void)
{
  EGAS_GPIO_Init();
  EGAS_ADC_Init();
  EGAS_UART_Init();
  EGAS_PWM_Init();
  while(1)
  {
      for (int i = 0; i < sizeof(buffer); i++){
          buffer[i] = 0;
      }
      for (int i = 0; i < MAX_PULSES; i++){
          EGAS_PWM_Start(120000, 75, 5);
          EGAS_ADC_Measure(temp_buf, sizeof(temp_buf) / sizeof(uint32_t));
          for (int i = 0; i < N; i++){
              buffer[i] += temp_buf[i];
          }
          for(int i = 0; i < 50000; i++);
      }

      EGAS_SavGol_Filter(buffer);
      float tof_ms = (find_tof(buffer, 0.2, FS, N) * 1000);
      float tof_us = tof_ms * 1000;
      float dis = tof_ms * Ms;
      float dis_with_offset = dis - OFFSET;
      EGAS_UART_Send(buffer, sizeof(buffer) / sizeof(uint32_t));

      for(int i = 0; i < 500000; i++);
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
