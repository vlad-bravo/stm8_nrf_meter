/**
  ******************************************************************************
  * @file stm8l10x_gpio.c
  * @brief This file contains all the functions for the GPIO peripheral.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.0
  * @date    09/14/2009
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l10x_gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @addtogroup GPIO_Public_Functions
  * @{
  */

/**
  * @brief Deinitializes the GPIOx peripheral registers to their default reset
  * values.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @retval None
  */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
    GPIOx->ODR = GPIO_ODR_RESET_VALUE; /* Reset Output Data Register */
    GPIOx->DDR = GPIO_DDR_RESET_VALUE; /* Reset Data Direction Register */
    GPIOx->CR1 = GPIO_CR1_RESET_VALUE; /* Reset Control Register 1 */
    GPIOx->CR2 = GPIO_CR2_RESET_VALUE; /* Reset Control Register 2 */
}

/**
  * @brief Initializes the GPIOx according to the specified parameters.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @param[in] GPIO_Pin : This parameter contains the pin number, it can be one or many members
  * of the @ref GPIO_Pin_TypeDef enumeration.
  * @param[in] GPIO_Mode : This parameter can be any of the @Ref GPIO_Mode_TypeDef enumeration.
  * @retval None
  */
void GPIO_Init(GPIO_TypeDef* GPIOx,
               uint8_t GPIO_Pin,
               GPIO_Mode_TypeDef GPIO_Mode)
{
    /*----------------------*/
    /* Check the parameters */
    /*----------------------*/

    assert_param(IS_GPIO_MODE(GPIO_Mode));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    /*-----------------------------*/
    /* Input/Output mode selection */
    /*-----------------------------*/

    if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x80) != (uint8_t)0x00) /* Output mode */
    {
        if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x10) != (uint8_t)0x00) /* High level */
        {
            GPIOx->ODR |= GPIO_Pin;
        } else /* Low level */
        {
            GPIOx->ODR &= (uint8_t)(~(GPIO_Pin));
        }
        /* Set Output mode */
        GPIOx->DDR |= GPIO_Pin;
    } else /* Input mode */
    {
        /* Set Input mode */
        GPIOx->DDR &= (uint8_t)(~(GPIO_Pin));
    }

    /*------------------------------------------------------------------------*/
    /* Pull-Up/Float (Input) or Push-Pull/Open-Drain (Output) modes selection */
    /*------------------------------------------------------------------------*/

    if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x40) != (uint8_t)0x00) /* Pull-Up or Push-Pull */
    {
        GPIOx->CR1 |= GPIO_Pin;
    } else /* Float or Open-Drain */
    {
        GPIOx->CR1 &= (uint8_t)(~(GPIO_Pin));
    }

    /*-----------------------------------------------------*/
    /* Interrupt (Input) or Slope (Output) modes selection */
    /*-----------------------------------------------------*/

    if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x20) != (uint8_t)0x00) /* Interrupt or Slow slope */
    {
        GPIOx->CR2 |= GPIO_Pin;
    } else /* No external interrupt or No slope control */
    {
        GPIOx->CR2 &= (uint8_t)(~(GPIO_Pin));
    }

}

/**
  * @brief Writes data to the specified GPIO data port.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @param[in] GPIO_PortVal : Specifies the value to be written to the port output.
  * data register.
  * @retval None
  * @par Required preconditions:
  * The port must be configured in output mode.
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint8_t GPIO_PortVal)
{
    GPIOx->ODR = GPIO_PortVal;
}
/**
  * @brief Sets or clears the selected data port bit.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @param[in] GPIO_Pin: specifies the port bit to be written.
  * This parameter can be one of GPIO_Pin_TypeDef enumeration.
  * @param[in] GPIO_BitVal: specifies the desired status to be written.
  * This parameter can be one of @ref BitAction enumeration.
  * @retval None
  */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin, BitAction GPIO_BitVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_STATE_VALUE(GPIO_BitVal));

    if (GPIO_BitVal != RESET)
    {
        SetBit(GPIOx->ODR,GPIO_Pin);

    }
    else
    {
        ClrBit(GPIOx->ODR,GPIO_Pin);
    }
}


/**
  * @brief Writes high level to the specified GPIO pins.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @param[in] GPIO_Pin : Specifies the pins to be turned high to the port output.
  * data register.
  * @retval None
  * @par Required preconditions:
  * The port must be configured in output mode.
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin)
{
    GPIOx->ODR |= GPIO_Pin;
}

/**
  * @brief Writes low level to the specified GPIO pins.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @param[in] GPIO_Pin : Specifies the pins to be turned low to the port output.
  * data register.
  * @retval None
  * @par Required preconditions:
  * The port must be configured in output mode.
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin)
{
    GPIOx->ODR &= (uint8_t)(~GPIO_Pin);
}

/**
  * @brief Toggles the specified GPIO pins.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @param[in] GPIO_Pin : Specifies the pins to be toggled.
  * data register.
  * @retval None
  * @par Required preconditions:
  * The port must be configured in output mode.
  */
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;
}

/**
  * @brief Reads the specified GPIO input data port.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @retval uint8_t : GPIO input data port value.
  * @par Required preconditions:
  * The port must be configured in input mode.
  */
uint8_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
    return ((uint8_t)GPIOx->IDR);
}
/**
  * @brief Reads the specified GPIO output data port.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @retval uint8_t : GPIO output data port value.
  * @par Required preconditions:
  * The port must be configured in input mode.
  */
uint8_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
    return ((uint8_t)GPIOx->ODR);
}
/**
  * @brief Reads the specified GPIO input data pin.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @param[in] GPIO_Pin : This parameter contains the pin number, it can be one member
  * of the @ref GPIO_Pin_TypeDef enumeration.
  * @retval BitStatus : GPIO input pin status.
	* This parameter can be any of the @ref BitStatus enumeration.
  * @par Required preconditions:
  * The port must be configured in Output mode.
  */
BitStatus GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin)
{
    return ((BitStatus)(GPIOx->IDR & (uint8_t)GPIO_Pin));
}
/**
  * @brief Reads the specified GPIO Output data pin.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @param[in] GPIO_Pin : This parameter contains the pin number, it can be one member
  * of the @ref GPIO_Pin_TypeDef enumeration.
  * @retval BitStatus : GPIO output pin status.
	* This parameter can be any of the @ref BitStatus enumeration.
  * @par Required preconditions:
  * The port must be configured in input mode.
  */
BitStatus GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin)
{
    return ((BitStatus)(GPIOx->ODR & (uint8_t)GPIO_Pin));
}
/**
  * @brief Configures the external pull-up on GPIOx pins.
  * @param[in] GPIOx : Select the GPIO peripheral number (x = A to D).
  * @param[in] GPIO_Pin : This parameter contains the pin number, it can be one or many members
  * of the @ref GPIO_Pin_TypeDef enumeration.
  * @param[in] NewState : The new state of the pull up pin.
	* This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void GPIO_ExternalPullUpConfig(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState!=DISABLE) /* External Pull-Up Set*/
    {
        GPIOx->CR1 |= GPIO_Pin;
    } else /* External Pull-Up Reset*/
    {
        GPIOx->CR1 &= (uint8_t)(~(GPIO_Pin));
    }
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
