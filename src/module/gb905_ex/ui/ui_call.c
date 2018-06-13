/****************************************Copyright (c)**************************************************                         
** File name:			ui_call.c
** Descriptions:		UI���ն˵�ͨ����Ϣ��������
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-22
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
#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_call.h"
#include	"module/gprs/gprs_at_cmd.h"

#include	"app/call/fleety_call.h"

#define		DEBUG_Y
#include	"libs/debug.h"

//UI�·�ͨ����������ݽṹ
typedef struct{
	unsigned char type;			//��������(0:����;1:�绰������;2:�������ĺ���;3:ֱ�Ӽ����������)
	unsigned int phone_num;		//��������
}__packed ui_call_out_t;

//�ն˷���ͨ��״̬�����ݽṹ
typedef  struct
{
    unsigned char start_magic_id;
    ui_protocol_header_t header;
	unsigned char call_state;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed ui_protocol_call_state_t;


#define		ANSWER_CALL		0x01			//����
#define		HANG_UP			0x02			//�Ҷ�

/** 
* @brief 			�绰��������
* @param buf		UI�������ݵĵ�ַ
* @param len		UI�������ݵĳ���
*
* @return	�Ƿ�����ɹ�
*/
unsigned char ui_call_out_treat(unsigned char *buf,int len)
{
	unsigned char result;
	ui_call_out_t *call_out;
	char phone_num[PHONE_NUMBER_MAX_LEN]={0};
	
	DbgFuncEntry();
	
	call_out = (ui_call_out_t *)buf;
	
	memcpy(phone_num,(char *)&call_out->phone_num,strlen((char *)&call_out->phone_num));

	DbgPrintf("call out type = 0x%02x\r\n",call_out->type);
	DbgPrintf("call out phone = 0x%02x\r\n",phone_num);
	
	if(fleety_call_out(phone_num,CALL_TYPE_COMMON))
	{
		result = UI_RESULT_OK;
	}
	else
	{
		result = UI_RESULT_FAIL;
	}
	
	DbgFuncExit();

	return result;
}

/** 
* @brief 			UIͨ�����ƴ���(������Ҷ�)
* @param buf		UI�������ݵĵ�ַ
* @param len		UI�������ݵĳ���
*
* @return	ͨ��Ӧ��
*/
unsigned char ui_call_control_treat(unsigned char *buf,int len)
{
	unsigned char control_type;
	
	DbgFuncEntry();

	control_type = *buf;
    
    DbgPrintf("ui call control type = 0x%02x\r\n",control_type);
    
	switch(control_type)
	{
		case ANSWER_CALL:
			fleety_answer_call();
			break;

		case HANG_UP:
			fleety_hand_up(ACTIVE_HANG_UP);
			break;

		default:
			DbgWarn("this control type is not support!!!\r\n");
			break;
	}

	DbgFuncExit();

	return UI_RESULT_OK;
}

/** 
* @brief 			����������ʾ��UI
* @param phone_num		����绰����
*
*/
void ui_send_call_in(char *phone_num)
{
	ui_protocol_header_t *header;
	unsigned char *send_buf;
	char *data_buf;
	int send_len,phone_num_len;
	
	DbgFuncEntry();

	phone_num_len = strlen(phone_num) + 1; // 1:'/0'
	
	send_len = 3 + sizeof(ui_protocol_header_t) + phone_num_len;//3:7e+crc+7e  1:sizeof display
	send_buf = malloc(send_len);
	memset(send_buf,0x00,send_len);
	
	header = (ui_protocol_header_t*)(send_buf + 1);//1:7e

	ui_protocol_build_header(header,SYSTEM2UI_CALL_IN,phone_num_len);

	data_buf = (char *)(send_buf + 1 + sizeof(ui_protocol_header_t));
	memcpy(data_buf,phone_num,phone_num_len);

	ui_protocol_send_data(send_buf,send_len);

	free(send_buf);
	
	DbgFuncExit();
}


/** 
* @brief 			����ͨ��״̬��UI
* @param state		ͨ��״̬
*
*/
void ui_send_call_state(unsigned char state)
{
	ui_protocol_call_state_t call_state;
	
	DbgFuncEntry();

	DbgPrintf("send call state 0x%02x \r\n",state);
	
	ui_protocol_build_header(&call_state.header,SYSTEM2UI_CALL_STATE,sizeof(unsigned char));
	call_state.call_state = state;
	ui_protocol_send_data((unsigned char *)&call_state,sizeof(ui_protocol_call_state_t));
		
	DbgFuncExit();	
}
