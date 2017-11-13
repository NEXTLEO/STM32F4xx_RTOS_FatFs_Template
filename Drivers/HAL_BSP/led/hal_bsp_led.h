/**
  ******************************************************************************
  * @file    Drivers/HAL_BSP/LED/hal_bsp_led.h
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

#ifndef __STM32F4xx_HAL_BSP_LED_H
#define __STM32F4xx_HAL_BSP_LED_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define LEDn										2

#define LED_GRE_PIN                                GPIO_PIN_10
#define LED_GRE_GPIO_PORT                          GPIOE	 
#define LED_GRE_GPIO_CLK_ENABLE()                  __GPIOE_CLK_ENABLE()  
#define LED_GRE_GPIO_CLK_DISABLE()                 __GPIOE_CLK_DISABLE()  

#define LED_RED_PIN                                GPIO_PIN_12
#define LED_RED_GPIO_PORT                          GPIOE
#define LED_RED_GPIO_CLK_ENABLE()                  __GPIOE_CLK_ENABLE()  
#define LED_RED_GPIO_CLK_DISABLE()                 __GPIOE_CLK_DISABLE()


#define LEDx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) LED_GRE_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 1) LED_RED_GPIO_CLK_ENABLE(); \
                                            }while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LED_GRE_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LED_RED_GPIO_CLK_DISABLE(); \
                                            }while(0)

typedef enum 
{
  LED_GRE = 0,
  LED_RED = 1
}Led_TypeDef;


void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);

/* LED Init */
uint32_t led_init(void);


#ifdef __cplusplus
}
#endif

#endif	/* __STM32F4xx_HAL_BSP_LED_H */
