/****************************************Copyright (c)**************************************************                         
** File name:			fleety_meter_supplement.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-02-25
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

#include	"module/gb905_peri/meter/gb905_meter.h"




#include	"middleware/info/meter.h"

#define		DEBUG_Y
#include	"libs/debug.h"


void fleety_meter_supplement_treat(void)
{
	DbgFuncEntry();
	
	gb905_meter_operation_t meter_operation;

	get_meter_operation_info((char *)&meter_operation);

	gb905_build_pre_operation(OPERATION_BEGIN);
	gb905_build_pre_operation(OPERATION_END);
	gb905_build_pre_operation(OPERATION_EVAL);

	gb905_send_operate((unsigned char *)&meter_operation,sizeof(gb905_meter_operation_t));

	DbgFuncExit();
}

