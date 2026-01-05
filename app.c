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
#define MAX_PULSES 5
#define MAX_DATA_POINTS 1200
#define WINDOW 51
#define MAX_PEAKS (MAX_DATA_POINTS/2)
#define min_height 12000
#define min_distance 92
#define N 1200

uint16_t buffer[MAX_DATA_POINTS]; // gebruik buffer buiten stack
uint32_t params[3];
uint16_t current_sample[MAX_DATA_POINTS];
uint16_t peaks[MAX_PEAKS];
uint16_t argsorted_peaks[MAX_PEAKS];
float step = (1.078082192/2);

void app_init(void)
{
  EGAS_GPIO_Init();
  EGAS_ADC_Init();
  EGAS_UART_Init();
  EGAS_PWM_Init();
  while(1)
  {
//    EGAS_UART_Receive_Params(params);
//    EGAS_PWM_Start(params[0], params[1], params[2]);
//    EGAS_ADC_Measure(buffer, sizeof(buffer) / sizeof(uint16_t));
//    EGAS_UART_Send(buffer, sizeof(buffer) / sizeof(uint16_t));

      for (int i = 0; i < sizeof(buffer); i++){
          buffer[i] = 0;
      }

      for (int i = 0; i < MAX_PULSES; i++){
          EGAS_UART_Receive(buffer);
      }

      EGAS_SavGol_Filter(buffer);
      float tof = find_tof(buffer, 2000, step, N);
      EGAS_UART_Send_Float(tof, sizeof(float));

//
//      local_maxima(buffer, MAX_DATA_POINTS, peaks, MAX_PEAKS);
//      filter_height(buffer, MAX_DATA_POINTS, peaks, MAX_PEAKS, min_height);
//      argsort(buffer, peaks, MAX_PEAKS, argsorted_peaks);
//      filter_distance(buffer, MAX_DATA_POINTS, peaks, MAX_PEAKS, argsorted_peaks, min_distance);
//      EGAS_UART_Send(buffer, sizeof(buffer)/sizeof(uint16_t));
      for(int i = 0; i < 500000; i++);
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
