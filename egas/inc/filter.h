/*
 * filter.h
 *
 *  Created on: 5 Jan 2026
 *      Author: timos
 */

#ifndef EGAS_INC_FILTER_H_
#define EGAS_INC_FILTER_H_

#include <stdint.h>
#include "em_cmu.h"

void EGAS_SavGol_Filter(uint32_t* _data);

#endif /* EGAS_INC_FILTER_H_ */
