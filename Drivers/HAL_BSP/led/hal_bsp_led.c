/**
  ******************************************************************************
  * @file    Drivers/HAL_BSP/LED/hal_bsp_led.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    22-April-2016
  * @brief   HAL BSP module.
  
  * @modify  nextleo
  * @version V2.2.1
  * @date    12/Oct./2017
	
  ******************************************************************************
  * @attention
  **/

#include "hal_bsp_led.h"

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED_GRE_GPIO_PORT, 
                                 LED_RED_GPIO_PORT};

const uint16_t GPIO_PIN[LEDn] = {LED_GRE_PIN, 
                                 LED_RED_PIN};


/**
  * @brief  LED Init
  * @param  None
  * @return 0-Success,
			1-Fail
  */							 
uint32_t led_init(void)
{
	/* led init */
	BSP_LED_Init(LED_GRE);
	BSP_LED_Init(LED_RED);
	
	/* set led initial status 
	LED_GRE: 0-Off, 1-On
	LED_RED: 0-On, 1-Off
	*/
	BSP_LED_On(LED_GRE);	/* Green LED On */
	BSP_LED_Off(LED_RED);	/* Red LED Off */
	
//	BSP_LED_On(LED_RED);
	
	return 0;
}
								 
/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
  LEDx_GPIO_CLK_ENABLE(Led);

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN[Led];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIO_PORT[Led], &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET); 
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4 
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET); 
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET); 
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4  
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}
