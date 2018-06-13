/****************************************Copyright (c)**************************************************                         
** File name:			event.c
** Descriptions:		记录事件信息
*
*
*
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-18
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

#include	"middleware/info/event.h"

#define		DEBUG_Y
#include	"libs/debug.h"


static save_event_t event_info_now;

// 设置事件信息
void event_set_info(save_event_t *event_info)
{
	DbgFuncEntry();
	
	memset(&event_info_now,0x00,sizeof(save_event_t));
	memcpy(&event_info_now,event_info,sizeof(save_event_t));

	DbgFuncExit();
}

// 获取事件信息
void event_get_info(save_event_t *event_info)
{
	DbgFuncEntry();
	
	memset(event_info,0x00,sizeof(save_event_t));
	memcpy(event_info,&event_info_now,sizeof(save_event_t));

	DbgFuncExit();
}

//打印事件信息
void event_info_debug(void)
{
	int i;
	save_event_t event_info;
	
	DbgFuncEntry();
	
	event_get_info(&event_info);

	for(i=0;i<event_info.num;i++)
	{
		DbgPrintf("event id = %d\r\n",event_info.event_msg[i].id);
		DbgPrintf("event content = %s\r\n",event_info.event_msg[i].content);
	}
	
	DbgFuncExit();
}
