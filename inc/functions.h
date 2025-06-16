/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#include "stm8l10x.h"
#include "stm8l10x_usart.h"
#include "nrf.h"
/* Help Menu Status - bit definition */
/* Bit[3..0] used as counter */
#define HELP_DISPLAY_STATUS_ENABLE                7

/* Application Status 1 - bit definition */
#define APPLICATION_STATUS_1_EXT_IT               0
#define APPLICATION_STATUS_1_RTCAWU_IT            1
#define APPLICATION_STATUS_1_PVD_IT               2
#define APPLICATION_STATUS_1_PVD_LEVEL            3
#define APPLICATION_STATUS_1_NAVIGATION_ENABLED   4
#define APPLICATION_STATUS_1_LED_ACTIVITY_ENABLED 5

/* LEDs I/Os */

#define LED_2_PORT (GPIOB)
#define LED_2_PIN  (GPIO_Pin_0)

#define LED_1_PORT (GPIOD)
#define LED_1_PIN  (GPIO_Pin_0)


/* Exported macro ------------------------------------------------------------*/
#define LED_1_OFF()       GPIO_SetBits(LED_1_PORT, LED_1_PIN)
#define LED_1_ON()        GPIO_ResetBits(LED_1_PORT, LED_1_PIN)
#define LED_1_TOG()       GPIO_ToggleBits(LED_1_PORT, LED_1_PIN)

#define LED_2_OFF()       GPIO_SetBits(LED_2_PORT, LED_2_PIN)
#define LED_2_ON()        GPIO_ResetBits(LED_2_PORT, LED_2_PIN)
#define LED_2_TOG()       GPIO_ToggleBits(LED_2_PORT, LED_2_PIN)

#define CSN_HIGH()       GPIO_SetBits(GPIOB, GPIO_Pin_4)
#define CSN_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_4)

#define CE_HIGH()       GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define CE_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_2)

/* Exported variables --------------------------------------------------------*/
extern volatile uint32_t GTimeStamp;
extern uint16_t TimeOutStartTime;

/* Exported functions --------------------------------------------------------*/
void Init_Application(void);
void Init_SPIFullDuplex(void);
void Init_GPIO(void);
void Init_Clock(void);
uint32_t GetTimeStamp(void);
void GetTimeOutStartTime(void);
bool TimeOut(uint16_t Delay);
void delay(unsigned long int);

#endif /* __FUNCTIONS_H */
