/*
 * tof.h
 *
 *  Created on: 5 Jan 2026
 *      Author: timos
 */

#ifndef EGAS_INC_TOF_H_
#define EGAS_INC_TOF_H_

#include "em_cmu.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

int local_maxima(const uint16_t data[], size_t data_size, uint16_t peaks[], size_t peaks_size);

int filter_height(const uint16_t data[], size_t data_size, uint16_t peaks[], size_t peaks_size, uint16_t height);

int argsort(const uint16_t data[], uint16_t peaks[], size_t peaks_size, uint16_t peaks_order[]);

int filter_distance(const uint16_t data[], size_t data_size,
                    uint16_t peaks[], size_t peaks_size,
                    uint16_t argsortArray[], size_t distance);

#endif /* EGAS_INC_TOF_H_ */
