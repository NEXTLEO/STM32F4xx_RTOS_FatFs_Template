/**
  ******************************************************************************
  * @file    Drivers/HAL_BSP/log/log.c
  * @author  nextleo
  * @version v1.0.1.1
  * @date    10/Oct./2017
  * @brief   HAL BSP module.    
  ******************************************************************************
  * @attention
  *
  ******************************************************************************  
  */ 
 
#include "log.h"

/*
	LOG RTT Init
*/
void log_rtt_init(void)
{
	SEGGER_RTT_Init();
}

/*
	LOG RTT Printf, Use SEGGER RTT directly.
*/
void log_rtt_printf(unsigned BufferIndex, const char * sFormat, ...)
{
	SEGGER_RTT_printf(BufferIndex, sFormat);
}
