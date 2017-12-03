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

#include	"misc/misc.h"


#include	"middleware/info/setting.h"
#include	"middleware/info/status.h"


#define		DEBUG_Y
#include	"libs/debug.h"

static trace_params_t	trace_params;



//----- 
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
static setting_params_t  setting_params;

setting_params_t * get_setting_params(void)
{ 
	return &setting_params;
}

void set_setting_params(void)
{
	// Ð´Èëxml  ÎÄ¼þ
	debug_setting_params();
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

