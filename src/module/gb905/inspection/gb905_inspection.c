/****************************************Copyright (c)**************************************************                         
** File name:			gb905_inspection.c
** Descriptions:		巡检		(0x8B11,0x0B11)
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-13
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

#include	"misc/bcd.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905/inspection/gb905_inspection.h"

#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/meter/gb905_meter.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"
#include	"module/gb905_peri/tsm/gb905_tsm.h"

#include	"middleware/info/device.h"
#include	"middleware/info/status.h"
#include	"middleware/socket/fleety_socket.h"



#define		DEBUG_Y
#include	"libs/debug.h"


typedef struct{
	unsigned char device_id;
	unsigned char len;
	unsigned char* data;
}inspection_device_ack_t;


typedef struct{
    uuid_device_id_t uuid_device_id;	// 终端标识之设备编号
    
	unsigned char	hw_version;			// 硬件版本号
	unsigned char	sw_version[2];		// 软件版本号

	
	report_status_t status;				// 状态
    report_alarm_t	alarm;				// 报警标志

    char  login_cache_number;			// 未上传成功的缓存签到记录数
    char  logout_cache_number;   		// 未上传成功的缓存签退记录数
    char  operation_cache_number;		// 未上传的营运记录数
    char  card_cache_number;			// 未上传的一卡通交易数据
}__packed gb905_mcu_inspection_ack_t;


static inspection_device_ack_t inspection_device_ack[GB905_DEVICE_MAX];



static void gb905_mcu_query_state(void)
{
	device_info_t info;
	unsigned int mtd_id;
	gb905_mcu_inspection_ack_t mcu_inspection_ack;
	DbgFuncEntry();
	
	get_device_info(&info);
	mtd_id = info.mtd_id % 1000000;
	
	mcu_inspection_ack.uuid_device_id.vendor_id = 0x02;
	mcu_inspection_ack.uuid_device_id.product_id = 0x00;
	decimal2bcd(mtd_id,mcu_inspection_ack.uuid_device_id.mtd_id,3);
	
	mcu_inspection_ack.hw_version = 0x00;
	mcu_inspection_ack.sw_version[0] = 0x00;
	mcu_inspection_ack.sw_version[1] = 0x00;
	
	mcu_inspection_ack.status.whole = 0x00;
	mcu_inspection_ack.alarm.whole = 0x00;
	
	mcu_inspection_ack.login_cache_number = 0x00;
	mcu_inspection_ack.logout_cache_number = 0x00;
	mcu_inspection_ack.operation_cache_number = 0x00;
	mcu_inspection_ack.card_cache_number = 0x00;

	gb905_inspection_set_ack_info(GB905_DEVICE_MCU,(unsigned char*)&mcu_inspection_ack,sizeof(mcu_inspection_ack));

	DbgFuncExit();
}

static void gb905_build_inspection_ack(unsigned char* buf,int len)
{
	int i;
	int offset = 0;
	
	for(i = 0;i < GB905_DEVICE_MAX;i++)
	{
		if(inspection_device_ack[i].len)
		{
			buf[offset++] = inspection_device_ack[i].device_id;
			buf[offset++] = inspection_device_ack[i].len;
			memcpy(&buf[offset],inspection_device_ack[i].data,inspection_device_ack[i].len);
			offset += inspection_device_ack[i].len;
		}
	}
}

static void gb905_inspection_device_query(unsigned char device_id)
{
	DbgFuncEntry();
	
	switch(device_id)
	{
		case GB905_DEVICE_MCU:
			gb905_mcu_query_state();
			break;

		case GB905_DEVICE_METER:
			gb905_meter_query_state();
			break;

		case GB905_DEVICE_TSM:
            set_tsm_insp_status(true);
			gb905_tsm_query_state();
			break;
			
		case GB905_DEVICE_TOPLIGHT:
            set_led_insp_status(true);
			gb905_toplight_query_state();
			break;
			
		case GB905_DEVICE_GPRS:
		case GB905_DEVICE_LED:
		case GB905_DEVICE_EVAL_FRONT:
		case GB905_DEVICE_CAMERA:
		case GB905_DEVICE_GPS:
		case GB905_DEVICE_LCD:
		case GB905_DEVICE_ISU:
		case GB905_DEVICE_EVAL_BACK:
			break;
		
		default:
			DbgWarn("don't support this device!\r\n");
			break;
	}

	DbgFuncExit();
}


void gb905_inspection_device_treat(unsigned char* buf,unsigned short len)
{
	int i;
	unsigned char device_id;
	
	DbgFuncEntry();

	if(len == 0)
	{
		for(device_id=GB905_DEVICE_MCU;device_id<GB905_DEVICE_MAX;device_id++)
		{
			gb905_inspection_device_query(device_id);
		}
	}
	else
	{
		for(i=0;i<len;i++)
		{
			device_id = buf[i];
			gb905_inspection_device_query(device_id);
		}
	}

	sleep(3);
	gb905_inspection_device_ack_treat();

	gb905_inspection_clr_all_ack_info();
	
	DbgFuncExit();

	#if 0
	Inspection_data_init();

	if(len == 0)
	{
		DbgPrintf(" ------- Inspection all the device!!\r\n");
		for(device_type = GB905_MCU_TYPE;device_type < GB905_DEVICE_MAX;device_type++)
		{
			gb905_device_inspection_req(device_type);

		}
	}
	else
	{
		while(len--)
		{
			device_type = *buf++;
			DbgPrintf("------ device_type = 0x%02x \r\n",device_type);
			gb905_device_inspection_req(device_type);
			
		}
	}
	sleep(6);

	gb905_device_inspection_ack();

	Inspection_data_init();

	DbgFuncExit();
	#endif
}


void gb905_inspection_device_ack_treat(void)
{
	int i;
		
	unsigned char * ack_buf;
	int ack_len = 0;
	
	//gb905_header_t * header;
	//unsigned short sequence;
	
	DbgFuncEntry();

	// magic_start header inspection_list xor magic_end = sizeof(gb905_header_t) + 3 + inspection_list
	ack_len = sizeof(gb905_header_t) + 3;
	ack_len += 1;	// 1 : total len
	
	for(i=0;i<GB905_DEVICE_MAX;i++)
	{
		if(inspection_device_ack[i].len)
		{
			// 2 : id,len
			ack_len += inspection_device_ack[i].len + 2;
		}
	}

	ack_buf = (unsigned char *)malloc(ack_len);
	if(!ack_buf)
	{
		DbgError("inspection memory malloc failed!\r\n");
		return;
	}
	memset((void*)ack_buf, 0, ack_len);
	
	gb905_build_header((gb905_header_t *)(ack_buf + 1),MESSAGE_INSPECTION_ACK,ack_len - sizeof(gb905_header_t) - 3);  //+ 0x7E
	gb905_build_inspection_ack(ack_buf + 1 + sizeof(gb905_header_t),ack_len - sizeof(gb905_header_t) - 3);
	gb905_send_data(MAIN_SOCKET,ack_buf,ack_len);
		
	free(ack_buf);

	DbgFuncExit();
}



/** 
* @brief 	设置各巡检设备的响应数据(meter/toplight/tsm)
* @param buf 	各个外围设备的查询应答数据缓冲
* @param len 	各个外围设备的查询应答数据长度
*
*/
void gb905_inspection_set_ack_info(unsigned char device_id,unsigned char* buf,unsigned char len)
{
	inspection_device_ack[device_id].device_id = device_id;
	inspection_device_ack[device_id].len = len;
	inspection_device_ack[device_id].data = (unsigned char*)malloc(len);
	
	memset(inspection_device_ack[device_id].data,0,len);
	memcpy(inspection_device_ack[device_id].data,buf,len);
}


/** 
* @brief 	清除各巡检设备的响应数据(meter/toplight/tsm)
*
*/
void gb905_inspection_clr_ack_info(unsigned char device_id)
{
	inspection_device_ack[device_id].device_id = -1;
	inspection_device_ack[device_id].len = 0;
	if(inspection_device_ack[device_id].data != NULL)
		free(inspection_device_ack[device_id].data);
}

void gb905_inspection_clr_all_ack_info(void)
{
	int i;

	for(i=0;i<GB905_DEVICE_MAX;i++)
	{
		gb905_inspection_clr_ack_info(i);
	}
}

