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
#define MAX_PULSES 30 // Picked 30, because of (almost) no time restrictions can be any number
#define N 1200 // Amount of values per sample
#define FS 730000 // Samplefrequency ADC on E-gaslevel, Page "x" of as-built
#define INITIAL_PULSE_WIDTH 117 // Amount of samples take make up the initial pulse

uint32_t buffer[N];
uint32_t temp_buf[N];

int find_max_buffer(uint32_t* _data){
  int highest_value = 0;
  for (int i = INITIAL_PULSE_WIDTH; i < N; i++){
      if (_data[i] > highest_value){
          highest_value = _data[i];
      }
  }
  return highest_value;
}

int find_min_buffer(uint32_t* _data){
  int lowest_value = _data[0];
  for (int i = 0; i < N; i++){
      if (_data[i] < lowest_value){
          lowest_value = _data[i];
      }
  }
  return lowest_value;
}

void app_init(void)
{
  EGAS_GPIO_Init();
  EGAS_ADC_Init();
  EGAS_UART_Init();
  EGAS_PWM_Init();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  for (int i = 0; i < sizeof(buffer); i++){
      buffer[i] = 0;
  }
  for (int i = 0; i < MAX_PULSES; i++){
      EGAS_PWM_Start(165000, 75, 6);
      EGAS_ADC_Measure(temp_buf, sizeof(temp_buf) / sizeof(uint32_t));
      for (int i = 0; i < N; i++){
          buffer[i] += temp_buf[i];
      }
      for(int i = 0; i < 50000; i++);
  }
  int min = find_min_buffer(buffer);
  int max = find_max_buffer(buffer);

  float threshold = 0.05*max + min;

  EGAS_SavGol_Filter(buffer);
  float tof_ms = (find_tof(buffer, threshold, FS, N) * 1000);
  float tof_us = tof_ms * 1000;
  EGAS_UART_Send(buffer, sizeof(buffer) / sizeof(uint32_t));

  for(int i = 0; i < 500000; i++);
}
