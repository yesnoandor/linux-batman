/****************************************Copyright (c)**************************************************                         
** File name:			fleety_login.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-15
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

#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/meter/gb905_meter.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"


#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905/login/gb905_login.h"



#define		DEBUG_Y
#include	"libs/debug.h"


void fleety_login_treat(int login)
{
	if(login)
	{
		// 设置顶灯状态
		gb905_toplight_set_operation_status(TOPLIGHT_STATUS_EMPTY);

		// 设置顶灯星级
		//gb905_toplight_set_star_status(0x01);

		// 向服务器发送上班签到信息
		gb905_login();
	}
	else
	{
		// 向服务器发送下班签退信息
		gb905_logout();
	}
}


