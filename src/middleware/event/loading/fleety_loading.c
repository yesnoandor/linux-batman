/****************************************Copyright (c)**************************************************                         
** File name:			fleety_loading.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-13
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

#include	"module/gb905/operation/gb905_operate.h"

#include	"middleware/info/toplight.h"

#include    "app/av/fleety_photo.h"

#define		DEBUG_Y
#include	"libs/debug.h"

/** 
* @brief 	 空重车事件的处理
* @param loading		空重车状态
* 
*/
void fleety_loading_treat(int loading)
{
	DbgFuncEntry();

	DbgPrintf("loading = %d\r\n",loading);
	
	if(loading)
	{
		gb905_build_pre_operation(OPERATION_BEGIN);
		set_toplight_operation_status(TOPLIGHT_STATUS_LOADING);
        
        //重车拍照
        fleety_photo_take_picture_treat(PHOTO_REASON_OCCUPY);
	}
	else
	{
		gb905_build_pre_operation(OPERATION_END);
		set_toplight_operation_status(TOPLIGHT_STATUS_EMPTY);
	}

	DbgFuncExit();
}

