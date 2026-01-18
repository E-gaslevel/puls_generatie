/*
 * new_tof.c
 *
 *  Created on: 5 Jan 2026
 *      Author: timos
 */

#include <stdint.h>

#include "new_tof.h"

//#define offset 12.75
//#define offset 12.7025

float find_tof (uint32_t *_data, float threshold, float fs, int N)
{
//  int avg_offset = 180;
//  uint32_t avg_value = 0;
//  for (int i = avg_offset; i < N; i++)
//    {
//      avg_value += _data[i];
//    }
//  avg_value = avg_value / (N - avg_offset);
//  threshold = (avg_value * (1+threshold));

  int peaks[3];
  int peak_count = 0;
  int in_peak = 0;
  int peak_max = 0;
  int peak_index = 0;

  for (int i = 0; i < N; i++)
    {
      if (peak_count == 3)
        {
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
          if (_data[i] > peak_max)
            {
              peak_max = _data[i];
              peak_index = i;
            }
        }
      else if (in_peak && _data[i] <= threshold)
        {
          peaks[peak_count] = peak_index;
          in_peak = 0;
          peak_max = 0;
          peak_index = 0;
          peak_count++;
        }
    }
  if (peak_count == 3)
    {
      int samples_between_peaks = (peaks[2] - peaks[1]);
      return (samples_between_peaks * (1/fs)) / 2;
    }
  else
    {
      return 0;
    }

}
