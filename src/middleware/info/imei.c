/****************************************Copyright (c)**************************************************                         
** File name:			imei.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-04
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#include	"common.h"


#define		DEBUG_Y
#include	"libs/debug.h"

// IMEI "864244020879622" --> MTD 0x25b85
#define		TEST_IMEI_ID			"864244020879622"

static char IMEI[MAX_IMEI_CHARS_SIZE];

void init_imei(char *imei)
{
	DbgFuncEntry();

	DbgPrintf("init imei:%s\r\n",imei);
	memcpy(IMEI,imei,MAX_IMEI_CHARS_SIZE);

	DbgFuncExit();
}

void get_imei(char * imei)
{
	DbgFuncEntry();
	
	memset(imei,0x00,MAX_IMEI_CHARS_SIZE);
	memcpy(imei,TEST_IMEI_ID,MAX_IMEI_CHARS_SIZE);

	DbgFuncExit();
}

