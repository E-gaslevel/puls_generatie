/*
 * usart.h
 *
 *  Created on: 3 oct 2025
 *      Author: krzys
 */

#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <stdio.h>

#include "em_cmu.h"
#include "em_usart.h"

void EGAS_UART_Init();
void EGAS_UART_Send(uint16_t *_data, int size);
void EGAS_UART_Receive_Params(uint32_t* params);

#endif /* USART_H */
