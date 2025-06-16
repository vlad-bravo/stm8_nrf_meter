#include "main.h"

void serial_tx(uint8_t dat)
{
  while (USART_GetFlagStatus(USART_FLAG_TXE) == RESET)
  {
  }
  USART_SendData8(dat);
}

void main(void)
{
  uint8_t chr;
  
  Init_Application();
    
  serial_tx('O');
  serial_tx('k');
  serial_tx(10);
    
  while (1)
  {
    CSN_LOW();
    chr = nrf_send_read(cmdNOP);
    CSN_HIGH();
    if(chr&0x40)
    {
      CSN_LOW();
      nrf_send_read(cmdWRITE_REG + regSTATUS);
      nrf_send_read(1<<RX_DR | 1<<TX_DS | 1<<MAX_RT);
      CSN_HIGH();
    }
    if((chr&0x0E) != 0x0E)
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
