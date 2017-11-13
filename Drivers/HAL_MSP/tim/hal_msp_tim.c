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

/* Includes ------------------------------------------------------------------*/
#include "hal_msp_tim.h"
#include "log.h"

TIM_HandleTypeDef Tim3Handle, Tim4Handle;

/**
  * @brief TIM MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIMx Peripheral clock enable */
	TIM3_CLK_ENABLE();
	TIM4_CLK_ENABLE();

	/*##-2- Configure the NVIC for TIMx ########################################*/
	/* Set the TIM3 priority */
	HAL_NVIC_SetPriority(TIM3_IRQn, 5, 0);
	/* Enable the TIM3 global Interrupt */
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
	/* Set the TIM4 priority */
	HAL_NVIC_SetPriority(TIM4_IRQn, 5, 0);
	/* Enable the TIM4 global Interrupt */
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

/*
	TIM3 init
*/
uint32_t TIM3_Config(void)
{
	uint16_t uwPrescalerValue = 0;
	
	/*##-1- Configure the TIM peripheral #######################################*/ 
	/* -----------------------------------------------------------------------
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
    To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
    Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    Prescaler = ((SystemCoreClock /2) /10 KHz) - 1
       
    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency  
	----------------------------------------------------------------------- */ 
  
	/* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz */
	uwPrescalerValue = (uint32_t) ((SystemCoreClock / 2) / 10000) - 1; //10000

	/* Set TIMx instance */
	Tim3Handle.Instance = TIM3;

	/* Initialize TIM3 peripheral as follows:
	   + Period = 10000 - 1
	   + Prescaler = ((SystemCoreClock/2)/10000) - 1
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	Tim3Handle.Init.Period = 10 - 1;	//1K
	Tim3Handle.Init.Prescaler = uwPrescalerValue;
	Tim3Handle.Init.ClockDivision = 0;
	Tim3Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&Tim3Handle) != HAL_OK)
	{
		/* Initialization Error */
		return 1;
	}

	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
	/* Start Channel1 */
//	if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
//	{
//		/* Starting Error */
//		return 1;
//	}
	
	return 0;
}

/*
	TIM4 init
*/
uint32_t TIM4_Config_For_Buzzer(void)
{
	uint32_t uwPrescalerValue = 0;
	
	/*##-1- Configure the TIM peripheral #######################################*/ 
	/* -----------------------------------------------------------------------
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock / 2
    To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
    Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    Prescaler = ((SystemCoreClock /2) / 10 KHz) - 1
       
    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency  
	----------------------------------------------------------------------- */ 
  
	/* Compute the prescaler value to have TIMx counter clock equal to 10 KHz */
	uwPrescalerValue = (uint32_t) ((SystemCoreClock / 2) / 10000) - 1; //10000
	//8000 - 1
//	LOG(MD_CRED "-------------------uwPrescalerValue = %d.\n", uwPrescalerValue);	
	
	/* Set TIMx instance */
	Tim4Handle.Instance = TIM4;

	/* Initialize TIMx peripheral as follows:
	   + Period = 10000 - 1
	   + Prescaler = ((SystemCoreClock/2)/10000) - 1
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	
	Tim4Handle.Init.Period = 2 - 1;	/* 5KHz */
	Tim4Handle.Init.Prescaler = uwPrescalerValue;
	Tim4Handle.Init.ClockDivision = 0;
	Tim4Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&Tim4Handle) != HAL_OK)
	{
		/* Initialization Error */
		return 1;
	}
	
	return 0;
}


/*
	TIM4 init
*/
uint32_t TIM4_Config_For_KEY_Detect(void)
{
	uint16_t uwPrescalerValue = 0;
	
	/*##-1- Configure the TIM peripheral #######################################*/ 
	/* -----------------------------------------------------------------------
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
    To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
    Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    Prescaler = ((SystemCoreClock /2) /10 KHz) - 1
       
    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency  
	----------------------------------------------------------------------- */ 
  
	/* Compute the prescaler value to have TIMx counter clock equal to 10 KHz */
	uwPrescalerValue = (uint32_t) ((SystemCoreClock / 2) / 10000) - 1; //10000

	/* Set TIMx instance */
	Tim4Handle.Instance = TIM4;

	/* Initialize TIMx peripheral as follows:
	   + Period = 10000 - 1
	   + Prescaler = ((SystemCoreClock/2)/10000) - 1
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	
	Tim4Handle.Init.Period = 10 - 1;	/* 1000Hz 1ms*/
	Tim4Handle.Init.Prescaler = uwPrescalerValue;
	Tim4Handle.Init.ClockDivision = 0;
	Tim4Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&Tim4Handle) != HAL_OK)
	{
		/* Initialization Error */
		return 1;
	}
	
	return 0;
}


/*
	TIMx Enable
*/
uint32_t TIMx_Enable(TIM_HandleTypeDef TimHandle)
{
	if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
	{
		/* Starting Error */
		return 1;
	}
	return 0;
}

/*
	TIMx Disable
*/
uint32_t TIMx_Disable(TIM_HandleTypeDef TimHandle)
{
	if(HAL_TIM_Base_Stop_IT(&TimHandle) != HAL_OK)
	{
		/* Starting Error */
		return 1;
	}
	return 0;
}
