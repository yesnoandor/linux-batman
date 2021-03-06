/****************************************Copyright (c)**************************************************                         
** File name:			fleety_eval.c
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
#include	"module/gb905/operation/gb905_operate.h"
#include	"module/gb905/av/gb905_photo.h"

#include	"module/gb905_peri/meter/gb905_meter.h"

#include	"middleware/info/meter.h"

#include    "app/av/fleety_photo.h"

#define		DEBUG_Y
#include	"libs/debug.h"


void fleety_eval_treat(void)
{
	DbgFuncEntry();
	
	gb905_meter_operation_t meter_operation;

    //服务评价拍照
    fleety_photo_take_picture_treat(PHOTO_REASON_EVALUTION);
     
	get_meter_operation_info((char *)&meter_operation);
	
	gb905_build_pre_operation(OPERATION_EVAL);

	gb905_send_operate((unsigned char *)&meter_operation,sizeof(gb905_meter_operation_t));

	DbgFuncExit();
}

