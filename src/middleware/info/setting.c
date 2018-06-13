/****************************************Copyright (c)**************************************************                         
** File name:			setting.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-12
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
#include	"misc/misc.h"
#include	"misc/check.h"


#include	"libs/mxml.h"

#include	"middleware/db/file/db_file_path.h"
#include	"middleware/db/file/db_file_xml.h"


#include	"middleware/info/setting.h"
#include	"middleware/info/status.h"
#include	"middleware/socket/fleety_socket.h"
#include	"middleware/uart/fleety_uart.h"


#define		DEBUG_Y
#include	"libs/debug.h"


#define		SETTING_FILE		"setting.xml"
#define		SETTING_FILE_BAK	"setting.bak"

#define		SOCKET_FILE			"socket_setting.xml"
#define		UART_FILE			"uart_setting.xml"

//----- 
static trace_params_t	trace_params;

void get_trace_setting(trace_params_t * setting)
{
	DbgFuncEntry();
	
	memcpy(setting,&trace_params,sizeof(trace_body_t));

	DbgFuncExit();
}

void set_trace_setting(trace_params_t * setting)
{
	DbgFuncEntry();
		
	memcpy(&trace_params,setting,sizeof(trace_params_t));
	
	if(trace_params.policy == TRACE_STOP_POLICY)
		stop_trace_mode();
	else
		start_trace_mode();
	
	DbgFuncExit();
}

void debug_trace_setting(void)
{
	DbgFuncEntry();

	DbgPrintf("trace policy = 0x%x\r\n",trace_params.policy);
	DbgPrintf("trace interval = 0x%x\r\n",trace_params.interval);
	DbgPrintf("trace threhold = 0x%x\r\n",trace_params.threhold);
	
	DbgFuncExit();
}


//-----
static uart_params_t  uart_params;

static unsigned char * uart_params_point[] = 
{
	(unsigned char *)uart_params.meter_uart_device,
	(unsigned char *)&uart_params.meter_uart_bps,

	(unsigned char *)uart_params.toplight_uart_device,
	(unsigned char *)&uart_params.toplight_uart_bps,

	(unsigned char *)uart_params.tsm_uart_device,
	(unsigned char *)&uart_params.tsm_uart_bps,

	(unsigned char *)uart_params.gps_uart_device,
	(unsigned char *)&uart_params.gps_uart_bps,

	(unsigned char *)uart_params.mcu_uart_device,
	(unsigned char *)&uart_params.mcu_uart_bps,

	(unsigned char *)uart_params.gprs_uart_device,
	(unsigned char *)&uart_params.gprs_uart_bps,

	(unsigned char *)uart_params.inspect_uart_device,
	(unsigned char *)&uart_params.inspect_uart_bps,	
};

static char * uart_params_type[] = 
{
	"STRING",
	"DWORD",

	"STRING",
	"DWORD",

	"STRING",
	"DWORD",

	"STRING",
	"DWORD",

	"STRING",
	"DWORD",

	"STRING",
	"DWORD",

	"STRING",
	"DWORD",
};

/** 
* @brief	从xml    文件中获取uart  设备和波特率 参数
*
*/
void init_uart_params(void)
{
	bool ret;
	char path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	build_db_path(path,UART_FILE);
	ret = xml2data(path,uart_params_point);
	if(!ret)
	{
		memset(&uart_params,0x00,sizeof(uart_params_t));				

		strcpy((char *)uart_params.meter_uart_device,METER_UART_DEV_NAME);
		uart_params.meter_uart_bps = METER_UART_DEV_BPS;

		strcpy((char *)uart_params.toplight_uart_device,TOPLIGHT_UART_DEV_NAME);
		uart_params.toplight_uart_bps = TOPLIGHT_UART_DEV_BPS;
		
		strcpy((char *)uart_params.tsm_uart_device,TSM_UART_DEV_NAME);
		uart_params.tsm_uart_bps = TSM_UART_DEV_BPS;

		strcpy((char *)uart_params.gps_uart_device,GPS_UART_DEV_NAME);
		uart_params.gps_uart_bps = GPS_UART_DEV_BPS;

		strcpy((char *)uart_params.mcu_uart_device,MCU_UART_DEV_NAME);
		uart_params.mcu_uart_bps = MCU_UART_DEV_BPS;
		
		strcpy((char *)uart_params.gprs_uart_device,GPRS_UART_DEV_NAME);
		uart_params.gprs_uart_bps = GPRS_UART_DEV_BPS;

		strcpy((char *)uart_params.inspect_uart_device,INSPECT_UART_DEV_NAME);
		uart_params.inspect_uart_bps = INSPECT_UART_DEV_BPS;

		DbgPrintf("list size = %d\r\n",ARRAY_SIZE(uart_params_point));
	
		init_xml(UART_FILE,uart_params_point,uart_params_type,ARRAY_SIZE(uart_params_point));
	}

	fleety_modify_uart_device(METER_UART,uart_params.meter_uart_device,uart_params.meter_uart_bps);
	fleety_modify_uart_device(TOPLIGHT_UART,uart_params.toplight_uart_device,uart_params.toplight_uart_bps);
	fleety_modify_uart_device(TSM_UART,uart_params.tsm_uart_device,uart_params.tsm_uart_bps);
	fleety_modify_uart_device(GPS_UART,uart_params.gps_uart_device,uart_params.gps_uart_bps);
	fleety_modify_uart_device(MCU_UART,uart_params.mcu_uart_device,uart_params.mcu_uart_bps);
	fleety_modify_uart_device(GPRS_UART,uart_params.gprs_uart_device,uart_params.gprs_uart_bps);
	fleety_modify_uart_device(INSPECT_UART,uart_params.inspect_uart_device,uart_params.inspect_uart_bps);

	debug_uart_params();

	DbgFuncExit();
}


uart_params_t * get_uart_params(void)
{
	return &uart_params;
}

/** 
* @brief	 将uart   参数写入xml  文件
*
*/
void set_uart_params(void)
{
	char path[MAX_PATH_CHARS_SIZE];

	build_db_path(path,SOCKET_FILE);
	data2xml(path,uart_params_point);
}

void debug_uart_params(void)
{
	DbgFuncEntry();

	DbgPrintf("meter_uart_device = %s\r\n",uart_params.meter_uart_device);
	DbgPrintf("meter_uart_bsp = %d\r\n",uart_params.meter_uart_bps);

	DbgPrintf("toplight_uart_device = %s\r\n",uart_params.toplight_uart_device);
	DbgPrintf("toplight_uart_bps = %d\r\n",uart_params.toplight_uart_bps);

	DbgPrintf("tsm_uart_device = %s\r\n",uart_params.tsm_uart_device);
	DbgPrintf("tsm_uart_bps = %d\r\n",uart_params.tsm_uart_bps);

	DbgPrintf("gps_uart_device = %s\r\n",uart_params.gps_uart_device);
	DbgPrintf("gps_uart_bps = %d\r\n",uart_params.gps_uart_bps);
	
	
	DbgPrintf("mcu_uart_device = %s\r\n",uart_params.mcu_uart_device);
	DbgPrintf("mcu_uart_bps = %d\r\n",uart_params.mcu_uart_bps);
	
	DbgPrintf("gprs_uart_device = %s\r\n",uart_params.gprs_uart_device);
	DbgPrintf("gprs_uart_bps = %d\r\n",uart_params.gprs_uart_bps);
	
	DbgPrintf("inspect_uart_device = %s\r\n",uart_params.inspect_uart_device);
	DbgPrintf("inspect_uart_bps = %d\r\n",uart_params.inspect_uart_bps);
	
	DbgFuncExit();
}



//-----
static socket_params_t  socket_params;

static unsigned char * socket_params_point[] = 
{
	(unsigned char *)socket_params.main_server_ip,
	(unsigned char *)&socket_params.main_server_port,

	(unsigned char *)socket_params.vice_server_ip,
	(unsigned char *)&socket_params.vice_server_port,

	(unsigned char *)socket_params.ui_server_ip,
	(unsigned char *)&socket_params.ui_server_port,

	(unsigned char *)socket_params.auth_server_ip,
	(unsigned char *)&socket_params.auth_server_port,	
};

static char * socket_params_type[] = 
{
	"STRING",
	"DWORD",

	"STRING",
	"DWORD",

	"STRING",
	"DWORD",

	"STRING",
	"DWORD",
};

/** 
* @brief	从xml    文件中获取socket  ip/port 参数
*
*/
void init_socket_params(void)
{
	bool ret;
	char path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	build_db_path(path,SOCKET_FILE);
	ret = xml2data(path,socket_params_point);
	if(!ret)
	{
		memset(&socket_params,0x00,sizeof(socket_params_t));				

		strcpy((char *)socket_params.main_server_ip,MAIN_SERVER_IP_ADDR);
		socket_params.main_server_port = MAIN_SERVER_PORT;

		strcpy((char *)socket_params.vice_server_ip,AUX_SERVER_IP_ADDR);
		socket_params.vice_server_port = AUX_SERVER_PORT;
		
		strcpy((char *)socket_params.ui_server_ip,UI_SERVER_IP_ADDR);
		socket_params.ui_server_port = UI_SERVER_PORT;

		strcpy((char *)socket_params.auth_server_ip,AUTH_SERVER_IP_ADDR);
		socket_params.auth_server_port = AUTH_SERVER_PORT;

		DbgPrintf("list size = %d\r\n",ARRAY_SIZE(socket_params_point));
	
		init_xml(SOCKET_FILE,socket_params_point,socket_params_type,ARRAY_SIZE(socket_params_point));
	}

	fleety_modify_server_ip(MAIN_SOCKET,socket_params.main_server_ip,socket_params.main_server_port);
	fleety_modify_server_ip(AUX_SOCKET,socket_params.vice_server_ip,socket_params.vice_server_port);
	fleety_modify_server_ip(UI_SOCKET,socket_params.ui_server_ip,socket_params.ui_server_port);
	
	debug_socket_params();

	DbgFuncExit();
}


socket_params_t * get_socket_params(void)
{
	return &socket_params;
}

/** 
* @brief	 将socket   参数写入xml  文件
*
*/
void set_socket_params(void)
{
	char path[MAX_PATH_CHARS_SIZE];

	build_db_path(path,SOCKET_FILE);
	data2xml(path,socket_params_point);
}

void debug_socket_params(void)
{
	DbgFuncEntry();

	DbgPrintf("main_server_ip = %s\r\n",socket_params.main_server_ip);
	DbgPrintf("main_server_port = %d\r\n",socket_params.main_server_port);

	DbgPrintf("vice_server_ip = %s\r\n",socket_params.vice_server_ip);
	DbgPrintf("vice_server_port = %d\r\n",socket_params.vice_server_port);

	DbgPrintf("ui_server_ip = %s\r\n",socket_params.ui_server_ip);
	DbgPrintf("ui_server_port = %d\r\n",socket_params.ui_server_port);
	
	DbgPrintf("auth_server_ip = %s\r\n",socket_params.auth_server_ip);
	DbgPrintf("auth_server_port = %d\r\n",socket_params.auth_server_port);
	
	DbgFuncExit();
}


//-----
static setting_params_t  setting_params;

static unsigned char * setting_params_point[] = 
{
	(unsigned char *)&setting_params.heartbeat_interval,
	(unsigned char *)&setting_params.tcp_msg_ack_timeout,
	(unsigned char *)&setting_params.tcp_msg_retry_count,	
	(unsigned char *)&setting_params.sms_msg_ack_timeout,
	(unsigned char *)&setting_params.sms_msg_retry_count,

	(unsigned char *)setting_params.main_server_apn,
	(unsigned char *)setting_params.main_server_username,
	(unsigned char *)setting_params.main_server_password,
	(unsigned char *)setting_params.main_server_ipaddr,
	(unsigned char *)setting_params.vice_server_apn,
	(unsigned char *)setting_params.vice_server_username,
	(unsigned char *)setting_params.vice_server_password,
	(unsigned char *)setting_params.vice_server_ipaddr,
	(unsigned char *)&setting_params.main_server_tcp_port,
	(unsigned char *)&setting_params.vice_server_tcp_port,
	(unsigned char *)setting_params.pay_main_server_apn,
	(unsigned char *)&setting_params.pay_main_server_tcp_port,
	(unsigned char *)setting_params.pay_vice_server_apn,
	(unsigned char *)&setting_params.pay_vice_server_tcp_port,

	(unsigned char *)&setting_params.location_report_policy,
	(unsigned char *)&setting_params.location_report_scheme,
	(unsigned char *)&setting_params.time_report_logout_interval,
	(unsigned char *)&setting_params.time_report_accoff_interval,
	(unsigned char *)&setting_params.time_report_accon_interval,
	(unsigned char *)&setting_params.time_report_vacant_internal,
	(unsigned char *)&setting_params.time_report_occupied_internal,
	(unsigned char *)&setting_params.time_report_idle_interval,
	(unsigned char *)&setting_params.time_report_alarm_interval,
	(unsigned char *)&setting_params.distance_report_logout_interval,
	(unsigned char *)&setting_params.distance_report_accoff_interval,
	(unsigned char *)&setting_params.distance_report_accon_interval,
	(unsigned char *)&setting_params.distance_report_vacant_interval,
	(unsigned char *)&setting_params.distance_report_occupied_interval,
	(unsigned char *)&setting_params.distance_report_idle_interval,
	(unsigned char *)&setting_params.distance_report_alarm_interval,
	(unsigned char *)&setting_params.report_angle_interval,
	
	(unsigned char *)setting_params.phone_number_monitor_center,
	(unsigned char *)setting_params.phone_number_reset,
	(unsigned char *)setting_params.phone_number_factory_recovery,
	(unsigned char *)setting_params.phone_number_sms_center,
	(unsigned char *)setting_params.phone_number_sms_rev,
	(unsigned char *)&setting_params.telephone_answer_policy,
	(unsigned char *)&setting_params.max_time_talk_once,
	(unsigned char *)&setting_params.max_time_talk_month,
	(unsigned char *)&setting_params.short_phone_number_len,
	(unsigned char *)setting_params.phone_number_monitor,
	(unsigned char *)setting_params.device_password,
	(unsigned char *)&setting_params.tts_volume,

	(unsigned char *)&setting_params.alarm_mask,
	(unsigned char *)&setting_params.alarm_sms_mask,
	(unsigned char *)&setting_params.alarm_video_mask,
	(unsigned char *)&setting_params.alarm_video_save_mask,
	(unsigned char *)&setting_params.max_speed,
	(unsigned char *)&setting_params.over_speed_duration,
	(unsigned char *)&setting_params.over_driver,
	(unsigned char *)&setting_params.min_rest_period,
	(unsigned char *)&setting_params.max_parking_time,
	(unsigned char *)&setting_params.driver_time_one_day,
	
	(unsigned char *)&setting_params.video_quality,
	(unsigned char *)&setting_params.video_brightness,
	(unsigned char *)&setting_params.video_contrast,
	(unsigned char *)&setting_params.video_saturation,
	(unsigned char *)&setting_params.video_chroma,
	
	(unsigned char *)&setting_params.vehicle_milometer,
	(unsigned char *)&setting_params.vehicle_province_id,
	(unsigned char *)&setting_params.vehicle_city_id,
	(unsigned char *)&setting_params.meter_operation_count_limit,
	(unsigned char *)setting_params.meter_operation_count_time,
	(unsigned char *)setting_params.company_license_number,
	(unsigned char *)setting_params.company_name,
	(unsigned char *)setting_params.plate_number,

	(unsigned char *)&setting_params.voice_record_mode,
	(unsigned char *)&setting_params.voice_record_period,
	(unsigned char *)&setting_params.lcd_heart_beat_time,
	(unsigned char *)&setting_params.led_heart_beat_time,
	(unsigned char *)&setting_params.time_idle_after_accoff,

	(unsigned char *)&setting_params.video_protocol_mode,
	(unsigned char *)setting_params.video_server_apn,
	(unsigned char *)setting_params.video_server_password,
	(unsigned char *)setting_params.video_server_password,
	(unsigned char *)setting_params.video_server_ipaddr,
	(unsigned char *)&setting_params.vidoe_server_port,
};

static char * setting_params_type[] = 
{
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",

	"STRING",
	"STRING",
	"STRING",
	"STRING",
	"STRING",
	"STRING",
	"STRING",
	"STRING",
	"DWORD",
	"DWORD",
	"STRING",
	"DWORD",
	"STRING",
	"DWORD",

	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",

	"STRING",
	"STRING",
	"STRING",
	"STRING",
	"STRING",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"STRING",
	"STRING",
	"DWORD",

	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",

	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",

	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"STRING",
	"STRING",
	"STRING",
	"STRING",

	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",
	"DWORD",

	"DWORD",
	"STRING",
	"STRING",
	"STRING",
	"STRING",
	"DWORD",
};


/** 
* @brief	从xml    文件中获取国标参数，如果没对应xml   文件，则第一次创建
*
*/
void init_setting_params(void)
{
	bool ret;
	char path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	build_db_path(path,SETTING_FILE);
	ret = xml2data(path,setting_params_point);
	if(!ret)
	{
		memset(&setting_params,0x00,sizeof(setting_params_t));

		setting_params.heartbeat_interval = 30;
		
		setting_params.tcp_msg_ack_timeout = 60;
		setting_params.tcp_msg_retry_count = 3;
				

		strcpy((char *)setting_params.main_server_ipaddr,MAIN_SERVER_IP_ADDR);
		setting_params.main_server_tcp_port = MAIN_SERVER_PORT;
		strcpy((char *)setting_params.vice_server_ipaddr,AUX_SERVER_IP_ADDR);
		setting_params.vice_server_tcp_port = AUX_SERVER_PORT;

		setting_params.location_report_policy = 0;
		setting_params.location_report_scheme = 0;
        setting_params.time_report_alarm_interval = 5;
		setting_params.time_report_accon_interval = 30;
		setting_params.time_report_accoff_interval = 20;
        setting_params.time_report_idle_interval = 20;
        setting_params.time_idle_after_accoff = 2*3600;
		setting_params.max_time_talk_once = 24*3600;
		setting_params.max_time_talk_month = 30*24*3600;

		DbgPrintf("list size = %d\r\n",ARRAY_SIZE(setting_params_point));
	
		init_xml(SETTING_FILE,setting_params_point,setting_params_type,ARRAY_SIZE(setting_params_point));
	}

	debug_setting_params();

	DbgFuncExit();
}


setting_params_t * get_setting_params(void)
{
	return &setting_params;
}

void set_setting_params(void)
{
	// 写入xml  文件
	char path[MAX_PATH_CHARS_SIZE];

	build_db_path(path,SETTING_FILE);
	data2xml(path,setting_params_point);
}

void export_setting_params(char *path)
{
	data2xml(path,setting_params_point);
}


void debug_setting_params(void)
{
	DbgFuncEntry();

	DbgPrintf("heartbeat_interval = %d\r\n",setting_params.heartbeat_interval);
	DbgPrintf("tcp_msg_ack_timeout = %d\r\n",setting_params.tcp_msg_ack_timeout);
	DbgPrintf("tcp_msg_retry_count = %d\r\n",setting_params.tcp_msg_retry_count);
	DbgPrintf("sms_msg_ack_timeout = %d\r\n",setting_params.sms_msg_ack_timeout);
	DbgPrintf("sms_msg_retry_count = %d\r\n",setting_params.sms_msg_retry_count);

	DbgPrintf("main_server_apn = %s\r\n",setting_params.main_server_apn);
	DbgPrintf("main_server_username = %s\r\n",setting_params.main_server_username);
	DbgPrintf("main_server_password = %s\r\n",setting_params.main_server_password);
	DbgPrintf("main_server_ipaddr = %s\r\n",setting_params.main_server_ipaddr);

	DbgPrintf("vice_server_apn = %s\r\n",setting_params.vice_server_apn);
	DbgPrintf("vice_server_username = %s\r\n",setting_params.vice_server_username);
	DbgPrintf("vice_server_password = %s\r\n",setting_params.vice_server_password);
	DbgPrintf("vice_server_ipaddr = %s\r\n",setting_params.vice_server_ipaddr);
	
	DbgPrintf("main_server_tcp_port = %d\r\n",setting_params.main_server_tcp_port);
	DbgPrintf("vice_server_tcp_port = %d\r\n",setting_params.vice_server_tcp_port);

	DbgPrintf("location_report_policy = %d\r\n",setting_params.location_report_policy);
	DbgPrintf("location_report_scheme = %d\r\n",setting_params.location_report_scheme);

	DbgPrintf("time_report_logout_interval = %d\r\n",setting_params.time_report_logout_interval);
	DbgPrintf("time_report_accoff_interval = %d\r\n",setting_params.time_report_accoff_interval);
	DbgPrintf("time_report_accon_interval = %d\r\n",setting_params.time_report_accon_interval);
	DbgPrintf("time_report_vacant_internal = %d\r\n",setting_params.time_report_vacant_internal);
	DbgPrintf("time_report_occupied_internal = %d\r\n",setting_params.time_report_occupied_internal);
	DbgPrintf("time_report_idle_interval = %d\r\n",setting_params.time_report_idle_interval);
	DbgPrintf("time_report_alarm_interval = %d\r\n",setting_params.time_report_alarm_interval);

	DbgPrintf("distance_report_logout_interval = %d\r\n",setting_params.distance_report_logout_interval);
	DbgPrintf("distance_report_accoff_interval = %d\r\n",setting_params.distance_report_accoff_interval);
	DbgPrintf("distance_report_accon_interval = %d\r\n",setting_params.distance_report_accon_interval);
	DbgPrintf("distance_report_vacant_interval = %d\r\n",setting_params.distance_report_vacant_interval);
	DbgPrintf("distance_report_occupied_interval = %d\r\n",setting_params.distance_report_occupied_interval);
	DbgPrintf("distance_report_idle_interval = %d\r\n",setting_params.distance_report_idle_interval);
	DbgPrintf("distance_report_alarm_interval = %d\r\n",setting_params.distance_report_alarm_interval);

	DbgPrintf("report_angle_interval = %d\r\n",setting_params.report_angle_interval);

	DbgPrintf("phone_number_monitor_center = %s\r\n",setting_params.phone_number_monitor_center);
	DbgPrintf("phone_number_reset = %s\r\n",setting_params.phone_number_reset);
	DbgPrintf("phone_number_factory_recovery = %s\r\n",setting_params.phone_number_factory_recovery);
	DbgPrintf("phone_number_sms_center = %s\r\n",setting_params.phone_number_sms_center);
	DbgPrintf("phone_number_sms_rev = %s\r\n",setting_params.phone_number_sms_rev);

	DbgPrintf("telephone_answer_policy = %d\r\n",setting_params.telephone_answer_policy);
	DbgPrintf("max_time_talk_once = %d\r\n",setting_params.max_time_talk_once);
	DbgPrintf("max_time_talk_month = %d\r\n",setting_params.max_time_talk_month);
	DbgPrintf("short_phone_number_len = %d\r\n",setting_params.short_phone_number_len);

	DbgPrintf("phone_number_monitor = %s\r\n",setting_params.phone_number_monitor);
	DbgPrintf("device_password = %s\r\n",setting_params.device_password);
	DbgPrintf("tts_volume = %d\r\n",setting_params.tts_volume);

	DbgPrintf("alarm_mask = 0x%x\r\n",setting_params.alarm_mask);
	DbgPrintf("alarm_sms_mask = 0x%x\r\n",setting_params.alarm_sms_mask);
	DbgPrintf("alarm_video_mask = 0x%x\r\n",setting_params.alarm_video_mask);
	DbgPrintf("alarm_video_save_mask = 0x%x\r\n",setting_params.alarm_video_save_mask);

	DbgPrintf("max_speed = %d\r\n",setting_params.max_speed);
	DbgPrintf("over_speed_duration = %d\r\n",setting_params.over_speed_duration);
	DbgPrintf("over_driver = %d\r\n",setting_params.over_driver);
	DbgPrintf("min_rest_period = %d\r\n",setting_params.min_rest_period);
	DbgPrintf("max_parking_time = %d\r\n",setting_params.max_parking_time);
	DbgPrintf("driver_time_one_day = %d\r\n",setting_params.driver_time_one_day);
		
	DbgFuncExit();
}


//-----
static meter_params_t  meter_params;


meter_params_t * get_meter_params(void)
{ 
	return &meter_params;
}

//void set_meter_params(void)
//{
//}

void debug_meter_params(void)
{
	unsigned int value;
	
	DbgFuncEntry();

	value = bcd2decimal(meter_params.price_return_per_day_bcd,2);
	DbgPrintf("back and forth price in day is %d \r\n",value);

	value = bcd2decimal(meter_params.price_return_per_night_bcd,2);
	DbgPrintf("back and forth price in night is %d \r\n",value);

	value = bcd2decimal(meter_params.price_oneway_per_day_bcd,2);
	DbgPrintf("one way price in day is %d \r\n",value);

	value = bcd2decimal(meter_params.price_oneway_per_night_bcd,2);
	DbgPrintf("one way price in night is %d \r\n",value);

	value = bcd2decimal(meter_params.price_subsidy_per_day_bcd,2);
	DbgPrintf("subsidy price in day is %d \r\n",value);

	value = bcd2decimal(meter_params.price_subsidy_per_night_bcd,2);
	DbgPrintf("subsidy price in night is %d \r\n",value);

	value = bcd2decimal(meter_params.price_start_per_day_bcd,2);
	DbgPrintf("starting price in day is %d \r\n",value);
	
	value = bcd2decimal(meter_params.price_start_per_night_bcd,2);
	DbgPrintf("starting price in night is %d \r\n",value);
	
	value = bcd2decimal(meter_params.price_wait_per_day_bcd,2);
	DbgPrintf("waiting price in day is %d \r\n",value);
	
	value = bcd2decimal(meter_params.price_wait_per_day_bcd,2);
	DbgPrintf("waiting price in night is %d \r\n",value);
	
	DbgFuncExit();
}

//-----
static license_params_t license_params;

void get_license_params(license_params_t * params)
{ 
	memcpy(params,&license_params,sizeof(license_params_t));
}

void set_license_params(license_params_t * params)
{
	memcpy(&license_params,params,sizeof(license_params_t));
}

void set_driver_license_number(unsigned char * driver_license_number,int len)
{
	int size;

	size = sizeof(license_params.driver_license_number);
	memset(license_params.driver_license_number,0x00,size);
	memcpy(license_params.driver_license_number,driver_license_number,MIN(len,size));
}

