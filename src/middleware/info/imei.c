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

#include	"middleware/info/imei.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static char IMEI[MAX_IMEI_CHARS_SIZE];

void get_imei(char * imei)
{
	DbgFuncEntry();
	
	memset(imei,0x00,MAX_IMEI_CHARS_SIZE);
	memcpy(imei,IMEI,MAX_IMEI_CHARS_SIZE);

	DbgFuncExit();
}


void set_imei(char * imei)
{
	DbgFuncEntry();
	
	memset(IMEI,0x00,MAX_IMEI_CHARS_SIZE);
	memcpy(IMEI,imei,MAX_IMEI_CHARS_SIZE);

	DbgFuncExit();
}

