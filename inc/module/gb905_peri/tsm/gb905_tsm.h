/****************************************Copyright (c)**************************************************                         
** File name:			gb905_tsm.h
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
#ifndef _GB905_TSM_H
#define _GB905_TSM_H

#ifdef __cplusplus
extern "C" {
#endif

#include	"module/gb905_peri/gb905_peri_common.h"

enum{
	TSM_BPS2400 = 0,
	TSM_BPS4800,
	TSM_BPS9600,		// default
	TSM_BPS14400,
	TSM_BPS19200,
	TSM_BPS38400,
	TSM_BPS57600,
	TSM_BPS115200,
	TSM_BPS128000,
};

typedef struct{
	unsigned char device_num[5];			// 设备序列号
	unsigned char hard_version;				// 硬件版本号
	unsigned char soft_version[2];			// 软件版本号
	unsigned char administrative_area[3];	// 所属行政区域
	unsigned char device_status;			// 设备状态(0x00,正常; 0x01,系统存储异常)
	unsigned char esam_type;				// ESAM卡型号
	//unsigned char pos_num[5];					// SAM 卡状态
	//unsigned char psam_card_num[5];
	//unsigned int trading_num; 
	//unsigned char rfu[5];
}__packed gb905_tsm_status_t;


typedef struct{
	unsigned char device_status;			// 
	unsigned char SAM_status;
	unsigned char loginout_status;
	unsigned char driver_code[18];
	unsigned char car_num[6];
	unsigned char terminal_func;
	unsigned char one_card_POS_status;
	unsigned char unionpay_POS_status;
	unsigned char extend;
}__packed gb905_tsm_heart_beat_t;


typedef struct{
	gb905_peri_bcd_timestamp_t now_time;		// MCU  当前时间
	unsigned char internet_content_status;		// 支付网络连接状态
	unsigned char working_status;				// 工作状态
	unsigned char car_num[6];					// 车辆车牌号码
	unsigned char certification_num[18];		// 当班司机从业资格证号(身份证号码)
}__packed gb905_tsm_heart_beat_ack_t;

typedef struct{
	unsigned char vendor_id;					// 厂商标识
	unsigned char hard_version;					// 硬件版本号
	unsigned char soft_major_version;			// 软件主版本号
	unsigned char soft_minor_version;			// 软件副版本号
}__packed gb905_tsm_upgrade_t;


typedef struct{
	unsigned char ic_card_type;					// 卡的类型(=0x01 道路运输证; =0x02,从业资格证)
	unsigned char country_version;				// 全国版本
	unsigned char place_version;				// 地方版本
	unsigned char driver_name[12];				// 司机姓名			(ASCII  编码)
	unsigned char driver_address[48];			// 司机住址			(ASCII  编码)
	unsigned char quailficantion_number[18];	// 从业资格证号	(ASCII  编码)
	unsigned char quailficantion_type[48];		// 从业资格类别	(ASCII  编码)
	unsigned char license_organ[48];			// 发证机关			(ASCII  编码)
	unsigned char useful_life[4];				// 有效截止日期	(BCD 编码YYYYMMDD)
	unsigned char card_number[6];				// 卡号				(HEX 编码)
	unsigned char return_result[2];				// 操作结果	(0x90 0x00 = 成功，0xFF 0x0E:不支持的卡座)
}__packed gb905_tsm_driver_info_t;

typedef struct{
	unsigned char vendor_id;					// 厂商标识
	unsigned short cmd;							// 应答的命令
	unsigned short result;						// 应答的状态(成功,  失败，不支持等)
}__packed gb905_tsm_common_ack_t;

bool gb905_tsm_query_state(void);
bool gb905_tsm_reset(void);
bool gb905_tsm_upgrade(gb905_tsm_upgrade_t* tsm_date);
bool gb905_tsm_get_driver_info(void);
bool gb905_tsm_decryption(unsigned char* data,unsigned short data_len);


int gb905_tsm_protocol_ayalyze(unsigned char * buf,int len);

#ifdef __cplusplus
}
#endif
	
#endif

