/****************************************Copyright (c)**************************************************                         
** File name:			mcu_eval.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-24
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

#include	"module/gb905_ex/mcu/mcu_common.h"
#include	"module/gb905_ex/mcu/mcu_eval.h"


#include	"middleware/event/fleety_event.h"


#define		DEBUG_Y
#include	"libs/debug.h"




typedef  struct
{
    unsigned char start_magic_id;
    mcu_protocol_header_t header;
	unsigned char eval;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed mcu_protocol_eval_t;




unsigned char mcu_get_evaluate_treat(unsigned char *buf,int len)
{
	unsigned char evaluate;

	fleety_event_t event;

	DbgFuncEntry();

	evaluate = *(unsigned char *)(&buf[1 + sizeof(mcu_protocol_header_t)]);

	DbgGood("evaluate = 0x%x\r\n",evaluate);

	event.id = EVAL_EVENT;
	//event.param = (int)evaluate;
	if(evaluate==0x00)
	{
		event.param = EVAL_OK;
	}
	else if(evaluate==0x01)
	{
		event.param = EVAL_SOSO;
	}
	else if(evaluate==0x02)
	{
		event.param = EVAL_BAD;
	}
	else if(evaluate==0x03)
	{
		event.param = EVAL_COMPLAINT;
	}
	else
	{
		event.param = EVAL_NONE;
	}

	event.priority = DAFAULT_PRIORITY;

	fleety_event_push(&event);

	DbgFuncExit();

	return GB905_EX_RESULT_OK;
	
}




