/****************************************Copyright (c)**************************************************                         
** File name:			gb905_trace.c
** Descriptions:		位置跟踪(0x8202/0x0202)   的协议解析
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-11
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

#include	"middleware/info/gps.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static gps_info_t gps_now;

void gps_get_info(gps_info_t * gps_info)
{
	memcpy(gps_info,&gps_now,sizeof(gps_info_t));
}

void gps_set_info(gps_info_t * gps_info)
{
	memcpy(&gps_now,gps_info,sizeof(gps_info_t));
}

