/**
  ******************************************************************************
  * @file    hal_bsp_sd.c
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

/* Includes ------------------------------------------------------------------*/
#include "hal_bsp_sd.h"
#include "hal_bsp_file.h"

SD_HandleTypeDef uSdHandle;
SD_CardInfo uSdCardInfo;

volatile SD_STA_Type sdConnectStatus;
volatile SD_STA_CHANGE_Type sdStatusChange;

extern char SDPath[4];

static void SD_MspInit(void);
static uint32_t sd_detect_init(void);
static uint32_t sd_check_connect_status(void);
static uint32_t sd_check_connect_status_change(void);

/*
	enter sd mode
	sd power enable 
	analog switcher power on
	card reader power off
*/
uint32_t sd_control_pin_init(void)
{
	sd_cardreader_power_control_init();
	sd_mux_control_init();
	sd_detect_init();
	
	sd_cardreader_power_control(CR_PWR_OFF);	/* disable cardreader power */
	sd_mux_control(SD_MUX_MT);					/* sd connect to cpu */
	
	sd_set_connect_status(SD_REMOVE);	/* set sd default status */
	sd_check_connect_status_change();	/* update sd connect stauts */
	
	return 0;
}

/**
  * @brief  Initializes the SD CardReader Power Control Pin.
  * @retval Init status
  */
uint32_t sd_cardreader_power_control_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/* Enable the GPIO_LED Clock */
	CR_PWR_CTRL_GPIO_CLK_ENABLE();

	/* Configure the GPIO_LED pin */
	GPIO_InitStruct.Pin = CR_PWR_CTRL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(CR_PWR_CTRL_PORT, &GPIO_InitStruct);

	sd_cardreader_power_control(CR_PWR_OFF);
//	HAL_GPIO_WritePin(CR_PWR_CTRL_PORT, CR_PWR_CTRL_PIN, GPIO_PIN_SET);	//Power Off
	return 0;
}

/*
	enter cardreader mode
*/
uint32_t enter_cardreader_mode(void)
{
	sd_cardreader_power_control(CR_PWR_ON);
	sd_mux_control(SD_MUX_CR);
	
	return 0;
}

/*
	exit cardreader mode
*/
uint32_t exit_cardreader_mode(void)
{
	sd_cardreader_power_control(CR_PWR_OFF);
	sd_mux_control(SD_MUX_MT);
	
	return 0;
}

/**
  * @brief  Control the SD CardReader Power.
  * @param  CR_Pwr: CR_PWR_OFF - CardReader Power Off, CR_PWR_ON - CardReader Power On
  * @retval Control status
  */
uint32_t sd_cardreader_power_control(CR_PWR_Type CR_Pwr)
{
	if(CR_Pwr == CR_PWR_OFF)
	{
		HAL_GPIO_WritePin(CR_PWR_CTRL_PORT, CR_PWR_CTRL_PIN, GPIO_PIN_SET);	//Power Off
	}
	else if(CR_Pwr == CR_PWR_ON)
	{
		HAL_GPIO_WritePin(CR_PWR_CTRL_PORT, CR_PWR_CTRL_PIN, GPIO_PIN_RESET); //Power On
	}
	
	return 0;
}

/**
  * @brief  Initializes the SD MUX Pin.
  * @retval Init status
  */
uint32_t sd_mux_control_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/* Enable the GPIO_LED Clock */
	SD_MUX_GPIO_CLK_ENABLE();

	/* Configure the GPIO_LED pin */
	GPIO_InitStruct.Pin = SD_MUX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(SD_MUX_PORT, &GPIO_InitStruct);

	sd_mux_control(SD_MUX_MT);
//	HAL_GPIO_WritePin(SD_MUX_PORT, SD_MUX_PIN, GPIO_PIN_SET);	//USD ENABLE, SD DISABLE
	return 0;
}

/**
  * @brief  Control the SD CardReader Power.
  * @param  CR_Pwr: CR_PWR_OFF - CardReader Power Off, CR_PWR_ON - CardReader Power On
  * @retval Control status
  */
uint32_t sd_mux_control(SD_MUX_Type SD_Mux)
{
	if(SD_Mux == SD_MUX_MT)
	{
		HAL_GPIO_WritePin(SD_MUX_PORT, SD_MUX_PIN, GPIO_PIN_RESET);//SD DISABLE, USD ENABLE
	}
	else if(SD_Mux == SD_MUX_CR)
	{
		HAL_GPIO_WritePin(SD_MUX_PORT, SD_MUX_PIN, GPIO_PIN_SET);//USD DISABLE, SD ENABLE
	}
	
	return 0;
}

/**
  * @brief  Initializes the SD Detect Pin.
  * @retval Init status
  */
static uint32_t sd_detect_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  
	/* Enable the SD Detect Pin Clock */
	SD_DETECT_GPIO_CLK_ENABLE();
	
	/* Configure SD Detect Pin as input */
	GPIO_InitStruct.Pin = SD_DETECT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(SD_DETECT_PORT, &GPIO_InitStruct);
	
	return 0;
}

/*
	get sd connect status
	1 - insert
	0 - remove
*/
static uint32_t sd_check_connect_status(void)
{
	if(HAL_GPIO_ReadPin(SD_DETECT_PORT, SD_DETECT_PIN) == 0x00)	/* SD Insert */
	{
		return 1;
	}
	else /* SD Remove */
	{
		return 0;
	}
}

/*
	check wheather the sd connect status change
*/
static uint32_t sd_check_connect_status_change(void)
{
	uint32_t newSdSta = 0;
	SD_STA_Type curSdSta;
	
	newSdSta = sd_check_connect_status();
	curSdSta = sd_get_conncet_status();
	
	if(newSdSta == 0x01 && curSdSta == SD_REMOVE)
	{
		sd_set_connect_status(SD_INSERT);
		sd_set_change_status(SD_STA_CHANGED);		
//		LOG(MD_CGREEN "SD Insert.\n");

	}
	else if(newSdSta == 0x00 && curSdSta == SD_INSERT)
	{
		sd_set_connect_status(SD_REMOVE);
		sd_set_change_status(SD_STA_CHANGED);		
//		LOG(MD_CGREEN "SD Renove.\n");
	}
	
	return 0;
}

/*
	sd connect state machine
*/
uint32_t sd_connect_statemachine(void)
{
	sd_check_connect_status_change();
	if(sd_get_change_status() == SD_STA_CHANGED)
	{
		sd_set_change_status(SD_STA_NOT_CHANGED);
		
		if(sd_get_conncet_status() == SD_INSERT)
		{
//			LOG(MD_CGREEN "SD Insert.\n");
		}
		else if(sd_get_conncet_status() == SD_REMOVE)
		{
//			LOG(MD_CGREEN "SD Renove.\n");			
		}
	}
	
	return 0;
}

/*
	get sd connect status
*/
inline SD_STA_Type sd_get_conncet_status(void)
{
	return sdConnectStatus;
}

/*
	set sd connect status
*/
inline void sd_set_connect_status(SD_STA_Type newConnectStatus)
{
	sdConnectStatus = newConnectStatus;
}

/*
	get sd change status
*/
inline SD_STA_CHANGE_Type sd_get_change_status(void)
{
	return sdStatusChange;
}

/*
	set sd change status
*/
inline void sd_set_change_status(SD_STA_CHANGE_Type newChangeStatus)
{
	sdStatusChange = newChangeStatus;
}


/**
  * @brief  Initializes the SD card device.
  * @retval SD status
  */
uint8_t BSP_SD_Init(void)
{ 
	uint8_t SD_state = MSD_OK;

	/* uSD device interface configuration */
	uSdHandle.Instance = SDIO;
	
	/*!< Configure the SDIO peripheral */
	/*!< SDIO_CK = SDIOCLK / (SDIO_TRANSFER_CLK_DIV + 2) */
	/*!< on STM32F4xx devices, SDIOCLK is fixed to 48MHz ? */ 

	uSdHandle.Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;
	uSdHandle.Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;
	uSdHandle.Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;
	uSdHandle.Init.BusWide             = SDIO_BUS_WIDE_1B;
	uSdHandle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	uSdHandle.Init.ClockDiv            = 18; //SDIO_TRANSFER_CLK_DIV;

	/* Configure IO functionalities for SD detect pin */
//	BSP_SD_DETECT_PIN_Init();
	sd_detect_init();
	
	/* Check if the SD card is plugged in the slot */
	if(BSP_SD_IsDetected() != SD_PRESENT)
	{
		return MSD_ERROR;
	}
	
	/* HAL SD initialization */
	SD_MspInit();
	if(HAL_SD_Init(&uSdHandle, &uSdCardInfo) != SD_OK)
	{
		SD_state = MSD_ERROR;
	}

	/* Configure SD Bus width */
	if(SD_state == MSD_OK)
	{
		/* Enable wide operation */
		if(HAL_SD_WideBusOperation_Config(&uSdHandle, SDIO_BUS_WIDE_4B) != SD_OK)
		{
			SD_state = MSD_ERROR;
		}
		else
		{
			SD_state = MSD_OK;
		}
	}

	return  SD_state;
}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @retval Returns if SD is detected or not
 */
uint8_t BSP_SD_IsDetected(void)
{
	__IO uint8_t status = SD_PRESENT;
	
	/* Check SD card detect pin */
	if(HAL_GPIO_ReadPin(SD_DETECT_PORT, SD_DETECT_PIN))
	{
		status = SD_NOT_PRESENT;
		sd_set_connect_status(SD_REMOVE);
	}
	else
	{
		sd_set_connect_status(SD_INSERT);
	}

	return status;
}

/** @brief  SD detect IT detection callback
  */
__weak void BSP_SD_DetectCallback(void)
{
  /* NOTE: This function Should not be modified, when the callback is needed,
     the BSP_SD_DetectCallback could be implemented in the user file
  */ 
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to read 
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
	if(HAL_SD_ReadBlocks(&uSdHandle, pData, ReadAddr, BlockSize, NumOfBlocks) != SD_OK)
	{
		return MSD_ERROR;
	}
	else
	{
		return MSD_OK;
	}
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode. 
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to write
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
	if(HAL_SD_WriteBlocks(&uSdHandle, pData, WriteAddr, BlockSize, NumOfBlocks) != SD_OK)
	{
		return MSD_ERROR;
	}
	else
	{
		return MSD_OK;
	}
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to read 
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint64_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
	uint8_t SD_state = MSD_OK;

	/* Read block(s) in DMA transfer mode */
	if(HAL_SD_ReadBlocks_DMA(&uSdHandle, pData, ReadAddr, BlockSize, NumOfBlocks) != SD_OK)
	{
		SD_state = MSD_ERROR;
	}

	/* Wait until transfer is complete */
	if(SD_state == MSD_OK)
	{
		if(HAL_SD_CheckReadOperation(&uSdHandle, (uint32_t)SD_DATATIMEOUT) != SD_OK)
		{
			SD_state = MSD_ERROR;
		}
		else
		{
			SD_state = MSD_OK;
		}
	}

	return SD_state; 
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to write 
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
	uint8_t SD_state = MSD_OK;

	/* Write block(s) in DMA transfer mode */
	if(HAL_SD_WriteBlocks_DMA(&uSdHandle, pData, WriteAddr, BlockSize, NumOfBlocks) != SD_OK)
	{
		SD_state = MSD_ERROR;
	}

	/* Wait until transfer is complete */
	if(SD_state == MSD_OK)
	{
		if(HAL_SD_CheckWriteOperation(&uSdHandle, (uint32_t)SD_DATATIMEOUT) != SD_OK)
		{
			SD_state = MSD_ERROR;
		}
		else
		{
			SD_state = MSD_OK;
		}
	}
  
	return SD_state;  
}

/**
  * @brief  Erases the specified memory area of the given SD card. 
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval SD status
  */
uint8_t BSP_SD_Erase(uint64_t StartAddr, uint64_t EndAddr)
{
	if(HAL_SD_Erase(&uSdHandle, StartAddr, EndAddr) != SD_OK)
	{
		return MSD_ERROR;
	}
	else
	{
		return MSD_OK;
	}
}

/**
  * @brief  Initializes the SD MSP.
  */
static void SD_MspInit(void)
{
	static DMA_HandleTypeDef dmaRxHandle;
	static DMA_HandleTypeDef dmaTxHandle;
	GPIO_InitTypeDef GPIO_Init_Structure;
	SD_HandleTypeDef *hsd = &uSdHandle;

	/* Enable SDIO clock */
	__SDIO_CLK_ENABLE();

	/* Enable DMA2 clocks */
	__DMAx_TxRx_CLK_ENABLE();

	/* Enable GPIOs clock */
	__GPIOC_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();

	/* Common GPIO configuration */
	GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
	GPIO_Init_Structure.Pull      = GPIO_PULLUP;
	GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
	GPIO_Init_Structure.Alternate = GPIO_AF12_SDIO;
  
	/* GPIOC configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

	/* GPIOD configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

//	/* NVIC configuration for SDIO interrupts */
//	#ifdef OLD_BOARD_DEBUG
//	HAL_NVIC_SetPriority(SDIO_IRQn, 0, 0);	//5, 0
//	#else
//	HAL_NVIC_SetPriority(SDIO_IRQn, 0, 0);	//5, 0
//	#endif
	
	HAL_NVIC_EnableIRQ(SDIO_IRQn);
    
	/* Configure DMA Rx parameters */
	dmaRxHandle.Init.Channel             = SD_DMAx_Rx_CHANNEL;
	dmaRxHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	dmaRxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
	dmaRxHandle.Init.MemInc              = DMA_MINC_ENABLE;
	dmaRxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	dmaRxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
	dmaRxHandle.Init.Mode                = DMA_PFCTRL;//DMA_PFCTRL;DMA_NORMAL
	dmaRxHandle.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
	dmaRxHandle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
	dmaRxHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	dmaRxHandle.Init.MemBurst            = DMA_MBURST_INC4;
	dmaRxHandle.Init.PeriphBurst         = DMA_PBURST_INC4;

	dmaRxHandle.Instance = SD_DMAx_Rx_STREAM;

	/* Associate the DMA handle */
	__HAL_LINKDMA(hsd, hdmarx, dmaRxHandle);

	/* Deinitialize the stream for new transfer */
	HAL_DMA_DeInit(&dmaRxHandle);

	/* Configure the DMA stream */
	HAL_DMA_Init(&dmaRxHandle);

	/* Configure DMA Tx parameters */
	dmaTxHandle.Init.Channel             = SD_DMAx_Tx_CHANNEL;
	dmaTxHandle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	dmaTxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
	dmaTxHandle.Init.MemInc              = DMA_MINC_ENABLE;
	dmaTxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	dmaTxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
	dmaTxHandle.Init.Mode                = DMA_PFCTRL;//DMA_PFCTRL;DMA_NORMAL
	dmaTxHandle.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
	dmaTxHandle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
	dmaTxHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	dmaTxHandle.Init.MemBurst            = DMA_MBURST_INC4;
	dmaTxHandle.Init.PeriphBurst         = DMA_PBURST_INC4;

	dmaTxHandle.Instance = SD_DMAx_Tx_STREAM;

	/* Associate the DMA handle */
	__HAL_LINKDMA(hsd, hdmatx, dmaTxHandle);

	/* Deinitialize the stream for new transfer */
	HAL_DMA_DeInit(&dmaTxHandle);

	/* Configure the DMA stream */
	HAL_DMA_Init(&dmaTxHandle); 

	/* NVIC configuration for DMA transfer complete interrupt */
	HAL_NVIC_SetPriority(SD_DMAx_Rx_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(SD_DMAx_Rx_IRQn);

	/* NVIC configuration for DMA transfer complete interrupt */
	HAL_NVIC_SetPriority(SD_DMAx_Tx_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(SD_DMAx_Tx_IRQn);
}

/**
  * @brief  Handles SD card interrupt request.
  */
void BSP_SD_IRQHandler(void)
{
	HAL_SD_IRQHandler(&uSdHandle);
}

/**
  * @brief  Handles SD DMA Tx transfer interrupt request.
  */
void BSP_SD_DMA_Tx_IRQHandler(void)
{
	HAL_DMA_IRQHandler(uSdHandle.hdmatx); 
}

/**
  * @brief  Handles SD DMA Rx transfer interrupt request.
  */
void BSP_SD_DMA_Rx_IRQHandler(void)
{
	HAL_DMA_IRQHandler(uSdHandle.hdmarx);
}

/**
  * @brief  Gets the current SD card data status.
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  SD_TRANSFER_OK: No data transfer is acting
  *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
  *            @arg  SD_TRANSFER_ERROR: Data transfer error 
  */
HAL_SD_TransferStateTypedef BSP_SD_GetStatus(void)
{
  return(HAL_SD_GetStatus(&uSdHandle));
}

/**
  * @brief  Get SD information about specific SD card.
  * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
  */
void BSP_SD_GetCardInfo(HAL_SD_CardInfoTypedef *CardInfo)
{
  /* Get SD card Information */
  HAL_SD_Get_CardInfo(&uSdHandle, CardInfo);
}
