/****************************************Copyright (c)**************************************************                         
** File name:			gb905_alarm.c
** Descriptions:		位置汇报(0x0200)  和位置信息查询(0x8201/0x0201) 的协议解析
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
#include	"module/gb905/av/gb905_photo.h"
#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"

#include	"middleware/info/status.h"
#include	"middleware/info/toplight.h"

#include	"app/report/fleety_report.h"
#include    "app/av/fleety_photo.h"

#define		DEBUG_Y
#include	"libs/debug.h"

/** 
* @brief 	中心确认报警
*
* @return  返回终端通用应答的状态
*/
unsigned char gb905_alarm_confirm_treat(void)
{
	DbgFuncEntry();

	reset_pre_alarm_status();
	set_emergency_alarm_status();
		
	DbgWarn("emergency alarm status!\r\n");

	fleety_report();

    //报警拍照
	fleety_photo_take_picture_treat(PHOTO_REASON_ALARM);

	set_toplight_alarm_status();
	
	DbgFuncExit();

	return GB905_RESULT_OK;
}


/** 
* @brief 	中心解除报警
* 
* @return  返回终端通用应答的状态
*/
unsigned char gb905_alarm_cancel_treat(void)
{
	DbgFuncEntry();

	reset_emergency_alarm_status();
	
	fleety_report();
	
	reset_toplight_alarm_status();
	
	DbgFuncExit();

	return GB905_RESULT_OK;
}



