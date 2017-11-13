/**
  ******************************************************************************
  * @file    Drivers/HAL_BSP/LED/hal_bsp_file.c
  * @author  nextleo
  * @version v1.0.1.1
  * @date    13/Oct./2017
  * @brief   Fatfs application.
  ******************************************************************************
**/

#include "hal_bsp_file.h"
#include "log.h"

FATFS SDFatFs;  	/* File system object for SD card logical drive */
char SDPath[4]; 	/* SD card logical drive path */
FIL TESTFile;

static FRESULT mount_volume(void);
static FRESULT unmount_volume(void);
static uint32_t file_create_directory(void);

/*
	sd init
*/
uint32_t sd_init(void)
{
	sd_control_pin_init();

	if(sd_get_conncet_status() != SD_INSERT)
	{
		LOG(LC_CRED "SD card removed.\n");
		return 1;
	}

	/*##-1- Link the micro SD disk I/O driver ##*/
	uint8_t res = FATFS_LinkDriver(&SD_Driver, SDPath);
	
	char buff[4];
	sprintf(buff, "%c%c%c%c", SDPath[0], SDPath[1], SDPath[2], SDPath[3]);
	LOG(LC_CGREEN "FATFS_LinkDriver res = %d, SDPath = %s.\n", res, buff);

	if(res != 0)
	{
		LOG(LC_CRED "FATFS_LinkDriver error %d.\n", res);
		return 1;
	}
	
	/*##-2- Register the file system object to the FatFs module ##*/
	if(mount_volume() != 0)
	{
		return 1;
	}
	
	/*##-3- Create Directory ##*/
	if(file_create_directory() != 0)
	{
		return 1;
	}
	/* ##-4- Create File ## */
	if(file_create() != 0)
	{
		LOG(LC_CRED "File Create Error.\r\n");
		return 1;
	}

	LOG(LC_CYELLOW " - - - - SD INIT FINISH.\r\n");
	
	return 0;	
}

/*
	Mount volume
*/
static FRESULT mount_volume(void)
{
	FRESULT res_mount;
	
	res_mount = f_mount(&SDFatFs, (TCHAR const*)SDPath, 1);
	if(res_mount != FR_OK)
	{
		LOG(LC_CRED "Mount volume Error - %d\r\n", res_mount);
	}
	return res_mount;
}

/*
	Unmount volume
*/
static FRESULT unmount_volume(void)
{
	FRESULT res_mount = f_mount(NULL, (TCHAR const*)SDPath, 0);
	if(res_mount != FR_OK)
	{
		LOG(LC_CRED "Unmount volume Error - %d\r\n", res_mount);
	}
	return res_mount;
}

/*
	Create Directory
*/
static uint32_t file_create_directory(void)
{
	FRESULT res_mkdir;
	char path[10];
	
	sprintf(path, "0:/STM32F4xx");
	res_mkdir = f_mkdir(path);
	if(res_mkdir != FR_OK && res_mkdir != FR_EXIST)
	{
		LOG(LC_CRED "f_mkdir Error - %d.\n", res_mkdir);
		return 1;
	}
	else
	{
		LOG(LC_CGREEN "f_mkdir Finish - %d\r\n", res_mkdir);
	}
	return 0;	/* Success */
}

/*
	Create File
*/
uint32_t file_create(void)	
{
	FRESULT res_open, res_write, res_sync;
	UINT bw;
	
	res_open = f_open(&TESTFile, "0:/STM32F4xx/HelloWorld.txt", FA_CREATE_ALWAYS | FA_WRITE);
	/*##-4- Create and Open a new text file object with write access #####*/
	if(res_open != FR_OK)
	{
		LOG(LC_CRED "Create File Error - %d\r\n", res_open);
		return 1;
	}
	LOG(LC_CGREEN "Create File Finish - %d\r\n", res_open);

//	res_write = f_write(&TESTFile, "HELLO\r\n", sizeof("HELLO\r\n")-1, &bw);
//	if(res_write != FR_OK)
//	{
//		LOG(LC_CRED "Write File Error - %d \r\n", res_write);
//		return 1;
//	}
//	LOG(LC_CGREEN "Write File Finish - %d \r\n", res_write);
	
	res_sync = f_sync(&TESTFile);
	if(res_sync != FR_OK)
	{
		LOG(LC_CRED "Sync File Error - %d \r\n", res_sync);
		return 1;
	}
	LOG(LC_CGREEN "Sync File Finish - %d \r\n", res_sync);
	
	return 0;
}

/*
	Write File
*/
FRESULT file_write(char *buff, uint32_t len, uint32_t offset)
{
	FRESULT res_lseek, res_write, res_sync;
	UINT bw;
	
	res_lseek = f_lseek(&TESTFile, len * offset);
	if(res_lseek != FR_OK)
	{
		LOG(LC_CRED "Seek File Error - %d\r\n", res_lseek);
		return res_lseek;
	}
	LOG(LC_CGREEN "Seek File OK\r\n");
	
	res_write = f_write(&TESTFile, buff, len, &bw);
	if(res_write != FR_OK)
	{
		LOG(LC_CRED "Write File Error - %d \r\n", res_write);
		return res_write;
	}
	LOG(LC_CGREEN "Write File Finish - %d \r\n", res_write); 
	
	f_puts("Stay Hungry, Stay Foolish.\r\n", &TESTFile);
	
	res_sync = f_sync(&TESTFile);
	if(res_sync != FR_OK)
	{
		LOG(LC_CRED "Sync File Error - %d \r\n", res_sync);
		return res_sync;
	}
	LOG(LC_CGREEN "Sync File Finish - %d \r\n", res_sync);
	
	return FR_OK;
}

/*
	Close File
*/
FRESULT file_close(void)
{
	FRESULT res_close, res_mount;
	res_close = f_close(&TESTFile);
	if(res_close != FR_OK)
	{
		LOG(LC_CRED, "Close File Error - %d \r\n", res_close);
		return res_close;
	}
	
	res_mount = unmount_volume();
	if(res_mount != FR_OK)
	{
		LOG(LC_CRED, "unmount_volume Error - %d \r\n", res_mount);
		return res_mount;
	}
	
	return FR_OK;
}
