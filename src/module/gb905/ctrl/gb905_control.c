/****************************************Copyright (c)**************************************************                         
** File name:			gb905_control.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-26
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

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/ctrl/gb905_control.h"

#include	"middleware/info/update.h"
#include	"middleware/socket/fleety_socket.h"


#define		DEBUG_Y
#include	"libs/debug.h"


#define		GB905_CONTROL_UPDATE		0x01
#define		GB905_CONTROL_POWER_OFF		0x02
#define		GB905_CONTROL_RESET			0x03
#define		GB905_CONTROL_RECOVERY		0x04
#define		GB905_CONTROL_OFF_SOCKET	0x05
#define		GB905_CONTROL_OFF_COMM		0x06






//-----
static void gb905_debug_update_param(gb905_update_param_t * param)
{
	DbgFuncEntry();

	DbgPrintf("device_type = %d\r\n",param->device_info.device_type);
	DbgPrintf("vendor_id = 0x%x\r\n",param->device_info.vendor_id);
	DbgPrintf("hw_version = %d\r\n",param->device_info.hw_version);
	DbgPrintf("sw_version[0] = 0x%x\r\n",param->device_info.sw_version[0]);
	DbgPrintf("sw_version[1] = 0x%x\r\n",param->device_info.sw_version[1]);

	DbgPrintf("update_server_apn = %s\r\n",param->update_server_apn);
	DbgPrintf("update_server_username = %s\r\n",param->update_server_username);
	DbgPrintf("update_server_password = %s\r\n",param->update_server_password);
	DbgPrintf("update_server_ipaddr = %s\r\n",param->update_server_ipaddr);

	DbgPrintf("update_server_tcp_port = %d\r\n",param->update_server_tcp_port);

	DbgFuncExit();
}


static void gb905_build_update_result(gb905_update_result_body_t * body,unsigned char result)
{
	gb905_update_device_info_t update_device_info;
	
	DbgFuncEntry();
	
	get_update_param((unsigned char *)&update_device_info);

	body->device_info.device_type = update_device_info.device_type;
	body->device_info.vendor_id = update_device_info.vendor_id;
	body->device_info.hw_version = update_device_info.hw_version;
	body->device_info.sw_version[0] = update_device_info.sw_version[0];
	body->device_info.sw_version[1] = update_device_info.sw_version[1];
	
	body->result = result;

	DbgFuncExit();
}


static void gb905_update_device(unsigned char *buf,int len)
{
	gb905_update_param_t update_param;

	gb905_update_device_info_t * device_info;
	char * str_buf;

	DbgFuncEntry();

	device_info = (gb905_update_device_info_t *)buf;

	update_param.device_info.device_type = device_info->device_type;
	update_param.device_info.hw_version = device_info->hw_version;
	update_param.device_info.vendor_id = device_info->vendor_id;
	update_param.device_info.sw_version[0] = device_info->sw_version[0];
	update_param.device_info.sw_version[1] = device_info->sw_version[1];

	str_buf = (char *)(buf + sizeof(gb905_update_device_info_t));
	memset(update_param.update_server_apn,0x00,sizeof(update_param.update_server_apn));
	strcpy((char *)update_param.update_server_apn,str_buf);

	str_buf = str_buf + strlen((const char *)update_param.update_server_apn) + 1; // 1 : skip '\0'
	memset(update_param.update_server_username,0x00,sizeof(update_param.update_server_username));
	strcpy((char *)update_param.update_server_username,str_buf);

	str_buf = str_buf + strlen(update_param.update_server_username) + 1;
	memset(update_param.update_server_password,0x00,sizeof(update_param.update_server_password));
	strcpy((char *)update_param.update_server_password,str_buf);

	str_buf = str_buf + strlen(update_param.update_server_password) + 1;
	memset(update_param.update_server_ipaddr,0x00,sizeof(update_param.update_server_ipaddr));
	strcpy((char *)update_param.update_server_ipaddr,str_buf);
	
	str_buf = str_buf + strlen(update_param.update_server_ipaddr) + 1;	// 1 : skip '\0'
	update_param.update_server_tcp_port = *(unsigned short*)(str_buf);
	update_param.update_server_tcp_port = EndianReverse16(update_param.update_server_tcp_port);
		
	gb905_debug_update_param(&update_param);

	set_update_param((unsigned char *)&update_param.device_info);
	
	DbgFuncExit();
}

/** 
* @brief 		GB905  协议的ISU  控制命令的处理
* @param 	buf		存放ISU  控制的命令参数地址
* @param 	len		存放ISU  控制的命令参数长度
* 
* @return			解析是否成功
*/
unsigned char gb905_control_treat(unsigned char *buf,int len)
{
	gb905_control_body_t * control_body;

	DbgFuncEntry();
	
	control_body = (gb905_control_body_t *)buf;

	switch(control_body->cmd)
	{
		case GB905_CONTROL_UPDATE:
			gb905_update_device((unsigned char *)&control_body->content,len - sizeof(control_body->cmd));
			break;

		case GB905_CONTROL_POWER_OFF:
			platform_system_power_off();
			break;

		case GB905_CONTROL_RESET:
			platform_system_reset();
			break;

		case GB905_CONTROL_RECOVERY:
			platform_system_recovery();
			break;

		case GB905_CONTROL_OFF_SOCKET:
			break;

		case GB905_CONTROL_OFF_COMM:
			break;
			
		default:
			DbgWarn("Don't support this control command(%d)\r\n",control_body->cmd);
			break;		
		
	}

	DbgFuncExit();

	return GB905_RESULT_OK;
}

/** 
* @brief 	发送ISU   升级结果报告
* @param result  	返回服务器的升级结果
*
*/
void gb905_send_update_result(unsigned char result)
{
	gb905_update_result_t update_result;

	DbgFuncEntry();
	
	gb905_build_header(&update_result.header,MESSAGE_UPDATE_RESULT_ACK,sizeof(gb905_update_result_t));

	gb905_debug_header(&update_result.header);

	gb905_build_update_result(&update_result.body,result);

	gb905_send_data(MAIN_SOCKET,(unsigned char *)&update_result,sizeof(gb905_update_result_t));

	DbgFuncExit();
}

