/****************************************Copyright (c)**************************************************                         
** File name:			mcu_common.c
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

#include	"module/gb905_ex/gb905_ex_common.h"
#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_gps.h"
#include	"module/gb905_ex/ui/ui_imei.h"
#include	"module/gb905_ex/ui/ui_heart_beat.h"



#include	"middleware/socket/fleety_socket.h"

#define		DEBUG_Y
#include	"libs/debug.h"





static gb905_ex_funcs_t	* ui_protocol_funcs;



/** 
* @brief 	GB905 EX  Э��������˶Կͻ��˵�ͨ��Ӧ��Ľ���
* @param buf	�յ�Ӧ��Ļ����ַ
* @param len	�յ�Ӧ��Ļ��峤��
*
*/
static void ui_protocol_common_ack_treat(unsigned char *buf,int len)
{
	ui_protocol_ack_body_t * ack_body;

	ack_body = (ui_protocol_ack_body_t *)buf;
	
	if(len != sizeof(ui_protocol_ack_body_t) || ack_body->result != GB905_EX_RESULT_OK)
	{
		DbgWarn("ui socket common ack (server --> terminal) faile!(id = 0x%x)\r\n",ack_body->id);
	}
	else
	{
		DbgGood("ui socket common ack (server --> terminal) ok!(id = 0x%x)\r\n",ack_body->id);

		switch (ack_body->id)
		{
			case SYSTEM2UI_HEART_BEAT:
				ui_heart_beat_reset();
				break;
				
			default:
				break;
		}

		
	}
	
	gb905_ex_debug_ack(ack_body);

}



/** 
* @brief	  ����		ui protocol  ��ͷ
* @param header	  	ui protocol  ͷ�ĵ�ַ
* @param msg_id	  	��Ϣ����ID
* @param msn_len	  	��Ϣ��ĳ���
*
*/
void ui_protocol_build_header(ui_protocol_header_t * header, unsigned short msg_id, unsigned short msg_len)
{
	ui_protocol_funcs->p_build_header_func(header,msg_id,msg_len);
}

/** 
* @brief	  ����mcu protocol  ��ͷ
* @param header	  mcu protocol  ͷ�ĵ�ַ
* @param msg_id	  ��Ϣ����ID
* @param msn_len	  ��Ϣ��ĳ���
*
*/
void ui_protocol_build_ack(ui_protocol_ack_t * ack, ui_protocol_header_t * header, unsigned char result)
{
	ui_protocol_funcs->p_build_ack_func(ack,SYSTEM2UI_GENERAL_ACK,header,result);
}


void ui_protocol_send_data(unsigned char * buf, int len)
{
	ui_protocol_funcs->p_send_data_func(ui_protocol_funcs->domain,
										ui_protocol_funcs->index,
										buf,
										len);
}


/** 
* @brief 			ui    ͨѶ  Э��Ľ���
* @param buf		��Ž����Ժ����Ϣ����
* @param len  		��Ž�����ǰ����Ϣ����
* 
* @return			��Ž����Ժ����Ϣ���� �����Ȼ���٣�
*/
int ui_protocol_ayalyze(unsigned char * buf,int len)
{
	int size;
	
	size = ui_protocol_funcs->p_analyze_func(ui_protocol_funcs,buf,len);

	return size;
}
	 

/** 
* @brief 	�ͻ��˷������������˵�Ӧ��
* @param header	ui protocol ack  Ӧ�����ݽṹ�ĵ�ַ
* @param result  	���ط�������״̬
*
*/
void ui_protocol_send_ack(ui_protocol_header_t * header,unsigned char result)
{
	ui_protocol_ack_t ack;

	DbgFuncEntry();

	ui_protocol_build_ack(&ack,header,result);
	
	ui_protocol_send_data((unsigned char *)&ack,sizeof(ui_protocol_ack_t));

	DbgFuncExit();
}

/** 
   * @brief 	  	ui  ͨѶЭ�����Ϣ��������
   * @param msg	�����Ϣ�Ļ������ݽṹ
   * 
   * @return		�����Ƿ�ɹ�
   */
bool ui_parse_protocol(buff_mgr_t * msg)
{
	ui_protocol_header_t * header;
	unsigned short msg_no;
	//unsigned char result = GB905_EX_RESULT_OK;

	DbgFuncEntry();

	header = (ui_protocol_header_t *)(msg->buf + 1);

	msg_no = header->msg_id;

	DbgPrintf("msg_no = 0x%x\r\n",msg_no);

	switch (msg_no)
	{
		case UI2SYSTEM_GENERAL_ACK:
			ui_protocol_common_ack_treat(msg->buf + 1 + sizeof(ui_protocol_header_t),header->msg_len);
			break;

		case UI2SYSTEM_GPS_INFO:
			ui_gps_info_treat(msg->buf + 1 + sizeof(ui_protocol_header_t),header->msg_len);
			break;

		case UI2SYSTEM_IMEI_REP:
			ui_imei_info_treat(msg->buf + 1 + sizeof(ui_protocol_header_t),header->msg_len);
			break;
		
		default:
			break;
	}

	DbgFuncExit();
	
	return true;
}

 


//-----
/** 
* @brief	ui  ͨѶЭ��ĳ�ʼ��(����һ��gb905 ex   �������)
* 
*/
void ui_protocol_init(void)
{
	ui_protocol_funcs = gb905_ex_base_create(SOCKET_DOMAIN,UI_SOCKET,ui_parse_protocol);
}

void ui_protocol_deinit(void)
{
	gb905_ex_base_release(ui_protocol_funcs);
}


