#include "functions.h"

void serial_tx(uint8_t dat)
{
  while (USART_GetFlagStatus(USART_FLAG_TXE) == RESET)
  {
  }
  USART_SendData8(dat);
}
