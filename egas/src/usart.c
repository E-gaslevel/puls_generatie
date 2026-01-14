/*
 * usart.c
 *
 *  Created on: 3 oct 2025
 *      Author: krzys
 */

#include "usart.h"
#include <stdlib.h>

// Match baudrate to UART baudrate
const uint32_t BAUDRATE = 115200;

// ----------------------------------------------------------------
// UART is initialized as USART0 using PC8 and PC9
// -----------------------------------------------------------------
void EGAS_UART_Init ()
{
  // Enable clock
  CMU_ClockEnable (cmuClock_USART0, true);

  // Init UART and it's parameters
  USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
  initAsync.baudrate = BAUDRATE;
  USART_InitAsync (USART0, &initAsync);

  // Route RX, TX to PC8, PC9
  USART0->ROUTEPEN |= USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
  USART0->ROUTELOC0 = (USART0->ROUTELOC0
      & ~(_USART_ROUTELOC0_TXLOC_MASK | _USART_ROUTELOC0_RXLOC_MASK))
      | USART_ROUTELOC0_TXLOC_LOC14 | USART_ROUTELOC0_RXLOC_LOC12;
}

// ----------------------------------------------------------------
// This functions send full array with UART. All values are parsed to char and sent
// At the end, \n is send to let other program now that the data transfer is finished
// -----------------------------------------------------------------
void EGAS_UART_Send (uint32_t *_data, int size)
{
  char buffer[16];
  for (int i = 0; i < size; i++)
    {
      int len = snprintf (buffer, sizeof(buffer), "%u\n", _data[i]);
      for (int j = 0; j < len; j++)
        {
          while (!(USART0->STATUS & USART_STATUS_TXBL));  // wait for TX
          USART_Tx (USART0, buffer[j]);
        }
    }

  while (!(USART0->STATUS & USART_STATUS_TXBL))
    ;
  USART_Tx (USART0, '\n');
}

void EGAS_UART_Send_Float(float _data, int size)
{
    char buffer[256];
    int len = snprintf(buffer, sizeof(buffer), "%f", _data);

    for (int i = 0; i < len; i++)
    {
        while (!(USART0->STATUS & USART_STATUS_TXBL));
        USART_Tx(USART0, buffer[i]);
    }
    while (!(USART0->STATUS & USART_STATUS_TXBL));
    USART_Tx(USART0, '\n');
}

void EGAS_UART_Receive (uint16_t *_data)
{
  char buffer[6];
  char incoming_byte;
  int index_data = 0;
  uint8_t index_buffer = 0;

  do
    {
      incoming_byte = USART_Rx (USART0);
    }
  while (incoming_byte != '$');

  while (1)
    {
      incoming_byte = USART_Rx (USART0);

      if (incoming_byte == '$')
        {
          break;
        }

      if (incoming_byte != '\n')
        {
          if (index_buffer < (sizeof(buffer) - 1))
            {
              buffer[index_buffer++] = incoming_byte;
            }
        }
      else
        {
          buffer[index_buffer] = '\0';
          _data[index_data] += atoi (buffer);
          index_buffer = 0;
          index_data++;
        }
    }
}

// ----------------------------------------------------------------
// This function processes parameters received with UART
// There is a blocking loop with a blocking USART_RX call which will wait for bytes
// Every char will be stored in buffer
// Then sscanf will put the variables from the string into variables, and then the array
// -----------------------------------------------------------------
  void EGAS_UART_Receive_Params (uint32_t *params)
  {
    char buffer[32];
    uint8_t buffer_index = 0;
    char incoming_byte;

    uint32_t frequency, duty_cycle, n_pulses;

    // Put whole message into string buffer, put \0 at the end
    // No check needed for overflow etc, because length is predefined in the transceiver
    while (1)
      {
        incoming_byte = USART_Rx (USART0);
        if (incoming_byte != '\n')
          {
            buffer[buffer_index] = incoming_byte;
            buffer_index++;
          }
        else
          {
            buffer[buffer_index] = '\0';
            break;
          }
      }

    int values_parsed = sscanf (buffer, "f%lu" "d%lu" "n%lu", &frequency,
                                &duty_cycle, &n_pulses);
    if (values_parsed == 3)
      {
        params[0] = frequency;
        params[1] = duty_cycle;
        params[2] = n_pulses;
      }
  }
