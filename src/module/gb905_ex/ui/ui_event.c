/****************************************Copyright (c)**************************************************                         
** File name:			ui_event.c
** Descriptions:		事件信息处理(与APK信息交互)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-21
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

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_event.h"
#include	"module/gb905_ex/ui/ui_history_info.h"

#include	"module/gb905/event/gb905_event.h"

#include	"middleware/info/event.h"

#define		DEBUG_Y
#include	"libs/debug.h"

/** 
* @brief 	  	发送历史事件列表到UI
* 
*/
void ui_send_history_event_list(unsigned char display_type)
{
	ui_protocol_header_t *header;
	ui_history_info_t *history_info;
	save_event_t event_info;
	unsigned char *send_buf;
	unsigned char *data_buf;
	int send_len = 0;
	int event_msg_len = 0;
	int i;
	
	DbgFuncEntry();

	event_get_info(&event_info);

	event_msg_len = event_info.num*(sizeof(unsigned char));

	for(i=0;i<event_info.num;i++)
	{
		event_msg_len = event_msg_len + strlen((char*)event_info.event_msg[i].content) + 1;//1:sizeof '/0'
	}
	
	send_len = 3 + sizeof(ui_protocol_header_t) + event_msg_len + sizeof(ui_history_info_t);//3:7e+crc+7e 

	send_buf = malloc(send_len);

	header = (ui_protocol_header_t*)(send_buf + 1);//1:7e

	ui_protocol_build_header(header,SYSTEM2UI_HISTORY_INFO,event_msg_len + sizeof(ui_history_info_t));

	history_info = (ui_history_info_t*)(send_buf + 1 + sizeof(ui_protocol_header_t));

	history_info->type = display_type;
	history_info->num = (unsigned char)event_info.num;
	history_info->start_id = 0;
	history_info->end_id = event_info.num;

	data_buf = (unsigned char*)(send_buf + 1 + sizeof(ui_protocol_header_t) + sizeof(ui_history_info_t));

	for(i=0;i<event_info.num;i++)
	{
		*data_buf = event_info.event_msg[i].id ;
		data_buf++;
		memcpy(data_buf,event_info.event_msg[i].content,strlen((char*)event_info.event_msg[i].content) + 1);
		data_buf = data_buf + strlen((char*)event_info.event_msg[i].content) + 1;
	}

	ui_protocol_send_data(send_buf,send_len);

	free(send_buf);
	
	DbgFuncExit();

}

/** 
* @brief 	  	发送实时事件列表到UI
* 
*/
void ui_send_current_event_list(void)
{
	ui_protocol_header_t *header;
    save_event_t event_info;
	unsigned char *send_buf;
	unsigned char *data_buf;
	int send_len;
    int event_msg_len = 0;
    int i;
    
	DbgFuncEntry();

    event_get_info(&event_info);

	event_msg_len = event_info.num*(sizeof(unsigned char)) + 1;// size of id + sizoe of event num

	for(i=0;i<event_info.num;i++)
	{
		event_msg_len = event_msg_len + strlen((char*)event_info.event_msg[i].content) + 1;//1:sizeof '/0'
	}
	
	send_len = 3 + sizeof(ui_protocol_header_t) + event_msg_len; // 3:7e+crc+7e

	send_buf = malloc(send_len);

	header = (ui_protocol_header_t*)(send_buf + 1);//1:7e

	ui_protocol_build_header(header,SYSTEM2UI_CURRENT_EVENT,event_msg_len);

	data_buf = (unsigned char*)(send_buf + 1 + sizeof(ui_protocol_header_t));//1:7e

    *data_buf = event_info.num;
    data_buf++;
    
	for(i=0;i<event_info.num;i++)
	{
		*data_buf = event_info.event_msg[i].id ;
		data_buf++;
		memcpy(data_buf,event_info.event_msg[i].content,strlen((char*)event_info.event_msg[i].content) + 1);
		data_buf = data_buf + strlen((char*)event_info.event_msg[i].content) + 1;
	}

	ui_protocol_send_data(send_buf,send_len);

	free(send_buf);
    
	DbgFuncExit();

}


/** 
* @brief 	  	UI发送事件报告到终端
* 
*/
unsigned char ui_event_report_treat(unsigned char *buf,int len)
{
	unsigned char event_id;
	
	DbgFuncEntry();

	event_id = *buf;
	DbgPrintf("ui event report id 0x%02x\r\n",event_id);
	
	gb905_report_event_treat(event_id);

	DbgFuncExit();

	return UI_RESULT_OK;
}
