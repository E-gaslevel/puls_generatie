/*
 * usart.h
 *
 *  Created on: 3 paź 2025
 *      Author: krzys
 */

#ifndef USART_H
#define USART_H

#include <stdint.h>

void EGAS_UART_Init();
void EGAS_UART_Send(uint16_t *_data, int size);

#endif /* USART_H */
