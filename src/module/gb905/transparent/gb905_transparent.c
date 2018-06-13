/****************************************Copyright (c)**************************************************                         
** File name:			gb905_transparent.c
** Descriptions:		外围数据的上下行透传(0x8B10,	0x0B10)
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
		unsigned char tar:1;		        //bit0: 0,数据无压缩;1,数据gz压缩
		unsigned char reserved1:1;			//bit1:预留
		unsigned char reserved2:1;          //bit2:预留
		unsigned char encrypt:1;            //bit3:0,明文;1,密文
		unsigned char :0;                   //bit4~bit15:预留
    }flag;
}data_type_t;

typedef struct{
	unsigned char device_id;				// 设备类型
	data_type_t data_type;					// 数据类型
	unsigned char data;						// 透传数据首字节
}__packed gb905_transparent_down_t;

typedef struct{
	unsigned char device_id;				// 设备类型
	unsigned char factory_id;				// 厂商标识
	unsigned short cmd_id;					// 命令字
}__packed gb905_transparent_up_t;


/*
* @brief 	下行透传数据的处理
* @param buf	 下行透传消息体的数据地址
* @param len	 下行透传消息体的数据长度
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

	//判断是否为密文
	if(down_msg->data_type.flag.encrypt)
	{
		gb905_tsm_decryption(data_prt,data_len);//解密
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
* @brief 	发送上行透传数据到服务器
* @param dev_id		设备类型
* @param factory_id	厂商ID
* @param cmd_id		透传的命令字
* @param buf			透传数据指针
* @param len			透传数据长度
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
