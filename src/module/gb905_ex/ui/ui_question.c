/****************************************Copyright (c)**************************************************                         
** File name:			ui_question.c
** Descriptions:		
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-24
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

#include	"misc/endian.h"

#include	"module/gb905/question/gb905_question.h"
#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_question.h"


#define		DEBUG_Y
#include	"libs/debug.h"

//ui发送的提问应答数据结构
typedef struct{
	unsigned int question_id;			//问题ID
	unsigned char answer_id;			//答案ID
}__packed ui_question_ack_t;

/** 
* @brief 	  	终端发送提问信息到UI
* 
*/
void ui_send_question_info(unsigned char *buf,int len)
{
	ui_protocol_header_t *header;
	unsigned char *send_buf;
	unsigned char *data_buf;
	int send_len;
	
	DbgFuncEntry();

	send_len = 3 + sizeof(ui_protocol_header_t) + len;//3:7e+crc+7e  1:sizeof display
	send_buf = malloc(send_len);
	
	header = (ui_protocol_header_t*)(send_buf + 1);//1:7e
		
	ui_protocol_build_header(header,SYSTEM2UI_CURRENT_QUESTION,len);

	data_buf = (unsigned char *)(send_buf + 1 + sizeof(ui_protocol_header_t));
	memcpy(data_buf,buf,len);

	ui_protocol_send_data(send_buf,send_len);

	free(send_buf);	
	
	DbgFuncExit();
	
}


/** 
* @brief 	  	UI发送提问应答到终端
* 
*/
unsigned char ui_question_ack_treat(unsigned char *buf,int len)
{
	ui_question_ack_t *question_ack;

	DbgFuncEntry();

	question_ack = (ui_question_ack_t *)buf;

	DbgPrintf("ui question ack :question id %d\r\n",question_ack->question_id);
	DbgPrintf("ui question ack :answer id 0x%02x\r\n",question_ack->answer_id);

	gb905_question_ack_treat(question_ack->question_id,question_ack->answer_id);
	
	DbgFuncExit();

	return UI_RESULT_OK;
}
