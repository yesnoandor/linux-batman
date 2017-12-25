/****************************************Copyright (c)**************************************************                         
** File name:			ubuntu_imei.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-24
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


// IMEI "864244020879622" --> MTD 0x25b85
#define		TEST_IMEI_ID			"864244020879622"

void ubuntu_imei_simu_init(void)
{
	DbgFuncEntry();

	set_imei(TEST_IMEI_ID,sizeof(TEST_IMEI_ID));

	DbgFuncExit();
}

