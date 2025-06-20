#include "main.h"

uint8_t flags = 0;

void main(void)
{
  uint8_t chr;
  
  Init_Application();
  
  enableInterrupts();
    
  while (1)
  {
    // Check keyboard input
    checkButtonDebounced();
    
    // Check keyboard flag
    if (flags & _FL_KEY)
    {
      deinitNRF();
      LED_1_OFF();
      
      if (flags & _FL_SNIFFER)  // sniffer -> meter
      {
        initNRF_meter();
        flags &= ~_FL_SNIFFER;
        LED_2_OFF();
      }
      else                      // meter -> sniffer
      {
        initNRF_sniffer();
        flags |= _FL_SNIFFER;
        LED_2_ON();
      }
      
      flags &= ~_FL_KEY;
    }

    // Check device mode - sniffer or meter
    if (flags & _FL_SNIFFER)    // sniffer mode
    {
      CSN_LOW();
      chr = nrf_send_read(cmdNOP);
      CSN_HIGH();
      if(chr&0x40)
      {
        CSN_LOW();
        nrf_send_read(cmdWRITE_REG + regSTATUS);
        nrf_send_read(1<<RX_DR | 1<<TX_DS | 1<<MAX_RT | 1<<TX_FULL);
        CSN_HIGH();
      }
      if((chr&0x0E) != 0x0E)
      {
        serial_tx(252);	// frame marker
        serial_tx('S');	// packet type
        serial_tx(chr);
        CSN_LOW();
        nrf_send_read(cmdRD_RX_PLOAD);
        chr = nrf_send_read(cmdNOP);
        serial_tx(chr);
        chr = nrf_send_read(cmdNOP);
        serial_tx(chr);
        chr = nrf_send_read(cmdNOP);
        serial_tx(chr);
        chr = nrf_send_read(cmdNOP);
        serial_tx(chr);
        CSN_HIGH();
        for(uint8_t i=0; i<26; i++)
          serial_tx(0);
        LED_1_TOG();
      }
    }
    else        // meter mode
    {
      CSN_LOW();
      chr = nrf_send_read(cmdNOP);
      CSN_HIGH();
      if(chr & (1<<RX_DR | 1<<TX_DS | 1<<MAX_RT | 1<<TX_FULL))
      {
        CSN_LOW();
        nrf_send_read(cmdWRITE_REG + regSTATUS);
        nrf_send_read(1<<RX_DR | 1<<TX_DS | 1<<MAX_RT | 1<<TX_FULL);
        CSN_HIGH();
      }
      if ((chr&0x0E) == 0x02)   // Payload for pipe 1 - command
      {
        Init_Payload();
      }
      else if ((chr&0x0E) != 0x0E)
      {
        serial_tx(252);	// frame marker
        CSN_LOW();
        nrf_send_read(cmdRD_RX_PLOAD);
        for(uint8_t i=0; i<32; i++)
          serial_tx(nrf_send_read(cmdNOP));
        CSN_HIGH();
        LED_1_TOG();
      }
    }

    // Check usart rx buffer
    if (RING_GetCount() > 0)
    {
      Parser();
    }
  }     // while (1)
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
