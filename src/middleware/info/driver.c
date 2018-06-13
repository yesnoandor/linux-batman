/****************************************Copyright (c)**************************************************                         
** File name:			driver.c
** Descriptions:		驾驶员信息(有TSM模块获取)
*
*
*
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-26
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

#include	"module/gb905_peri/tsm/gb905_tsm.h"

#include	"middleware/info/driver.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static gb905_tsm_driver_info_t driver_info;

void set_driver_info(unsigned char *buf)
{
	DbgFuncEntry();

	memset((unsigned char*)&driver_info,0x00,sizeof(gb905_tsm_driver_info_t));	
	memcpy((unsigned char*)&driver_info,buf,sizeof(gb905_tsm_driver_info_t));

	DbgFuncExit();
}

void get_driver_info(unsigned char *buf)
{
	DbgFuncEntry();

	memcpy(buf,(unsigned char*)&driver_info,sizeof(gb905_tsm_driver_info_t));

	DbgFuncExit();
}

