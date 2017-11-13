/**
  ******************************************************************************
  * @file    STM32F4xx_PRJ/Src/init.c 
  * @author  nextleo
  * @version v1.0.1.3
  * @date    13/Oct./2017 
  * @brief   System Init File.
  ******************************************************************************
  **/
 
#include "init.h"
#include "log.h"
#include "hal_bsp_sd.h"
#include "hal_bsp_led.h"
#include "hal_bsp_file.h"

static uint32_t SystemClock_Config(void);

uint32_t system_init_os(void)
{
	HAL_Init();
	SystemClock_Config();
	
	if(led_init() != 0)	/* led init */
	{
		LOG(LC_CRED "LED Init Error.\n");
		return 1;		
	}
	if(sd_init() != 0)
	{
		LOG(LC_CRED "SD INIT ERROR\r\n");
		return 1;
	}
	
	LOG(LC_CGREEN "OS System Init Finish.\n");
	
	return 0;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
uint32_t SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is 
	 clocked below the maximum system frequency, to update the voltage scaling value 
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/*
	PLL_VCO input clock = (HSE_VALUE or HSI_VALUE / PLL_M)
	PLL_VCO output clock = (PLL_VCO input clock) * PLL_N
	System Clock = (PLL_VCO output clock) / PLL_P = (16000000 / 8 * 160) / 2 = 160 MHz
	USB / SDIO Clock = (PLL VCO output clock) / PLL_Q = (16000000 / 8 * 160) / 8 = 40 MHz
	*/
	
	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 160;	//New Board Configure, external Oscillator = 16MHz
	
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 8;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/* Activate the Over-Drive mode */
//	HAL_PWREx_EnableOverDrive();
	
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
	
	return 0;
}
