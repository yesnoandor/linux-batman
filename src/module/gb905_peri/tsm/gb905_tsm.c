/****************************************Copyright (c)**************************************************                         
** File name:			gb905_tsm.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-21
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
#include	"misc/misc.h"
#include	"misc/check.h"
#include	"misc/misc.h"



#include	"module/gb905/gb905_common.h"
//#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905/inspection/gb905_inspection.h"
#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/meter/gb905_meter.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"
#include	"module/gb905_peri/tsm/gb905_tsm.h"

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_notice.h"

#include	"module/gb905_update/gb905_update_common.h"
#include	"module/gb905_update/tsm/gb905_update_tsm.h"


#include	"middleware/info/status.h"
#include	"middleware/info/setting.h"
#include	"middleware/info/update.h"
#include	"middleware/info/driver.h"
#include	"middleware/info/notice.h"
#include	"middleware/info/product.h"

#include	"middleware/uart/fleety_uart.h"

#define		DEBUG_Y
#include	"libs/debug.h"


#define		TSM_TYPE_ID					0x03

#define		TSM_COMMAND_QUERY_STATUS	0x0000		// ��ѯTSM 	 ģ������״̬
#define		TSM_COMMAND_RESET			0x0001		// ��λTSM  ģ�� 
#define		TSM_COMMAND_SET_BAUDRATE	0x0002		// ��λTSM  ģ�鲨����
#define		TSM_COMMAND_HEART_BEAT		0x0004		// ����ָ��
#define		TSM_COMMAND_UPGRADE			0x00ff		// ����ָ��


#define		TSM_COMMAND_DRIVER_INFO		0x0020		// ��ȡ��ʻԱ��Ϣ
#define		TSM_COMMAND_EX_DRIVER_INFO	0x0021		// ��ȡ��ʻԱ��չ��Ϣ
#define		TSM_COMMAND_TAXI_INFO		0x0023		// ��ȡ���⳵Ӧ����Ϣ
#define		TSM_COMMAND_DECRYPTION 	 	0x0040		// ���ݽ���


#define		TSM_COMMAND_CARD_SET_BLACKLIST	0x009C		// ���ÿ�ƬΪ������
#define		TSM_COMMAND_CARD_STATUS			0x009D		// ��ѯһ��ͨ״̬�Լ����
#define		TSM_COMMAND_CARD_DEDUCT_MONEY	0x009E		// ���⳵��ֵ


#define		TSM_ACK_RESULT_OK			0x0090		
#define		TSM_ACK_RESULT_FAIL			0x16FF		//

#define		TSM_ACK_RESET_NOT_SUPPORT	0x0190		// ��λ�豸��֧��

#define		TSM_ACK_UPGRADE_NOT_SUPPORT	0x01FF		// �����豸��֧��
#define		TSM_ACK_UPGRADE_NOT_READY	0x02FF		// ͬһ�汾����������
#define		TSM_ACK_UPGRADE_MISMATCH	0x03FF		// ��ʶ��Ϣ��һ�£��޷�����

#define		TSM_ACK_DECRYP_MAC_ERR		0x20F0		// MAC  ��֤ʧ�ܣ�����ʧ��
#define		TSM_ACK_DECRYP_KEY_ERR		0x21F0		// MAC ������Կ����ȷ���޷�����
#define		TSM_ACK_DECRYP_LEN_ERR		0x22F0		// ���ݳ��Ȳ���ȷ������ʧ��


#define	TSM_REMIND_INFO	"��Ƭ�����ڣ���ˢ˾������"

static int tsm_heart_beat_count = 0;
#define TSM_HEART_BEAT_THREHOLD         5

static void gb905_tsm_build_header(gb905_peri_header_t* header,unsigned short cmd,unsigned short len)
{
	product_params_t product_params;
	
	get_product_params((unsigned char *)&product_params);

	header->type = TSM_TYPE_ID;
	header->vendor = product_params.tsm_vendor_id;
	gb905_peri_build_header(header,cmd,len);
}



static void gb905_tsm_build_update_body(gb905_tsm_update_body_t * body)
{
	gb905_update_info_t update_info;
			
	DbgFuncEntry();

	get_update_info((unsigned char *)&update_info);
	
	body->vendor_id = update_info.base_info.vendor_id;
	body->hw_version = update_info.hw_version;
	body->main_sw_version = update_info.sw_version[0];
	body->vice_sw_version = update_info.sw_version[1];

	DbgFuncExit();
}

/** 
* @brief 	TSM  ģ���յ�����Ӧ��Ĵ���
*
*/
static void gb905_tsm_update_ack_treat(unsigned char *buf,int len)
{
	gb905_update_tsm_start();
}


/** 
* @brief 	����TSM  ��ͨ���������ݽṹ��������
* @param cmd		TSM   ��ͨ������(TSM_COMMAND_RESET/)
* 
* @return			�Ƿ�ɹ�
*/
static bool gb905_tsm_common_command(unsigned short cmd)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* tsm_header;
	gb905_peri_tail_t* tsm_tail;

	unsigned char xor;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("tsm state query memory malloc failed!\r\n");
		return false;
	}
	memset(buf,0x00,len);
	
	tsm_header = (gb905_peri_header_t*)buf;
	gb905_tsm_build_header(tsm_header,cmd,sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));
	
	tsm_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len), sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(tsm_tail,xor);
	
	fleety_uart_send(TSM_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;
}


/** 
* @brief 	����TSM    ģ�鷢�͹�����ͨ��Ӧ��
* @param buf		���TSM   ��Ϣ��Ļ����ַ
* @param len		���TSM   ��Ϣ��Ļ��泤��
* 
*/
static void gb905_tsm_common_ack_treat(unsigned char * buf)
{
	gb905_tsm_common_ack_t * ack;

	DbgFuncEntry();
	
	ack = (gb905_tsm_common_ack_t *)buf;

	DbgPrintf("tsm ack vendor = 0x%x \r\n",ack->vendor_id);
	DbgPrintf("tsm ack cmd = 0x%x \r\n",ack->cmd);
	DbgPrintf("tsm ack result = 0x%x \r\n",ack->result);

	DbgFuncExit();
}

/** 
* @brief 	����TSM    ģ���MCU  ����Ĳ�ѯ״̬Ӧ��
* @param buf		���TSM   ��Ϣ��Ļ����ַ
* @param len		���TSM   ��Ϣ��Ļ��泤��
* 
*/
static void gb905_tsm_query_state_treat(unsigned char * buf,unsigned short len)
{
	unsigned short i;
	gb905_tsm_status_t* tsm_status;

	DbgFuncEntry();
	
	tsm_status = (gb905_tsm_status_t*)buf;

	for(i = 0;i < len;i++)
	{
		DbgPrintf(" 0x%02x ",buf[i]);
	}

	DbgPrintf("++++++++++++++++++++++++++++++++++++\r\n");
	DbgPrintf("tsm_status->hard_version = 0x%02x\r\n",tsm_status->hard_version);
	DbgPrintf("tsm_status->soft_version[0] = 0x%02x",tsm_status->soft_version[0]);
	DbgPrintf("tsm_status->soft_version[1] = 0x%02x\r\n",tsm_status->soft_version[1]);
	DbgPrintf("tsm_status->device_status = 0x%02x\r\n",tsm_status->device_status);
	DbgPrintf("------------------------------------\r\n");

	//��Ѳ��״̬(�������ж��Ƿ����ӳ�ͻ)
	if(get_tsm_insp_status())
	{
		gb905_inspection_set_ack_info(GB905_DEVICE_TSM,buf,len);
		set_tsm_insp_status(false);
	}

    tsm_heart_beat_count = 0;
    
	DbgFuncExit();
}

/** 
* @brief 	����TSM    ģ�������Э��
* @param buf		���TSM   ��Ϣ��Ļ����ַ
* @param len		���TSM   ��Ϣ��Ļ��泤��
* 
*/
static void gb905_tsm_parse_heart_beat(unsigned char *buf,unsigned short len)
{
	gb905_tsm_heart_beat_t *tsm_heart_beat;
	DbgFuncEntry();

	tsm_heart_beat = (gb905_tsm_heart_beat_t *)buf;

	DbgPrintf(" extend info = %s",&tsm_heart_beat->extend);

	
	DbgFuncExit();
}

/** 
* @brief 	����TSM    ģ�������Ӧ��
* 
* @return			Ӧ���Ƿ�ɹ�
*/
static bool gb905_tsm_heart_beat_ack(void)
{
	gb905_tsm_heart_beat_ack_t tsm_heart_beat_ack;

	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* tsm_header;
	gb905_peri_tail_t* tsm_tail;

	unsigned char xor;
	
	DbgFuncEntry();

	len = sizeof(gb905_peri_header_t) + sizeof(gb905_tsm_heart_beat_ack_t) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("tsm heart beat ack Memory Malloc Failed!\r\n");
		return false;
	}
	memset(buf,0x00,len);
	
	tsm_header = (gb905_peri_header_t*)buf;
	gb905_tsm_build_header(tsm_header,TSM_COMMAND_HEART_BEAT,sizeof(gb905_tsm_heart_beat_ack_t) + sizeof(gb905_peri_header_t) -offsetof(gb905_peri_header_t,type));

	gb905_peri_build_timestamp(&tsm_heart_beat_ack.now_time);
	tsm_heart_beat_ack.internet_content_status = 0x00;
	tsm_heart_beat_ack.working_status = 0x00;
	memcpy(tsm_heart_beat_ack.car_num,"A12345",sizeof("A12345"));
	memcpy(tsm_heart_beat_ack.certification_num,"12345678900000000",sizeof("123456789000000000"));

	tsm_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + sizeof(gb905_peri_bcd_timestamp_t));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_bcd_timestamp_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(tsm_tail,xor);
	
	fleety_uart_send(TSM_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;
}

/** 
* @brief 	���յ�TSM    ģ�鷵�ص���ʻ֤��Ϣ�Ĵ���
* 
* @return			Ӧ���Ƿ�ɹ�
*/
static void gb905_tsm_driver_info_treat(unsigned char* buf,unsigned short data_len)
{
	//unsigned short i;
	gb905_tsm_driver_info_t* driver_info;
	//get_tsm_driver_info_new_t tsm_driver_info;
	
	
	DbgFuncEntry();
	DbgGood("tsm get driver info :\r\n");

	#if 0
	memset(&tsm_driver_info,0x00,sizeof(get_tsm_driver_info_new_t));
	for(i = 0;i < data_len;i++)
	{
		DbgPrintf(" 0x%02x ",buf[i]);
	}
	DbgPrintf("\r\n");
	#endif
	
	driver_info = (gb905_tsm_driver_info_t*)buf;

	#if 0
	//tsm_driver_info.get_info_ok = 1;
	memcpy(tsm_driver_info.driver_info.driver_name,driver_info->driver_name,sizeof(driver_info->driver_name));
	memcpy(tsm_driver_info.driver_info.quailficantion_number,driver_info->quailficantion_number,sizeof(driver_info->quailficantion_number));
	memcpy(tsm_driver_info.driver_info.license_organ,driver_info->license_organ,sizeof(driver_info->license_organ));
	memcpy(tsm_driver_info.driver_info.useful_life.bcd,driver_info->useful_life,sizeof(driver_info->useful_life));
	memcpy(tsm_driver_info.driver_info.card_number,driver_info->card_number,sizeof(driver_info->card_number));
	set_tsm_driver_info_new(&tsm_driver_info);
	#endif
	
	DbgPrintf("tsm driver info : type = 0x%02x \r\n",driver_info->ic_card_type);
	DbgPrintf("tsm driver info : country_version = 0x%02x \r\n",driver_info->country_version);
	DbgPrintf("tsm driver info : place_version = 0x%02x \r\n",driver_info->place_version);
	DbgPrintf("tsm driver info : driver_name = %s \r\n",driver_info->driver_name);
	DbgPrintf("tsm driver info : driver_address = %s \r\n",driver_info->driver_address);
	DbgPrintf("tsm driver info : quailficantion_number = %s\r\n",driver_info->quailficantion_number);
	DbgPrintf("tsm driver info : quailficantion_type = %s\r\n",driver_info->quailficantion_type);
	DbgPrintf("tsm driver info : license_organ = %s\r\n ",driver_info->license_organ);
	DbgPrintf("tsm driver info : useful_life = %02x%02x-%02x-%02x \r\n",driver_info->useful_life[0],driver_info->useful_life[1],driver_info->useful_life[2],driver_info->useful_life[3]);
	DbgPrintf("tsm driver info : card_number = %02x%02x%02x%02x%02x%02x\r\n",driver_info->card_number[0],driver_info->card_number[1],driver_info->card_number[2],\
		driver_info->card_number[3],driver_info->card_number[4],driver_info->card_number[5]);
	DbgPrintf("tsm driver info : return_result = 0x%02x 0x%02x \r\n ",driver_info->return_result[0],driver_info->return_result[1]);

	set_driver_info(buf);
	
	set_driver_license_number(driver_info->quailficantion_number,sizeof(driver_info->quailficantion_number));

	DbgFuncExit();
}

/** 
* @brief 	��TSM  ģ���л�ȡ�����Ժ������
* @param buf		���TSM   ��Ϣ��Ļ����ַ
* @param len		���TSM   ��Ϣ��Ļ��泤��
*
*/
static void gb905_tsm_decryption_ack_treat(unsigned char* buf,unsigned short len)
{
	unsigned short result;
	
	DbgFuncEntry();
	
	result = *(unsigned short *)(buf + len - 2);
	DbgPrintf(" result = 0x%x \r\n",result);

	switch(result)
	{
		case TSM_ACK_RESULT_OK:
			DbgPrintf(" data decryption success \r\n");
			gb905_toplight_set_security(buf,len - 2);
			break;

		case TSM_ACK_DECRYP_MAC_ERR:
			DbgError("decryption mac error!\r\n");
			break;

		case TSM_ACK_DECRYP_KEY_ERR:
			DbgError("decryption key error!\r\n");
			break;

		case TSM_ACK_DECRYP_LEN_ERR:
			DbgError("decryption len error !\r\n");
			break;
			
		default:
			DbgError("don't support this result!\r\n");
			break;
	}

	DbgFuncExit();
}

/** 
* @brief 	������TSM   ģ���л�ȡһ��ͨ��״̬������Ӧ��
* @param buf		���TSM   ��Ϣ��Ļ����ַ
* @param len		���TSM   ��Ϣ��Ļ��泤��
*
*/
static void gb905_tsm_card_status_ack_treat(unsigned char* buf,unsigned short len)
{
	unsigned short i;
	//gb905_tsm_status_balance_t* data;

	DbgFuncEntry();
	
	DbgPrintf("tsm_status_balance_ack :\r\n");
	for(i = 0;i < len;i++)
	{
		DbgPrintf(" 0x%02x ",buf[i]);
	}
	DbgPrintf("\r\n");


	//data = (gb905_tsm_status_balance_t*)buf;

	DbgFuncExit();
}

/** 
* @brief 	GB905  TSM  ģ��Э�����Ϣͷ�������
* @param msg		�����Ϣ�Ļ������ݽṹ
* 
* @return			�����Ƿ�ɹ�
*/
static bool gb905_tsm_parse_header(buff_mgr_t * msg)
{
	bool ret = true;
	gb905_peri_header_t * header;
	product_params_t product_params;
	
	DbgFuncEntry();

	header = (gb905_peri_header_t *)(msg->buf);

	header->len = EndianReverse16(header->len);
	header->cmd = EndianReverse16(header->cmd);
	
	DbgPrintf("len = 0x%04x\r\n",header->len);
	DbgPrintf("cmd = 0x%04x\r\n",header->cmd);

	// len : start from device_info
	if(header->len != msg->len - (int)offsetof(gb905_peri_header_t,type) - sizeof(gb905_peri_tail_t))
	{
		DbgError("msg len is invaild!\r\n");
				
		ret = false;
		return ret;
	}

	DbgPrintf("type = 0x%04x\r\n",header->type);
	
	if(header->type != TSM_TYPE_ID)
	{
		DbgError("tsm terminal type error!\r\n");
				
		ret = false;
		return ret;
	}

	
	get_product_params((unsigned char *)&product_params);

	DbgPrintf("vendor = 0x%04x\r\n",header->vendor);
	
	if( header->vendor != product_params.tsm_vendor_id)
	{
		DbgError("tsm ternimal id is error!\r\n");
		
		ret = false;
		return ret;
	}
	
	DbgFuncExit();
	
	return ret;
}

/** 
* @brief 	GB905  TSM   ģ��Э�����Ϣ��������
* @param msg		�����Ϣ�Ļ������ݽṹ
* 
* @return			�����Ƿ�ɹ�
*/
static int gb905_tsm_parse_protocol(buff_mgr_t * msg)
{
		
	bool ret = 0;
	
	gb905_peri_header_t * header;
	unsigned char * ack;
	unsigned char * tsm_data;
	unsigned short tsm_data_len;

	DbgFuncEntry();
		
	header = (gb905_peri_header_t *)(msg->buf);
	tsm_data = (unsigned char *)(msg->buf + sizeof(gb905_peri_header_t));
	tsm_data_len = header->len - 4;
	
#if 1
	{
		int i;
		DbgPrintf("parse tsm date is :");
		for(i = 0;i < header->len + 7;i++)
		{
			DbgPrintf("0x%02x ",msg->buf[i]);
		}
		DbgPrintf("\r\n");
	}
#endif
		
	DbgPrintf("cmd = 0x%04x\r\n",header->cmd);
		
	switch(header->cmd)
	{
		case TSM_COMMAND_QUERY_STATUS:
			gb905_tsm_query_state_treat(tsm_data,tsm_data_len);
			//inspection_tsm_state(tsm_data,tsm_data_len);
			break;

		case TSM_COMMAND_HEART_BEAT:
			gb905_tsm_parse_heart_beat(tsm_data,tsm_data_len);
			gb905_tsm_heart_beat_ack();
			break;

		case TSM_COMMAND_RESET:
		case TSM_COMMAND_SET_BAUDRATE:
			ack = msg->buf + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,vendor);
			gb905_tsm_common_ack_treat(ack);
			break;

		
		
		case TSM_COMMAND_UPGRADE:
			gb905_tsm_update_ack_treat(tsm_data,tsm_data_len);
			break;
			
		case TSM_COMMAND_DRIVER_INFO:
			if(tsm_data_len == 0x02)		// --???? 
			{
				DbgError(" can not find the card  !\r\n");
				DbgError(" tsm get driver information return error !\r\n");
                ui_send_terminal_remind((unsigned char*)TSM_REMIND_INFO,sizeof(TSM_REMIND_INFO));
            }
			else
			{
				gb905_tsm_driver_info_treat(tsm_data,tsm_data_len);
				gb905_meter_open_close_ack();
			}
			break;

		case TSM_COMMAND_DECRYPTION:
			gb905_tsm_decryption_ack_treat(tsm_data,tsm_data_len);
			break;

		case TSM_COMMAND_CARD_STATUS:
			gb905_tsm_card_status_ack_treat(tsm_data,tsm_data_len);
			break;

		default:
			DbgError(" can not support this cmd(0x%x)!\r\n",header->cmd);
			break;			
	}

	DbgFuncExit();

	return ret;
}
	





/*
*  @brief 		����һ��������TSM   ��Ϣ
*  @param msg 	���������Ϣ�����buf
* 
*  @return		������Э�飬��Ҫƫ�Ƶĳ���
*/
static int gb905_tsm_parse_a_full_msg(buff_mgr_t * msg)
{

	int ret;

	DbgFuncEntry();

	ret = gb905_tsm_parse_header(msg);
	if( ret < 0){
		return ret;
	}

	ret = gb905_tsm_parse_protocol(msg);

	DbgFuncExit();

	return ret;

}

//------
/** 
* @brief 	��ѯTSM  ģ������״̬
*
* @return �Ƿ���������
*/
bool gb905_tsm_query_state(void)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* tsm_header;
	gb905_peri_tail_t* tsm_tail;
	gb905_peri_bcd_timestamp_t * tsm_timestamp;

	unsigned char xor;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(gb905_peri_bcd_timestamp_t) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("tsm state query memory malloc failed!\r\n");
		return false;
	}
	memset(buf,0x00,len);
	
	tsm_header = (gb905_peri_header_t*)buf;
	gb905_tsm_build_header(tsm_header,TSM_COMMAND_QUERY_STATUS,sizeof(gb905_peri_bcd_timestamp_t) + sizeof(gb905_peri_header_t) -offsetof(gb905_peri_header_t,type));

	tsm_timestamp = (gb905_peri_bcd_timestamp_t *)(buf + sizeof(gb905_peri_header_t));
	gb905_peri_build_timestamp(tsm_timestamp);
	
	tsm_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + sizeof(gb905_peri_bcd_timestamp_t));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_bcd_timestamp_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(tsm_tail,xor);
	
	fleety_uart_send(TSM_UART,buf,len);

	free(buf);

    tsm_heart_beat_count++;
        
	DbgFuncExit();

	return true;
}


/** 
* @brief 	��λTSM    ģ��
*
* @return �Ƿ���������
*/
bool gb905_tsm_reset(void)
{
	bool ret;
	
	ret = gb905_tsm_common_command(TSM_COMMAND_RESET);

	return ret;
}

/** 
* @brief 	����TSM    ģ�鲨����
* @param baud	������ 
*
* @return �Ƿ���������
*/
bool gb905_tsm_set_baudrate(unsigned char baud)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* tsm_header;
	gb905_peri_tail_t* tsm_tail;

	unsigned char* baud_rate;
	unsigned char xor;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(gb905_peri_tail_t) + sizeof(baud);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("tsm set baudrate memory malloc failed!\r\n");
		return false;
	}
	memset(buf,0x00,len);
	
	tsm_header = (gb905_peri_header_t*)buf;
	gb905_tsm_build_header(tsm_header,TSM_COMMAND_SET_BAUDRATE,sizeof(baud) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));

	baud_rate = (unsigned char*)(buf + sizeof(gb905_peri_header_t));
	*baud_rate = baud;
	
	tsm_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + sizeof(baud));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(baud) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(tsm_tail,xor);
	
	fleety_uart_send(TSM_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;
}

/** 
* @brief 	Զ�̹̼�����
*
* @return �Ƿ���������
*/
bool gb905_tsm_upgrade(gb905_tsm_upgrade_t* tsm_date)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* tsm_header;
	gb905_peri_tail_t* tsm_tail;

	//gb905_tsm_upgrade_t* tsm_upgrade;
	unsigned char * tsm_upgrade;
	
	unsigned char xor;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(gb905_peri_tail_t) + sizeof(gb905_tsm_upgrade_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("tsm upgrade memory malloc failed!\r\n");
		return false;
	}
	memset(buf,0x00,len);
	
	tsm_header = (gb905_peri_header_t*)buf;
	gb905_tsm_build_header(tsm_header,TSM_COMMAND_UPGRADE,sizeof(gb905_tsm_upgrade_t) + sizeof(gb905_peri_header_t) -offsetof(gb905_peri_header_t,type));

	tsm_upgrade = (unsigned char*)(buf + sizeof(gb905_peri_header_t));
	memcpy(tsm_upgrade,tsm_date,sizeof(gb905_tsm_upgrade_t));
		
	tsm_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + sizeof(gb905_tsm_upgrade_t));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_tsm_upgrade_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(tsm_tail,xor);
	
	fleety_uart_send(TSM_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;
}

/** 
* @brief 	��ȡ��ʻԱ��Ϣ
*
* @return �Ƿ���������
*/
bool gb905_tsm_get_driver_info(void)
{
	bool ret;
	
	ret = gb905_tsm_common_command(TSM_COMMAND_DRIVER_INFO);

	return ret;
}


/** 
* @brief 	����������
*
* @return �Ƿ���������
*/
bool gb905_tsm_decryption(unsigned char* data,unsigned short data_len)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* tsm_header;
	gb905_peri_tail_t* tsm_tail;

	unsigned char xor;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(gb905_peri_tail_t) + data_len;
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("tsm data decryption memory malloc failed!\r\n");
		return false;
	}
	memset(buf,0x00,len);
	
	tsm_header = (gb905_peri_header_t*)buf;
	gb905_tsm_build_header(tsm_header,TSM_COMMAND_DECRYPTION,data_len + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));

	memcpy(buf + sizeof(gb905_peri_header_t),data,data_len);
		
	tsm_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + data_len);
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),data_len + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(tsm_tail,xor);
	
	fleety_uart_send(TSM_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;

}



/** 
* @brief 	��ѯһ��ͨ״̬�����
*
* @return �Ƿ���������
*/
bool gb905_tsm_card_status_balance(void)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* tsm_header;
	gb905_peri_tail_t* tsm_tail;
	gb905_peri_bcd_timestamp_t * tsm_timestamp;

	unsigned char xor;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(gb905_peri_bcd_timestamp_t) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("tsm state query memory malloc failed!\r\n");
		return false;
	}
	memset(buf,0x00,len);
	
	
	tsm_header = (gb905_peri_header_t*)buf;
	gb905_tsm_build_header(tsm_header,TSM_COMMAND_CARD_STATUS,sizeof(gb905_peri_bcd_timestamp_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));

	tsm_timestamp = (gb905_peri_bcd_timestamp_t *)(buf + sizeof(gb905_peri_header_t));
	gb905_peri_build_timestamp(tsm_timestamp);
	
	tsm_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + sizeof(gb905_peri_bcd_timestamp_t));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_bcd_timestamp_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(tsm_tail,xor);
	
	fleety_uart_send(TSM_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;
}


/** 
* @brief 	GB905   ��TSM   Э�����
* @param buf	 	��Ž����Ժ����Ϣ����
* @param len		��Ž�����ǰ����Ϣ����
*
* @return 		�����������ݳ���(   ���Դӻ��������Ƴ�)
*/
int gb905_tsm_protocol_ayalyze(unsigned char * buf,int len)
{
	unsigned char * msg_buf;
	int msg_len;
	int offset;
	
	double_buff_mgr_t tsm_buf;
	
	DbgFuncEntry();

	msg_buf = buf;
	msg_len = len;
	
	do{
		memset(&tsm_buf,0,sizeof(double_buff_mgr_t));
		
		tsm_buf.raw.buf = msg_buf;
		tsm_buf.raw.len = msg_len;

		offset = gb905_peri_get_a_full_msg(&tsm_buf);
	
#if 0
		{
			int i;
				
			DbgPrintf("offset = %d\r\n",offset);
			DbgPrintf("tsm_lens = %d\r\n",tsm_buf.raw.len);
			
			for(i=0;i<tsm_buf.raw.len;i++)
			{
				DbgPrintf("tsm_buf[%d] = 0x%2x\r\n",i,tsm_buf.raw.buf[i]);
			}
		}
#endif
		if(offset && tsm_buf.raw.len)
		{
			// 3:magic_end + xor				5: magic_start+magic_end+xor
			if(tsm_buf.raw.buf[tsm_buf.raw.len - 3] != xor8_computer(tsm_buf.raw.buf + 2,tsm_buf.raw.len - 5))
			{
				DbgError("xor verify failed!\r\n");
				return -1;
			}
			
			gb905_tsm_parse_a_full_msg(&tsm_buf.raw);
		}

		msg_buf += offset;
		msg_len -= offset;
	}while(offset && tsm_buf.raw.len && msg_len > 0);
	
	return (len - msg_len);

	DbgFuncExit();
}


void gb905_tsm_heart_beat_treat(void)
{
    taxi_status_t status;
    taxi_get_status(&status);
	
    if(tsm_heart_beat_count > TSM_HEART_BEAT_THREHOLD)
	{
        //������ڱ���״̬�������ñ���״̬
        if(!status.alarm.flag.tsm_fault)
        {
            set_tsm_alarm_status();
        }
        
        DbgError("GB905 TSM Heart Beat Timeout!\r\n");
	}
    else
    {
        //����ڱ���״̬�����������״̬
        if(status.alarm.flag.tsm_fault)
        {
            clr_tsm_alarm_status();
        }
    }
}

/** 
* @brief 	��TSM   ģ�鷢����������
*
*/
void gb905_tsm_update(void)
{
	unsigned char *buf;
	unsigned char xor;

	gb905_tsm_update_t tsm_update;

	DbgFuncEntry();
	
	buf = (unsigned char *)&tsm_update;
	gb905_tsm_build_header(&tsm_update.header,TSM_COMMAND_UPGRADE,sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));

	gb905_tsm_build_update_body(&tsm_update.body);
	
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(&tsm_update.tail,xor);

	fleety_uart_send(TSM_UART,buf,sizeof(gb905_tsm_update_t));

	DbgFuncExit();
}


