/****************************************Copyright (c)**************************************************                         
** File name:			gb905_login.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-15
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _GB905_LOGIN_H
#define _GB905_LOGIN_H

#ifdef __cplusplus
extern "C" {
#endif

// 上班登陆消息体的数据格式
typedef struct{
	report_body_t report;							// 位置信息

	unsigned char company_license_number[16];		// 公司经营许可证号(ASCII码,  不足16  字节，右补0)
	unsigned char driver_license_number[19];		// 驾驶员从业资格证号(ASCII码,  不足19  字节，右补0)
	unsigned char plate_number[6];					// 车牌号(ASCII 码)
	
	gb905_meter_timestamp_t login_time;				// 登陆时间
}__packed gb905_login_body_t;

//  上班登陆的整个消息数据格式
typedef  struct
{
	unsigned char start_magic_id;
	gb905_header_t header;
	gb905_login_body_t login_body;
	unsigned char xor;
	unsigned char end_magic_id;
} __packed gb905_login_t;

// 下班签退消息体的数据格式
typedef struct{
	report_body_t report;							// 位置信息
	gb905_meter_close_success_body_t meter_close_info;	// 计价器关机信息
	unsigned char logout_mode;						// 签退方式(0x00,正常签退;   0x01, 强制签退)
}__packed gb905_logout_body_t;

//  下班签退的整个消息数据格式
typedef  struct
{
	unsigned char start_magic_id;
	gb905_header_t header;
	gb905_logout_body_t logout_body;
	unsigned char xor;
	unsigned char end_magic_id;
} __packed gb905_logout_t;


void gb905_login(void);
void gb905_logout(void);


#ifdef __cplusplus
}
#endif



#endif




