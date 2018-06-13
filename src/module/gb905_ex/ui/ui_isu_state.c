/****************************************Copyright (c)**************************************************                         
** File name:			ui_isu_state.c
** Descriptions:		终端状态信息展示(例如:网关连接，报警，空重状态等)
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

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_isu_state.h"


#include	"middleware/info/setting.h"
#include	"middleware/info/status.h"
#include	"middleware/info/version.h"
#include	"middleware/info/device.h"
#include	"middleware/info/gps.h"


#define		DEBUG_Y
#include	"libs/debug.h"

//终端当前状态
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned gateway:1;			//网关连接状态(0:未连接;1:连接)				
		unsigned fix:1;				//定位状态(0:未定位;1:已定位)
		unsigned acc:1;				//ACC状态(0:ACC OFF ;1:ACC ON)
		unsigned login:1;			//登录状态(0:未登录;1:已登录)
		unsigned loading:1;			//空重车状态(0:空车;1:重车)		
		unsigned order:1;			//电召状态(0:处于电召状态;1:未处于电召状态)
		unsigned alarm:2;			//报警状态(00:未报警;01:预警;11:实警)
		unsigned :0;				//预留
    }flag;
}terminal_state_t;

//发送终端状态的数据结构
typedef struct{
	terminal_state_t state;			//终端当前状态
	unsigned char star_num;			//卫星颗数
}__packed ui_isu_state_t;

//发送终端状态的整体结构
typedef  struct
{
    unsigned char start_magic_id;	
    ui_protocol_header_t header;
    ui_isu_state_t isu_state;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed ui_protocol_isu_state_t;


//发送终端信息的数据结构
typedef struct{
	char sw_ver[MAX_SW_VERSION_SIZE];	//终端软件版本
	unsigned int mtd_id;				//终端设备id
	unsigned char main_ip[32];			//终端连接的主IP地址
	unsigned int main_port;				//终端连接的主端口
	unsigned char vice_ip[32];			//终端连接的副IP地址
	unsigned int vice_port;				//终端连接的副端口
}__packed ui_isu_info_t;


//发送终端信息的整体结构
typedef  struct
{
    unsigned char start_magic_id;	
    ui_protocol_header_t header;
    ui_isu_info_t isu_info;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed ui_protocol_isu_info_t;

static void ui_build_isu_state(ui_isu_state_t *isu_state)
{
	gps_info_t gps_info;

	DbgFuncEntry();

	gps_get_info(&gps_info);

	isu_state->state.whole = 0;
	isu_state->state.flag.gateway = (unsigned char)get_gateway_mode();
	isu_state->state.flag.fix = gps_info.fixed?0:1;
	isu_state->state.flag.acc = (unsigned char)get_acc_status();
	isu_state->state.flag.login = (unsigned char)get_login_mode();
	isu_state->state.flag.loading = (unsigned char)get_loading_status();
    isu_state->state.flag.order = (unsigned char)get_order_state();
    
	isu_state->star_num = gps_info.number;
	
	DbgFuncExit();
}

void ui_send_isu_state(void)
{
	ui_protocol_isu_state_t isu_state;
	
	DbgFuncEntry();
	
	ui_protocol_build_header(&isu_state.header,SYSTEM2UI_ISU_STATE_ACK,sizeof(ui_isu_state_t));

	ui_build_isu_state(&isu_state.isu_state);
	
	ui_protocol_send_data((unsigned char *)&isu_state,sizeof(ui_protocol_isu_state_t));

	DbgFuncExit();
	
	
}

static void ui_build_isu_info(ui_isu_info_t *isu_info)
{
	device_info_t device_info;
	setting_params_t * params;

	DbgFuncEntry();
	
	get_device_info(&device_info);
	params = get_setting_params();
	
	get_sw_version(isu_info->sw_ver);
	isu_info->mtd_id = device_info.mtd_id;
	memset(isu_info->main_ip,0x00,sizeof(isu_info->main_ip));
	
	DbgPrintf("main_server_ipaddr %s\r\n",params->main_server_ipaddr);
	memcpy(isu_info->main_ip,params->main_server_ipaddr,sizeof(isu_info->main_ip));
	
	DbgPrintf("main_ip %s\r\n",isu_info->main_ip);
	
	isu_info->main_port = params->main_server_tcp_port;

	memset(isu_info->vice_ip,0x00,sizeof(isu_info->vice_ip));
	
	DbgPrintf("vice_server_ipaddr %s\r\n",params->vice_server_ipaddr);
	memcpy(isu_info->vice_ip,params->vice_server_ipaddr,sizeof(isu_info->vice_ip));
	DbgPrintf("vice_ip %s\r\n",isu_info->vice_ip);

	isu_info->vice_port = params->vice_server_tcp_port;
	
	DbgFuncExit();
}

void ui_send_isu_info(void)
{
	ui_protocol_isu_info_t isu_info;
	
	DbgFuncEntry();
		
	ui_protocol_build_header(&isu_info.header,SYSTEM2UI_ISU_INFO_ACK,sizeof(ui_isu_info_t));

	ui_build_isu_info(&isu_info.isu_info);
	
	ui_protocol_send_data((unsigned char *)&isu_info,sizeof(ui_protocol_isu_info_t));
		
	
	DbgFuncExit();
	
}
