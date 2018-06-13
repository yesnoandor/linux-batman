/****************************************Copyright (c)**************************************************                         
** File name:			ui_notice.c
** Descriptions:		文本信息处理(与APK信息交互)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-23
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

#include	"module/gb905_ex/ui/ui_history_info.h"
#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_notice.h"

#include	"middleware/info/notice.h"

#define		DEBUG_Y
#include	"libs/debug.h"


/** 
* @brief 	  	发送历史报文列表到UI
* 
*/

void ui_send_history_notice_list(unsigned char display_type)
{
	ui_protocol_header_t *header;
	ui_history_info_t *history_info;
	save_notice_t notice_info;
	unsigned char *send_buf;
	unsigned char *data_buf;
	int send_len = 0;
	int notice_msg_len = 0;
	int i;
	
	DbgFuncEntry();

	notice_get_info(&notice_info);

	notice_msg_len = notice_info.num*(sizeof(unsigned char));//sizeof flag

	for(i=0;i<notice_info.num;i++)
	{
		notice_msg_len = notice_msg_len + strlen((char*)notice_info.motice_msg[i].content) + 1;//1:sizeof '/0'
	}
	
	send_len = 3 + sizeof(ui_protocol_header_t) + notice_msg_len + sizeof(ui_history_info_t);//3:7e+crc+7e  

	send_buf = malloc(send_len);

	header = (ui_protocol_header_t*)(send_buf + 1);//1:7e

	ui_protocol_build_header(header,SYSTEM2UI_HISTORY_INFO,notice_msg_len + sizeof(ui_history_info_t));

	history_info = (ui_history_info_t*)(send_buf + 1 + sizeof(ui_protocol_header_t));

	history_info->type = display_type;
	history_info->num = (unsigned char)notice_info.num;
	history_info->start_id = 0;
	history_info->end_id = notice_info.num;

	data_buf = (unsigned char*)(send_buf + 1 + sizeof(ui_protocol_header_t) + sizeof(ui_history_info_t));

	for(i=0;i<notice_info.num;i++)
	{
		*data_buf = notice_info.motice_msg[i].flag;
		data_buf++;
		memcpy(data_buf,notice_info.motice_msg[i].content,strlen((char*)notice_info.motice_msg[i].content) + 1);
		data_buf = data_buf + strlen((char*)notice_info.motice_msg[i].content) + 1;
	}

	ui_protocol_send_data(send_buf,send_len);

	free(send_buf);
	
	DbgFuncExit();

}

/** 
* @brief 	  	发送当前报文到UI
* 
*/
void ui_send_current_notice_list(unsigned char *buf,int len,unsigned char type)
{
	ui_protocol_header_t *header;
	unsigned char *send_buf;
	unsigned char *data_buf;
	int send_len;
	
	DbgFuncEntry();

	
	send_len = 3 + sizeof(ui_protocol_header_t) + len + 1;//3:7e+crc+7e  1:sizeof type
	send_buf = malloc(send_len);
	
	header = (ui_protocol_header_t*)(send_buf + 1);//1:7e
		
	ui_protocol_build_header(header,SYSTEM2UI_CURRENT_NOTICE,len + 1);// 1:sizeof type

	data_buf = (unsigned char *)(send_buf + 1 + sizeof(ui_protocol_header_t));
	*data_buf = type;//显示类型(0:文本提示;1:语音提示;2:文本+语音提示)
	data_buf++;
	memcpy(data_buf,buf,len);

	ui_protocol_send_data(send_buf,send_len);

	free(send_buf);	
	
	DbgFuncExit();
}

/** 
* @brief 	  	发送终端提示报文到UI
* 
*/
void ui_send_terminal_remind(unsigned char *buf,int len)
{
    
    ui_protocol_header_t *header;
	unsigned char *send_buf;
	unsigned char *data_buf;
	int send_len;
	
	DbgFuncEntry();

	send_len = 3 + sizeof(ui_protocol_header_t) + len;//3:7e+crc+7e  
	send_buf = malloc(send_len);
	
	header = (ui_protocol_header_t*)(send_buf + 1);//1:7e
		
	ui_protocol_build_header(header,SYSTEM2UI_TERMINAL_REMIND,len + 1);

	data_buf = (unsigned char *)(send_buf + 1 + sizeof(ui_protocol_header_t));
    
	memcpy(data_buf,buf,len);

	ui_protocol_send_data(send_buf,send_len);

	free(send_buf);	
	
	DbgFuncExit();
}
