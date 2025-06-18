#include "functions.h"

void Init_SPIFullDuplex(void)
{
    GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);   // CE
    GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);   // CSN
	  /*Set the MOSI,MISO and SCK at high level*/
    GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, ENABLE);
    SPI_DeInit();
    SPI_Init(SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2, SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft);

    SPI_Cmd(ENABLE);
}

void Init_USART(void)
{
  USART_DeInit();
  USART_Init(625000, USART_WordLength_8D, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx | USART_Mode_Tx));  // BaudRate value should be < 625000 bps
  
  USART_Cmd(ENABLE);
}

void Init_GPIO(void)
{
    GPIO_Init(LED_1_PORT, LED_1_PIN, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(LED_2_PORT, LED_2_PIN, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(NRF_IRQ_PORT, NRF_IRQ_PIN, GPIO_Mode_In_PU_No_IT);   // NRF_IRQ
    GPIO_Init(KEY_PORT, KEY_PIN, GPIO_Mode_In_PU_No_IT);   // K1
}

void Init_Clock(void)
{
    CLK_PeripheralClockConfig((CLK_PCKENR_USART | CLK_Peripheral_TIM2 | CLK_Peripheral_TIM3 | CLK_Peripheral_AWU | CLK_Peripheral_SPI), ENABLE);
	  /* Select fCPU = 16MHz */
    CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv1);
}

/** Initialize the application.
  * @param[in] None
  * @return None
  **/
void Init_Application(void)
{
    Init_Clock();
    Init_GPIO();
    Init_SPIFullDuplex();
    Init_USART();
    deinitNRF();
    initNRF_meter();
    delay(10000);
    initNRF_meter();
}

void delay(unsigned long int n)
{
  while (n-- > 0);
}
