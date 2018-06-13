/****************************************Copyright (c)**************************************************                         
** File name:			gb905_transparent.c
** Descriptions:		��Χ���ݵ�������͸��(0x8B10,	0x0B10)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-02
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

#include	"module/gb905/gb905_common.h"

#include	"module/gb905_peri/tsm/gb905_tsm.h"
#include	"module/gb905_peri/meter/gb905_meter.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"

#include	"module/gb905/transparent/gb905_transparent.h"

#include	"middleware/socket/fleety_socket.h"

#include	"middleware/uart/fleety_uart.h"

#define		DEBUG_Y
#include	"libs/debug.h"


typedef union 
{
	unsigned short whole;
	
	struct
	{
		// low --> high
		unsigned char tar:1;		        //bit0: 0,������ѹ��;1,����gzѹ��
		unsigned char reserved1:1;			//bit1:Ԥ��
		unsigned char reserved2:1;          //bit2:Ԥ��
		unsigned char encrypt:1;            //bit3:0,����;1,����
		unsigned char :0;                   //bit4~bit15:Ԥ��
    }flag;
}data_type_t;

typedef struct{
	unsigned char device_id;				// �豸����
	data_type_t data_type;					// ��������
	unsigned char data;						// ͸���������ֽ�
}__packed gb905_transparent_down_t;

typedef struct{
	unsigned char device_id;				// �豸����
	unsigned char factory_id;				// ���̱�ʶ
	unsigned short cmd_id;					// ������
}__packed gb905_transparent_up_t;


/*
* @brief 	����͸�����ݵĴ���
* @param buf	 ����͸����Ϣ������ݵ�ַ
* @param len	 ����͸����Ϣ������ݳ���
* 
*/
void gb905_transparent_download_treat(unsigned char *buf,int len)
{
	gb905_transparent_down_t *down_msg;
	unsigned char *data_prt;
	unsigned short data_len;
	int i;
	
	DbgFuncEntry();
	
	down_msg = (gb905_transparent_down_t *)buf;

	data_prt = &down_msg->data;
	data_len = len-(sizeof(gb905_transparent_down_t)-1);

	
	DbgPrintf("device id is 0x%02x\r\n",down_msg->device_id);
	DbgPrintf("data type is 0x%04x\r\n",down_msg->data_type.whole);
	DbgPrintf("gb905 Down tansparent data is::\r\n");
	for(i=0;i<data_len;i++)
	{
		DbgPrintf("0x%02x ",data_prt[i]);
	}
	DbgPrintf("\r\n");

	//�ж��Ƿ�Ϊ����
	if(down_msg->data_type.flag.encrypt)
	{
		gb905_tsm_decryption(data_prt,data_len);//����
	}
	else
	{
		switch(down_msg->device_id)
		{
			case GB905_DEVICE_MCU:
				break;

			case GB905_DEVICE_GPRS:
				break;
				
			case GB905_DEVICE_METER:
				gb905_meter_transparent(data_prt,data_len);
				break;
				
			case GB905_DEVICE_TSM:
				break;
				
			case GB905_DEVICE_LED:
				break;
				
			case GB905_DEVICE_TOPLIGHT:
				gb905_toplight_transparent(data_prt,data_len);
				break;	
				
			case GB905_DEVICE_EVAL_FRONT:
				break;
				
			case GB905_DEVICE_CAMERA:
				break;
				
			case GB905_DEVICE_GPS:
				break;
				
			case GB905_DEVICE_LCD:
				break;
				
			case GB905_DEVICE_ISU:
				break;
				
			case GB905_DEVICE_EVAL_BACK:
				break;
				
			default:
				DbgError(" can not support this device id(0x%02x)!\r\n",down_msg->device_id);
				break;
		}
	}

	DbgFuncExit();
		
}


/*
* @brief 	��������͸�����ݵ�������
* @param dev_id		�豸����
* @param factory_id	����ID
* @param cmd_id		͸����������
* @param buf			͸������ָ��
* @param len			͸�����ݳ���
*
*/
void gb905_transparent_upload_treat(unsigned char dev_id, unsigned char factory_id, unsigned short cmd_id, unsigned char* buf, unsigned short len)
{
	gb905_transparent_up_t *up_msg;
	gb905_header_t *header;
	unsigned char *data_buf;
	unsigned char *send_buf;
	unsigned short send_len;

	DbgFuncEntry();
	
	send_len = 3+sizeof(gb905_header_t)+sizeof(gb905_transparent_up_t)+len;//3:sizeof 7E+CRC+7E

	send_buf = (unsigned char*)malloc(send_len);
	if(!send_buf)
	{
		DbgError("up transparent memory malloc failed!\r\n");
		return;
	}
	header = (gb905_header_t *)(send_buf+1);// 1:sizeof 7E
	gb905_build_header(header,MESSAGE_PERIPHERY_UP,sizeof(gb905_transparent_up_t)+len);

	up_msg = (gb905_transparent_up_t *)(send_buf+1+sizeof(gb905_header_t));
	up_msg->device_id = dev_id;
	up_msg->factory_id = factory_id;
	up_msg->cmd_id = cmd_id;

	data_buf = send_buf+1+sizeof(gb905_header_t)+sizeof(gb905_transparent_up_t);
	memcpy((void*)data_buf, (void*)buf, len);

	#if 1
	
	int i;
	DbgPrintf("gb905 Up transparent data is ::\r\n");
	for(i=0;i<send_len;i++)
	{
		DbgPrintf("0x%02x ",send_buf[i]);
	}
	DbgPrintf("\r\n");
	#endif
	
	gb905_send_data(MAIN_SOCKET,send_buf,send_len);

	free(send_buf);
	
	DbgFuncExit();
		
}
