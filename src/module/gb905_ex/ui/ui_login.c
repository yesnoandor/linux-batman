/****************************************Copyright (c)**************************************************                         
** File name:			ui_login.c
** Descriptions:		登录信息展示
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-26
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

#include	"module/gb905_peri/tsm/gb905_tsm.h"

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_imei.h"
#include	"module/gb905_ex/ui/ui_login.h"

#include	"middleware/info/driver.h"

#define		DEBUG_Y
#include	"libs/debug.h"


typedef struct{
	unsigned char driver_name[12];				// 司机姓名			(ASCII  编码)
	unsigned char quailficantion_number[18];	// 从业资格证号	(ASCII  编码)
	unsigned char license_organ[48];			// 发证机关			(ASCII  编码)
	unsigned char useful_life[4];				// 有效截止日期	(BCD 编码YYYYMMDD)
	unsigned char card_number[6];				// 卡号				(HEX 编码)
}__packed ui_login_info_t;

void ui_login_info_send_treat(void)
{
	ui_protocol_header_t *header;
	gb905_tsm_driver_info_t driver_info;
	ui_login_info_t *login_info;
	unsigned char *send_buf;
	int send_len = 0;
	
	DbgFuncEntry();

	get_driver_info((unsigned char *)&driver_info);

	send_len = 3 + sizeof(ui_protocol_header_t) + sizeof(ui_login_info_t); // 7e + crc + 7e

	send_buf = (unsigned char *)malloc(send_len);

	header = (ui_protocol_header_t *)(send_buf + 1);	// 7e
	ui_protocol_build_header(header,SYSTEM2UI_LOGIN_INFO_ACK,sizeof(ui_login_info_t));

	login_info = (ui_login_info_t *)(send_buf + 1 + sizeof(ui_protocol_header_t));
	memcpy(login_info->driver_name,driver_info.driver_name,sizeof(login_info->driver_name));
	memcpy(login_info->quailficantion_number,driver_info.quailficantion_number,sizeof(login_info->quailficantion_number));
	memcpy(login_info->license_organ,driver_info.license_organ,sizeof(login_info->license_organ));
	memcpy(login_info->useful_life,driver_info.useful_life,sizeof(login_info->useful_life));
	memcpy(login_info->card_number,driver_info.card_number,sizeof(login_info->card_number));

	ui_protocol_send_data(send_buf,send_len);

	free(send_buf);	
	
	DbgFuncExit();
	
}
