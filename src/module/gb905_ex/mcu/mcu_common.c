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
#include	"module/gb905_ex/mcu/mcu_common.h"
#include	"module/gb905_ex/mcu/mcu_adc.h"
#include	"module/gb905_ex/mcu/mcu_gpio.h"
#include	"module/gb905_ex/mcu/mcu_eval.h"
#include	"module/gb905_ex/mcu/mcu_volumn.h"
#include	"module/gb905_ex/mcu/mcu_heart_beat.h"
#include	"module/gb905_ex/mcu/mcu_version.h"


#include	"middleware/uart/fleety_uart.h"

#define		DEBUG_Y
#include	"libs/debug.h"





static gb905_ex_funcs_t	* mcu_protocol_funcs;



/** 
* @brief 	GB905 EX  Э��������˶Կͻ��˵�ͨ��Ӧ��Ľ���
* @param buf	�յ�Ӧ��Ļ����ַ
* @param len	�յ�Ӧ��Ļ��峤��
*
*/
static void mcu_protocol_common_ack_treat(unsigned char *buf,int len)
{
	mcu_protocol_ack_body_t * ack_body;

	ack_body = (mcu_protocol_ack_body_t *)buf;
	
	if(len != sizeof(mcu_protocol_ack_body_t) || ack_body->result != GB905_EX_RESULT_OK)
	{
		DbgWarn("gb905 ex common ack (server --> terminal) faile!(id = 0x%x)\r\n",ack_body->id);
	}
	else
	{
		DbgGood("gb905 ex common ack (server --> terminal) ok!(id = 0x%x)\r\n",ack_body->id);

		switch (ack_body->id)
		{
			case SYSTEM2MCU_HEART_BEAT:
				mcu_heart_beat_reset();
				break;
				
			default:
				break;
		}		

	}
	
	gb905_ex_debug_ack(ack_body);

}



/** 
* @brief	  ����mcu protocol  ��ͷ
* @param header	  mcu protocol  ͷ�ĵ�ַ
* @param msg_id	  ��Ϣ����ID
* @param msn_len	  ��Ϣ��ĳ���
*
*/
void mcu_protocol_build_header(mcu_protocol_header_t * header, unsigned short msg_id, unsigned short msg_len)
{
	mcu_protocol_funcs->p_build_header_func(header,msg_id,msg_len);
}

/** 
* @brief	  ����mcu protocol  ��ͷ
* @param header	  mcu protocol  ͷ�ĵ�ַ
* @param msg_id	  ��Ϣ����ID
* @param msn_len	  ��Ϣ��ĳ���
*
*/
void mcu_protocol_build_ack(mcu_protocol_ack_t * ack, mcu_protocol_header_t * header, unsigned char result)
{
	mcu_protocol_funcs->p_build_ack_func(ack,SYSTEM2MCU_GENERAL_ACK,header,result);
}

/** 
* @brief 	SYSTEM   ����UI   �˷�������(����ת�壬ħ���ֺ�У��Ĵ���)
* @param buf	�������ݵ����ݻ�������ַ
* @param len  	�������ݵ����ݻ���������
*
*/
void mcu_protocol_send_data(unsigned char * buf, int len)
{
	mcu_protocol_funcs->p_send_data_func(mcu_protocol_funcs->domain,
										mcu_protocol_funcs->index,
										buf,
										len);
}


/** 
* @brief 	mcu ͨѶЭ��Ľ���
* @param buf		��Ž����Ժ����Ϣ����
* @param len  		��Ž�����ǰ����Ϣ����
* 
* @return			��Ž����Ժ����Ϣ���� �����Ȼ���٣�
*/
int mcu_protocol_ayalyze(unsigned char * buf,int len)
{
	int size;

	size = mcu_protocol_funcs->p_analyze_func(mcu_protocol_funcs,buf,len);

	return size;
}
	 

/** 
* @brief 	�ͻ��˷������������˵�Ӧ��
* @param header	GB905  ex ack  Ӧ�����ݽṹ�ĵ�ַ
* @param result  	���ط�������״̬
*
*/
void mcu_protocol_send_ack(mcu_protocol_header_t * header,unsigned char result)
{
	mcu_protocol_ack_t ack;

	DbgFuncEntry();

	mcu_protocol_build_ack(&ack,header,result);
	
	mcu_protocol_send_data((unsigned char *)&ack,sizeof(mcu_protocol_ack_t));

	DbgFuncExit();
}

/** 
   * @brief 	mcu  ͨѶЭ�����Ϣ��������
   * @param msg		�����Ϣ�Ļ������ݽṹ
   * 
   * @return			�����Ƿ�ɹ�
   */
bool mcu_parse_protocol(buff_mgr_t * msg)
{
	mcu_protocol_header_t * header;
	unsigned short msg_no;
	unsigned char result = GB905_EX_RESULT_OK;

	DbgFuncEntry();

	header = (mcu_protocol_header_t *)(msg->buf + 1);

	msg_no = header->msg_id;

	DbgPrintf("mcu_msg_no = 0x%x\r\n",msg_no);

	switch (msg_no)
	{
		case MCU2SYSTEM_GENERAL_ACK:
			mcu_protocol_common_ack_treat(msg->buf + 1 + sizeof(mcu_protocol_header_t),header->msg_len);
			break;

		case MCU2SYSTEM_GPIO_IN:
			result = mcu_gpio_in_treat(msg->buf,msg->len);
			mcu_protocol_send_ack(header,result);
			break;

		case MCU2SYSTEM_ADC_REP:
			mcu_get_adc_treat(msg->buf,msg->len);
			break;
			
		case MCU2SYSTEM_EVAL_IN:
			result = mcu_get_evaluate_treat(msg->buf,msg->len);
			mcu_protocol_send_ack(header,result);
			break;

		case MCU2SYSTEM_VOLUMN_IN:
			result = mcu_volumn_treat(msg->buf,msg->len);
			mcu_protocol_send_ack(header,result);
			break;

		case MCU2SYSTEM_GPIO_MAP_REP:
			mcu_gpio_map_treat(msg->buf,msg->len);
			break;

        case MCU2SYSTEM_VERSION_REP:
            mcu_get_version_treat(msg->buf,msg->len);
            break;
		
		default:
			break;
	}

	DbgFuncExit();
	
	return true;
}

 


//-----
/** 
* @brief			mcu  ͨѶЭ��ĳ�ʼ��(����һ��gb905 ex   �������)
* 
*/
void mcu_protocol_init(void)
{
	mcu_protocol_funcs = gb905_ex_base_create(UART_DOMAIN,MCU_UART,mcu_parse_protocol);
}

void mcu_protocol_deinit(void)
{
	gb905_ex_base_release(mcu_protocol_funcs);
}


