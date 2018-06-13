/****************************************Copyright (c)**************************************************                         
** File name:			toplight.c
** Descriptions:		获得当前运行的车辆顶灯状态
**					
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-13
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
#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"

#include	"middleware/info/toplight.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static unsigned char toplight_status;
static unsigned char last_toplight_status;

void set_toplight_operation_status(unsigned char status)
{
	if(toplight_status != TOPLIGHT_STATUS_ALARM)
	{
		last_toplight_status = toplight_status;
	}
	
	toplight_status = status;
	gb905_toplight_set_operation_status(status);
}

void set_toplight_alarm_status(void)
{
	DbgFuncEntry();
	
	set_toplight_operation_status(TOPLIGHT_STATUS_ALARM);
	
	DbgFuncExit();
}

void reset_toplight_alarm_status(void)
{
	DbgFuncEntry();
	
	set_toplight_operation_status(last_toplight_status);
	
	DbgFuncExit();
}
