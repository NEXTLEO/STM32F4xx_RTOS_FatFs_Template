/**
  ******************************************************************************
  * @file    STM32F4xx_PRJ/Src/main.c 
  * @author  nextleo
  * @version v1.0.1.3
  * @date    13/Oct./2017
  * @brief   This is a STM32F427 Project Template with FreeRTOS + Fatfs + LED.
  ******************************************************************************
**/

#include "main.h"
#include "log.h"
#include "cmsis_os.h"

#define WRITE_STRING		"STM32F4xx_FatFs_RTOS\r\n"

uint32_t counter0 = 0;
uint32_t counter1 = 0;

osThreadId FILEThreadHandle;

static void osTimerCallback(void const *argument);
static void FileHandleThread(void const *argument);
static void Error_Handler(void);

/* main function */
int main(void)
{
	if(system_init_os() != 0) {
		LOG(LC_CRED "System Init Error.\n");
//		Error_Handler();
	}
	LOG(LC_CGREEN "osTimerDef Start.\n");
	/* Create Timer */
	osTimerDef(LEDTimer, osTimerCallback);
	osTimerId osTimer = osTimerCreate (osTimer(LEDTimer), osTimerPeriodic, NULL);

	LOG(LC_CGREEN "osTimerDef OK.\n");

	/* Start Timer */
	osTimerStart(osTimer, 500);

	/* Create File Handle Thread */
	osThreadDef(FILEThread, FileHandleThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	FILEThreadHandle = osThreadCreate(osThread(FILEThread), NULL);
	
	/* Start scheduler */
	osKernelStart();
	LOG(LC_CGREEN "osKernelStart OK.\n");
	/* We should never get here as control is now taken by the scheduler */
	for(;;);
}

/**
  * @brief  Timer Clbk that toggle LED_GRE
  * @param  argument not used
  * @retval None
  */
static void osTimerCallback (void const *argument)
{
	(void) argument;  
	/* Toggle LED1 */
	BSP_LED_Toggle(LED_GRE);
	counter0++;
	LOG(LC_CCYAN "Toggle LED_GRE Thread OK  -  %d.\n", counter0);
}

/**
  * @brief  File Handle thread
  * @param  argument not used
  * @retval None
  */
static void FileHandleThread(void const *argument)
{
	(void) argument;
	FRESULT res_write;
	for(;;) {
		BSP_LED_Toggle(LED_RED);
		osDelay(1000);
		counter1++;
		res_write = file_write((char *)WRITE_STRING, sizeof(WRITE_STRING)-1, counter1-1);
		if(res_write != FR_OK) {
			LOG(LC_CRED "Write File Error - %d.\n", counter1);
		}
		else {
			LOG(LC_CYELLOW "Write File OK - %d.\n", counter1);
		}
	}
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
*/
void Error_Handler(void)
{
	/* Turn RED LED on */
	BSP_LED_Off(LED_GRE);
	BSP_LED_On(LED_RED);
		
	LOG(LC_CRED "System Error_Handler.\n");
	while(1) {
	}
}
