/****************************************Copyright (c)**************************************************                         
** File name:			ui_phone_book.c
** Descriptions:		
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

#include	"module/gb905/phone_book/gb905_phone_book.h"
#include	"module/gb905_ex/ui/ui_history_info.h"
#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_phone_book.h"

#include	"middleware/info/phone_book.h"

#define		DEBUG_Y
#include	"libs/debug.h"


/** 
* @brief 	  	发送电话薄列表到UI
* 
*/
void ui_send_phone_book_list(unsigned char display_type)
{
	ui_protocol_header_t *header;
	ui_history_info_t *history_info;
	save_phone_book_t phone_book_info;
	unsigned char *send_buf;
	unsigned char *data_buf;
	int send_len = 0;
	int phone_book_msg_len = 0;
	int i;
	
	DbgFuncEntry();

	phone_book_get_info(&phone_book_info);

	phone_book_msg_len = phone_book_info.num*(sizeof(unsigned char));

	for(i=0;i<phone_book_info.num;i++)
	{
		phone_book_msg_len = phone_book_msg_len + strlen((char*)phone_book_info.phone_book_msg[i].phone_num) + 1;//1:sizeof '/0'
		phone_book_msg_len = phone_book_msg_len + strlen((char*)phone_book_info.phone_book_msg[i].name) + 1;//1:sizeof '/0'
	}
	
	send_len = 3 + sizeof(ui_protocol_header_t) + phone_book_msg_len + sizeof(ui_history_info_t);//3:7e+crc+7e 

	send_buf = malloc(send_len);

	header = (ui_protocol_header_t*)(send_buf + 1);//1:7e

	ui_protocol_build_header(header,SYSTEM2UI_HISTORY_INFO,phone_book_msg_len + sizeof(ui_history_info_t));

	history_info = (ui_history_info_t*)(send_buf + 1 + sizeof(ui_protocol_header_t));//1:7e

	history_info->type = display_type;
	history_info->num = (unsigned char)phone_book_info.num;
	history_info->start_id = 0;
	history_info->end_id = phone_book_info.num;

	data_buf = (unsigned char*)(send_buf + 1 + sizeof(ui_protocol_header_t) + sizeof(ui_history_info_t));//1:7e

	for(i=0;i<phone_book_info.num;i++)
	{
		*data_buf = phone_book_info.phone_book_msg[i].flag;
		data_buf++;
		memcpy(data_buf,phone_book_info.phone_book_msg[i].phone_num,strlen((char*)phone_book_info.phone_book_msg[i].phone_num) + 1);//1:sizeof '/0'
		data_buf = data_buf + strlen((char*)phone_book_info.phone_book_msg[i].phone_num) + 1;//1:sizeof '/0'
		memcpy(data_buf,phone_book_info.phone_book_msg[i].name,strlen((char *)phone_book_info.phone_book_msg[i].name) + 1);//1:sizeof '/0'
		data_buf = data_buf + strlen((char *)phone_book_info.phone_book_msg[i].name) + 1;//1:sizeof '/0'
	}

	ui_protocol_send_data(send_buf,send_len);

	free(send_buf);
	
	DbgFuncExit();

}

