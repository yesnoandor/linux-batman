/****************************************Copyright (c)**************************************************                         
** File name:			debug.c
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

#include	<stdio.h>
#include 	<stdarg.h>

#include	"debug.h"


void DbgError(const char *fmt, ...)
{
    char strings[256];
	
    va_list args;
    va_start(args, fmt);
    vsprintf(strings, fmt, args);
    va_end(args);
	
    printf(DBG_C_LIGHT_RED);
    printf("%s", strings);
    printf(DBG_C_NONE);
    fflush(stdout);
}

void DbgWarn(const char *fmt, ...)
{
    char strings[256];
    va_list args;
    va_start(args, fmt);
    vsprintf(strings, fmt, args);
    va_end(args);
	
    printf(DBG_C_YELLOW);
    printf("%s",strings);
    printf(DBG_C_NONE);
    fflush(stdout);
}

void DbgPrintf(const char *fmt, ...)
{
    char strings[256];

    va_list args;
    va_start(args, fmt);
    vsprintf(strings, fmt, args);
    va_end(args);

    printf("%s", strings);
    printf(DBG_C_NONE);
    fflush(stdout);
}

void DbgGood(const char *fmt, ...)
{
    char strings[256];

    va_list args;
    va_start(args, fmt);
    vsprintf(strings, fmt, args);
    va_end(args);

    printf(DBG_C_LIGHT_BLUE);
    printf("%s",strings);
    printf(DBG_C_NONE);
    fflush(stdout);
}




