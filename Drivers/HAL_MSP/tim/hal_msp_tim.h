/**
  ******************************************************************************
  * @file    TIM/TIM_TimeBase/Src/stm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @version V1.2.7
  * @date    04-November-2016 
  * @brief   HAL MSP module.    
  
  * @modify  nextleo
  * @version V1.2.8
  * @date    11/Oct./2017
  
  ******************************************************************************
  * @attention
  *
  ******************************************************************************  
  */

#ifndef __STM32F4xx_HAL_MSP_TIM_H
#define __STM32F4xx_HAL_MSP_TIM_H

#include "stm32f4xx_hal.h"
#include "hal_bsp_led.h"

/* Definition for TIMx clock resources */
#define TIMx							TIM3
#define TIM3_CLK_ENABLE					__HAL_RCC_TIM3_CLK_ENABLE

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM3_IRQn
#define TIMx_IRQHandler                TIM3_IRQHandler

#define TIM4_CLK_ENABLE					__HAL_RCC_TIM4_CLK_ENABLE

/* Exported functions ------------------------------------------------------- */

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
/* Timer3 Configure */
uint32_t TIM3_Config(void);
/* Timer4 Configure */
uint32_t TIM4_Config_For_Buzzer(void);
uint32_t TIM4_Config_For_KEY_Detect(void);
/* Timer Enable */
uint32_t TIMx_Enable(TIM_HandleTypeDef TimHandle);
/* Timer Disable */
uint32_t TIMx_Disable(TIM_HandleTypeDef TimHandle);

#endif	/* __STM32F4xx_HAL_MSP_TIM_H */
