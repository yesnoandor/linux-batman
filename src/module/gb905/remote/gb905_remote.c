/****************************************Copyright (c)**************************************************                         
** File name:			gb905_remote.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-21
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
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905/remote/gb905_remote.h"

#include	"middleware/socket/fleety_socket.h"
#include	"middleware/info/status.h"

#define		DEBUG_Y
#include	"libs/debug.h"


#define		CHANNEL_CTRL_OIL		0
#define		CHANNEL_CTRL_CIRCUIT	1
#define		CHANNEL_CTRL_DOOR		2
#define		CHANNEL_CTRL_CAR		3

#define		RELAY_INDEX_OIL			0				
#define		RELAY_INDEX_CIRCUIT		1
#define		RELAY_INDEX_DOOR		2
#define		RELAY_INDEX_CAR			3


/**
* @brief 	平台设置车辆远程控制
* @param buf		车辆远程控制消息体的数据地址
* @param len		车辆远程控制消息体的数据长度
*
* @return  返回终端通用应答的状态
*/
unsigned char gb905_remote_control_treat(unsigned char *buf,int len)
{
	gb905_remote_ctrl_body_t * ctrl;

	DbgFuncEntry();

	ctrl = (gb905_remote_ctrl_body_t *)buf;

	DbgPrintf("channel = %d\r\n",ctrl->channel);
	DbgPrintf("cmd = %d\r\n",ctrl->cmd);

	switch(ctrl->channel)
	{
		case CHANNEL_CTRL_OIL:
			platform_hardware_relay_output(RELAY_INDEX_OIL,ctrl->cmd);
			set_oil_status(ctrl->cmd);
			break;
				
		case CHANNEL_CTRL_CIRCUIT:
			platform_hardware_relay_output(RELAY_INDEX_CIRCUIT,ctrl->cmd);
			set_circuit_status(ctrl->cmd);
			break;
				
		case CHANNEL_CTRL_DOOR:
			platform_hardware_relay_output(RELAY_INDEX_DOOR,ctrl->cmd);
			set_door_lock_status(ctrl->cmd);
			break;
			
		case CHANNEL_CTRL_CAR:
			platform_hardware_relay_output(RELAY_INDEX_CAR,ctrl->cmd);
			set_car_lock_status(ctrl->cmd);
			break;

		default:
			DbgWarn("don't support this channel(%d)!\r\n",ctrl->channel);
			break;
	}

	DbgFuncExit();

	return GB905_RESULT_OK;
}

void gb905_remote_control_ack(unsigned short seq_number)
{
	gb905_remote_ctrl_ack_t gb905_remote_contrl_ack;
		
	DbgFuncEntry();

	gb905_build_header(&gb905_remote_contrl_ack.header,MESSAGE_REMOTE_CTRL_ACK, sizeof(gb905_remote_ctrl_ack_body_t));
	gb905_debug_header(&gb905_remote_contrl_ack.header);
	
	gb905_remote_contrl_ack.body.serial_number = seq_number;

	gb905_build_report_body(&gb905_remote_contrl_ack.body.report);
	
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&gb905_remote_contrl_ack,sizeof(gb905_remote_ctrl_ack_t));
	
	DbgFuncExit();
}


