/*
 * usart.c
 *
 *  Created on: 3 oct 2025
 *      Author: krzys
 */

#include "usart.h"

const uint16_t BAUDRATE = 9600;


void EGAS_UART_Init()
{
  // Enable clock
  CMU_ClockEnable(cmuClock_USART0, true);

  // Init UART and it's parameters
  USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
  initAsync.baudrate = BAUDRATE;
  USART_InitAsync(USART0, &initAsync);

  // Route RX, TX to PC8, PC9
  USART0->ROUTEPEN |= USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
  USART0->ROUTELOC0 = (USART0->ROUTELOC0
                        & ~(_USART_ROUTELOC0_TXLOC_MASK
                        | _USART_ROUTELOC0_RXLOC_MASK))
                        | USART_ROUTELOC0_TXLOC_LOC14
                        | USART_ROUTELOC0_RXLOC_LOC12;
}

void EGAS_UART_Send(uint16_t *_data, int size)
{
  char buffer[16];
  for (int i = 0; i < size; i++) {
    int len = snprintf(buffer, sizeof(buffer), "%u,", _data[i]);
    for (int j = 0; j < len; j++) {
      while (!(USART0->STATUS & USART_STATUS_TXBL));  // wait for TX
      USART_Tx(USART0, buffer[j]);
    }
  }

  // optional newline at the end
  while (!(USART0->STATUS & USART_STATUS_TXBL));
  USART_Tx(USART0, '\n');
}
