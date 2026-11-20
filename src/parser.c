#include "functions.h"

uint8_t symbol;
static uint8_t p_cnt = 0;   // pending paramters (0 = command mode)
static uint8_t p_idx;       // payload index

void Parser(void)
{
  while (RING_GetCount() > 0)
  {
    symbol = RING_Pop();

    /* receive parameters after command 'p' */
    if (p_cnt > 0)
    {
      payload[p_idx++] = symbol;
      if (--p_cnt == 0)
      {
        /* all parameters received: payload[2..5]
           any additional processing */
        CSN_LOW();
        nrf_send_read(cmdW_ACK_PAYLOAD | 0);      // Payload for pipe 0
        for (uint8_t i=0; i<16; i++) nrf_send_read(payload[i]);
        CSN_HIGH();
      }
      continue;   // parameters are not commands
    }
    
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

      case 'p':
        payload[1] = '@';
        p_idx = 2;    // first index in payload = 2
        p_cnt = 4;    // number of parameters = 4
        break;
    }
  }
}
