/**
  ******************************************************************************
  * @file    Drivers/HAL_BSP/log/log.h
  * @author  nextleo
  * @version v1.0.1.1
  * @date    10/10/2017 
  * @brief   HAL BSP module.    
  ******************************************************************************
  * @attention
  *
  ******************************************************************************  
  */ 
 
#ifndef __LOG_H
#define __LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SEGGER_RTT_Conf.h>
#include <SEGGER_RTT.h>	 
	 
#ifndef LOG_USE_COLORS
	#define LOG_USE_COLORS			1
#endif /* LOG_USE_COLORS */

#if defined	LOG_USE_COLORS
    #define LC_CBLACK    			"\x1B[1;30m"
    #define LC_CRED      			"\x1B[1;31m"
    #define LC_CGREEN    			"\x1B[1;32m"
    #define LC_CYELLOW   			"\x1B[1;33m"
    #define LC_CBLUE     			"\x1B[1;34m"
    #define LC_CMAGENTA  			"\x1B[1;35m"
    #define LC_CCYAN     			"\x1B[1;36m"
    #define LC_CWHITE    			"\x1B[1;37m"
    #define LC_CDEFAULT
#else
    #define LC_CBLACK
    #define LC_CRED
    #define LC_CGREEN
    #define LC_CYELLOW
    #define LC_CBLUE
    #define LC_CMAGENTA
    #define LC_CCYAN
    #define LC_CWHITE
    #define LC_CDEFAULT
#endif /* LOG_USE_COLORS*/

#if defined LOG_RTT

#define LOG_DEFAULT_TERMINAL		0

#define LOG_INIT()               	log_rtt_init()

#define LOG_PRINTF(...)          	log_rtt_printf(LOG_DEFAULT_TERMINAL, ##__VA_ARGS__)
#define LOG							LOG_PRINTF

void log_rtt_init(void);
void log_rtt_printf(unsigned BufferIndex, const char * sFormat, ...);

#endif	/* LOG_RTT */

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* __LOG_H */
