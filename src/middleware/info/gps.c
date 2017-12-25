/****************************************Copyright (c)**************************************************                         
** File name:			gps.c
** Descriptions:		获取和设置GPS   信息
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

static gps_info_t gps_now = {{0},0,0,0,0,0,0};

void gps_get_info(gps_info_t * gps_info)
{
	memcpy(gps_info,&gps_now,sizeof(gps_info_t));
}

void gps_set_info(gps_info_t * gps_info)
{
	memcpy(&gps_now,gps_info,sizeof(gps_info_t));
}

