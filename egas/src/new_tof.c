/*
 * new_tof.c
 *
 *  Created on: 5 Jan 2026
 *      Author: timos
 */

#include <stdint.h>

#include "new_tof.h"


float find_tof (uint16_t *_data, int threshold, float step, int N)
{
  uint32_t avg_value = 0;
  for (int i = 0; i < N; i++){
      avg_value += _data[i];
  }
  avg_value = avg_value / N;
  threshold += avg_value;

  int peaks[3];
  int peak_count;
  int in_peak = 0;
  int peak_max = 0;
  int peak_index = 0;

  for (int i = 0; i < N; i++)
    {
      if (peak_count == 3){
          break;
      }
      if (in_peak == 0 && _data[i] > threshold)
        {
          in_peak = 1;
          peak_max = _data[i];
          peak_index = i;
        }
      else if (in_peak && _data[i] > threshold)
        {
          if (_data[i] > threshold)
            {
              peak_max = _data[i];
              peak_index = i;
            }
        }
      else if (in_peak && _data[i] <= threshold)
        {
          peaks[peak_count] = (peak_index * step);
          in_peak = 0;
          peak_max = 0;
          peak_index = 0;
          peak_count++;
        }
    }
  return (peaks[2] - peaks[1]) * step;
}
