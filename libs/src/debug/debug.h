/****************************************Copyright (c)**************************************************                         
** File name:			debug.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-01-19
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef __DEBUG_H
#define __DEBUG_H

#include	"stdio.h"
#include	"string.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DBG_C_NONE         "\033[m"
#define DBG_C_RED          "\033[0;32;31m"
#define DBG_C_LIGHT_RED    "\033[1;31m"
#define DBG_C_GREEN        "\033[0;32;32m"
#define DBG_C_LIGHT_GREEN  "\033[1;32m"
#define DBG_C_BLUE         "\033[0;32;34m"
#define DBG_C_LIGHT_BLUE   "\033[1;34m"
#define DBG_C_DARY_GRAY    "\033[1;30m"
#define DBG_C_CYAN         "\033[0;36m"
#define DBG_C_LIGHT_CYAN   "\033[1;36m"
#define DBG_C_PURPLE       "\033[0;35m"
#define DBG_C_LIGHT_PURPLE "\033[1;35m"
#define DBG_C_BROWN        "\033[0;33m"
#define DBG_C_YELLOW       "\033[1;33m"
#define DBG_C_LIGHT_GRAY   "\033[0;37m"
#define DBG_C_WHITE        "\033[1;37m"

#define	DbgFuncEntry()								\
	printf(DBG_C_DARY_GRAY);						\
	printf("%s::++++++++++\r\n",__FUNCTION__);		\
	printf(DBG_C_NONE);								\
	fflush(stdout);

#define	DbgFuncExit()								\
	printf(DBG_C_DARY_GRAY);						\
	printf("%s::----------\r\n",__FUNCTION__);		\
	printf(DBG_C_NONE);								\
	fflush(stdout);



void DbgError(const char *fmt, ...);
void DbgWarn(const char *fmt, ...);

void DbgPrintf(const char *fmt, ...);
void DbgGood(const char *fmt, ...);

#ifdef __cplusplus
}
#endif



#endif

