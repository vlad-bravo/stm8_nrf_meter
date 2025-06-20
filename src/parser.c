#include "functions.h"

uint8_t symbol;

void Parser(void)
{
  while (RING_GetCount() > 0)
  {
    symbol = RING_Pop();
    
    switch (symbol)
    {
      case 'm':
        deinitNRF();
        LED_1_OFF();
        initNRF_meter();
        flags &= ~_FL_SNIFFER;
        LED_2_OFF();
        break;

      case 's':
        deinitNRF();
        LED_1_OFF();
        initNRF_sniffer();
        flags |= _FL_SNIFFER;
        LED_2_ON();
        break;
    }
  }
}
