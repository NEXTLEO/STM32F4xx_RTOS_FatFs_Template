/**
  ******************************************************************************
  * @file    Drivers/HAL_BSP/LED/hal_bsp_file.h
  * @author  nextleo
  * @version v1.0.1.1
  * @date    13/Oct./2017
  * @brief   Fatfs application.
  ******************************************************************************
**/

#ifndef __STM32F4xx_HAL_BSP_FILE_H
#define __STM32F4xx_HAL_BSP_FILE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hal_bsp_sd.h"


uint32_t sd_init(void);
uint32_t file_create(void);
FRESULT file_write(char *buff, uint32_t len, uint32_t offset);
FRESULT file_close(void);


#ifdef __cplusplus
}
#endif

#endif	/* __STM32F4xx_HAL_BSP_FILE_H */
