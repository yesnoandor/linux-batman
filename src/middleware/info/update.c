/****************************************Copyright (c)**************************************************                         
** File name:			update.c
** Descriptions:		升级的信息
**						
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-28
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

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/ctrl/gb905_control.h"

#include	"middleware/info/update.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static gb905_update_device_info_t update_device_info;

void get_update_info(unsigned char * device_info)
{
	memcpy(device_info,&update_device_info,sizeof(gb905_update_device_info_t));
}

void set_update_info(unsigned char * device_info)
{
	memcpy(&update_device_info,device_info,sizeof(gb905_update_device_info_t));
}



