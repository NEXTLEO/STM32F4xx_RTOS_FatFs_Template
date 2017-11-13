/**
  ******************************************************************************
  * @file    hal_bsp_sd.h
  * @author  MCD Application Team
  * @version V2.2.3
  * @date    22-April-2016
  * @brief   This file includes the uSD card driver.
  
  * @modify  nextleo
  * @version V2.2.4
  * @date    12/Oct./2017
	
  ******************************************************************************
  * @attention
  **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_BSP_SD_H
#define __STM32F4xx_HAL_BSP_SD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#define SD_CardInfo HAL_SD_CardInfoTypedef
  
#define   MSD_OK         0x00
#define   MSD_ERROR      0x01
   
#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)

#define SD_DATATIMEOUT           ((uint32_t)100000000)
    
/* DMA definitions for SD DMA transfer */
#define __DMAx_TxRx_CLK_ENABLE            __DMA2_CLK_ENABLE
#define SD_DMAx_Tx_CHANNEL                DMA_CHANNEL_4
#define SD_DMAx_Rx_CHANNEL                DMA_CHANNEL_4
#define SD_DMAx_Tx_STREAM                 DMA2_Stream6  
#define SD_DMAx_Rx_STREAM                 DMA2_Stream3  
#define SD_DMAx_Tx_IRQn                   DMA2_Stream6_IRQn
#define SD_DMAx_Rx_IRQn                   DMA2_Stream3_IRQn
#define SD_DMAx_Tx_IRQHandler             DMA2_Stream6_IRQHandler   
#define SD_DMAx_Rx_IRQHandler             DMA2_Stream3_IRQHandler 
#define SD_DetectIRQHandler()             HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8)

//SD Card Detect Pin
#define SD_DETECT_PIN						GPIO_PIN_8
#define SD_DETECT_PORT						GPIOB
#define SD_DETECT_GPIO_CLK_ENABLE()			__GPIOB_CLK_ENABLE()
#define SD_DETECT_GPIO_CLK_DISABLE()		__GPIOB_CLK_DISABLE()

//CardReader Power Control Pin

#define CR_PWR_CTRL_PIN						GPIO_PIN_13				//Card Reader Power Control Pin
#define CR_PWR_CTRL_PORT					GPIOC					//Card Reader(AU6465) Power Control Port
#define CR_PWR_CTRL_GPIO_CLK_ENABLE()		__GPIOC_CLK_ENABLE()
#define CR_PWR_CTRL_GPIO_CLK_DISABLE()		__GPIOC_CLK_DISABLE()

#define SD_MUX_PIN							GPIO_PIN_15				//SD Mux PIN
#define SD_MUX_PORT							GPIOA					//SD Mux PORT
#define SD_MUX_GPIO_CLK_ENABLE()			__GPIOA_CLK_ENABLE()
#define SD_MUX_GPIO_CLK_DISABLE()			__GPIOA_CLK_DISABLE()

/* Card Reader Power Status */
typedef enum 
{
  CR_PWR_OFF,
  CR_PWR_ON
}CR_PWR_Type;

/* CardReader Enable/Disable Type */
typedef enum
{
	SD_MUX_MT,
	SD_MUX_CR,
	SD_MUX_INVALID
}SD_MUX_Type;

/* SD Insert/Remove Type */
typedef enum
{
	SD_REMOVE,
	SD_INSERT,
	SD_INVALID
}SD_STA_Type;

/* SD state change type */
typedef enum
{
	SD_STA_CHANGED,
	SD_STA_NOT_CHANGED,
	SD_STA_INVALID
}SD_STA_CHANGE_Type;


/* functions */
uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_ITConfig(void);
void    BSP_SD_DetectIT(void);
void    BSP_SD_DetectCallback(void);
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_SD_Erase(uint64_t StartAddr, uint64_t EndAddr);
void    BSP_SD_IRQHandler(void);
void    BSP_SD_DMA_Tx_IRQHandler(void);
void    BSP_SD_DMA_Rx_IRQHandler(void);
HAL_SD_TransferStateTypedef BSP_SD_GetStatus(void);
void    BSP_SD_GetCardInfo(HAL_SD_CardInfoTypedef *CardInfo);
uint8_t BSP_SD_IsDetected(void);


/* New Add */

uint32_t sd_control_pin_init(void);
uint32_t sd_cardreader_power_control_init(void);
uint32_t sd_cardreader_power_control(CR_PWR_Type CR_Pwr);
uint32_t sd_mux_control_init(void);
uint32_t sd_mux_control(SD_MUX_Type SD_Mux);
uint32_t sd_connect_statemachine(void);

/* enter cardreader mode */
uint32_t enter_cardreader_mode(void);
uint32_t exit_cardreader_mode(void);

/* sd status */
extern inline SD_STA_Type sd_get_conncet_status(void);
extern inline void sd_set_connect_status(SD_STA_Type newConnectStatus);
extern inline SD_STA_CHANGE_Type sd_get_change_status(void);
extern inline void sd_set_change_status(SD_STA_CHANGE_Type newChangeStatus);

#ifdef __cplusplus
}
#endif

#endif
