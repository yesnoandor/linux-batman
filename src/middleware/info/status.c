/****************************************Copyright (c)**************************************************                         
** File name:			status.c
** Descriptions:		获得当前运行的各种状态
**						1. 定位or    
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

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"

#include	"middleware/info/status.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static taxi_status_t taxi_status;

void taxi_get_status(taxi_status_t * status)
{
	memcpy(status,&taxi_status,sizeof(taxi_status_t));
}

