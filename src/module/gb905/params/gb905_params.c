/****************************************Copyright (c)**************************************************                         
** File name:			gb905_params.c
** Descriptions:		位置汇报(0x0200)  和位置信息查询(0x8201/0x0201) 的协议解析
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-14
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

#include	"middleware/info/setting.h"
#include	"middleware/socket/fleety_socket.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/params/gb905_params.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define		PARAMS_ID_INTERVAL_HEARTBEAT			0x0001
#define		PARAMS_ID_TCP_MSG_ACK_TIMEOUT			0x0002
#define		PARAMS_ID_TCP_MSG_RETRY_COUNT			0x0003
#define		PARAMS_ID_SMS_MSG_ACK_TIMEOUT			0x0004
#define		PARAMS_ID_SMS_MSG_RETRY_COUNT			0x0005

#define		PARAMS_ID_MAIN_SERVER_APN				0x0010
#define		PARAMS_ID_MAIN_SERVER_USERNAME			0x0011
#define		PARAMS_ID_MAIN_SERVER_PASSWORD			0x0012
#define		PARAMS_ID_MAIN_SERVER_IPADDR			0x0013
#define		PARAMS_ID_VICE_SERVER_APN				0x0014
#define		PARAMS_ID_VICE_SERVER_USERNAME			0x0015
#define		PARAMS_ID_VICE_SERVER_PASSWORD			0x0016
#define		PARAMS_ID_VICE_SERVER_IPADDR			0x0017
#define		PARAMS_ID_MAIN_SERVER_TCP_PORT			0x0018
#define		PARAMS_ID_VICE_SERVER_TCP_PORT			0x0019

#define		PARAMS_ID_LOCATION_REPORT_POLICY		0x0020
#define		PARAMS_ID_LOCATION_REPORT_SCHEME		0x0021

#define		PARAMS_ID_TIME_REPORT_LOGOUT_INTERVAL	0x0022
#define		PARAMS_ID_TIME_REPORT_ACCOFF_INTERVAL	0x0023
#define		PARAMS_ID_TIME_REPORT_ACCON_INTERVAL	0x0024
#define		PARAMS_ID_TIME_REPORT_VACANT_INTERVAL	0x0025
#define		PARAMS_ID_TIME_REPORT_OCCUPIED_INTERVAL	0x0026
#define		PARAMS_ID_TIME_REPORT_IDLE_INTERVAL		0x0027
#define		PARAMS_ID_TIME_REPORT_ALARM_INTERVAL	0x0028

#define		PARAMS_ID_DISTANCE_LOGOUT_INTERVAL		0x0029
#define		PARAMS_ID_DISTANCE_ACCOFF_INTERVAL		0x002A
#define		PARAMS_ID_DISTANCE_ACCON_INTERVAL		0x002B
#define		PARAMS_ID_DISTANCE_VACANT_INTERVAL		0x002C
#define		PARAMS_ID_DISTANCE_OCCUPIED_INTERVAL	0x002D
#define		PARAMS_ID_DISTANCE_IDLE_INTERVAL		0x002E
#define		PARAMS_ID_DISTANCE_ALARM_INTERVAL		0x002F
#define		PARAMS_ID_REPORT_ANGLE_INTERVAL			0x0030
			
#define		PARAMS_ID_PHONE_MONITOR_CENTER			0x0040
#define		PARAMS_ID_PHONE_RESET					0x0041
#define		PARAMS_ID_PHONE_FACTORY_RECOVERY		0x0042
#define		PARAMS_ID_PHONE_SMS_CENTER				0x0043
#define		PARAMS_ID_PHONE_SMS_REV					0x0044

#define		PARAMS_ID_PHONE_ANSWER_POLICY			0x0045
#define		PARAMS_ID_MAX_TIME_TALK_ONCE			0x0046
#define		PARAMS_ID_MAX_TIME_TALK_MONTH			0x0047
#define		PARAMS_ID_SHORT_PHONE_NUMBER_LEN		0x0048

#define		PARAMS_ID_PHONE_NUMBER_MONITOR			0x0049
#define		PARAMS_ID_DEVICE_PASSWORD				0x004A
#define		PARAMS_ID_TTS_VOLUMN					0x004B

#define		PARAMS_ID_ALARM_MASK					0x0050
#define		PARAMS_ID_ALARM_SMS_MASK				0x0051
#define		PARAMS_ID_ALARM_VIDEO_MASK				0x0052
#define		PARAMS_ID_ALARM_VIDEO_SAVE_MASK			0x0053

#define		PARAMS_ID_MAX_SPEED						0x0055
#define		PARAMS_ID_OVER_SPEED_DURATION			0x0056
#define		PARAMS_ID_OVER_DRIVE					0x0057
#define		PARAMS_ID_MIN_REST_PERIOD				0x0058
#define		PARAMS_ID_MAX_PARKING_TIME				0x0059
#define		PARAMS_ID_DRIVE_TIME_ONE_DAY			0x005A


#define		PARAMS_ID_VIDEO_QUALITY					0x0070
#define		PARAMS_ID_VIDEO_BRIGHTNESS				0x0071
#define		PARAMS_ID_VIDEO_CONTRAST				0x0072
#define		PARAMS_ID_VIDEO_SATUATION				0x0073
#define		PARAMS_ID_VIDEO_CHROMA					0x0074

#define		PARAMS_ID_VEHICLE_MILOMETER				0x0080
#define		PARAMS_ID_VEHICLE_PROVINCE_ID			0x0081
#define		PARAMS_ID_VEHICLE_CITY_ID				0x0082

#define		PARAMS_ID_METER_OPERATION_COUNT_LIMIT	0x0090
#define		PARAMS_ID_METER_OPERATION_COUNT_TIME	0x0091

#define		PARAMS_ID_TIME_IDLE_AFTER_ACCOFF		0x00AF
	
#define		PARAMS_ID_VIDEO_SERVER_APN				0x00B0
#define		PARAMS_ID_VIDEO_SERVER_USERNAME			0x00B1
#define		PARAMS_ID_VIDEO_SERVER_PASSWORD			0x00B2
#define		PARAMS_ID_VIDEO_SERVER_IPADDR			0x00B3
#define		PARAMS_ID_VIDEO_SERVER_TCP_PORT			0x00B4



/*
* @brief 	设置一条参数的处理
* @param msg_params	  存放一条参数的缓冲地址
*
* @return  返回终端通用应答的状态
*/
static void gb905_params_single_treat(unsigned short id,msg_params_t * msg_params)
{
	unsigned int temp;
	unsigned short temp1;
	setting_params_t * setting_params;
	
	DbgFuncEntry();

	setting_params = get_setting_params();
	
	switch(id)
	{
		case PARAMS_ID_INTERVAL_HEARTBEAT:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->heartbeat_interval)
			{
				setting_params->heartbeat_interval = temp;
				set_setting_params();
			}
			break;
			
		case PARAMS_ID_TCP_MSG_ACK_TIMEOUT:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->tcp_msg_ack_timeout)
			{
				setting_params->tcp_msg_ack_timeout = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_TCP_MSG_RETRY_COUNT:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->tcp_msg_retry_count)
			{
				setting_params->tcp_msg_retry_count = temp;
				set_setting_params();
			}
			break;
			
		case PARAMS_ID_SMS_MSG_ACK_TIMEOUT:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->sms_msg_ack_timeout)
			{
				setting_params->sms_msg_ack_timeout = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_SMS_MSG_RETRY_COUNT:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->sms_msg_retry_count)
			{
				setting_params->sms_msg_retry_count = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_MAIN_SERVER_APN:
			if(0 != strcmp((char *)setting_params->main_server_apn,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->main_server_apn,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_MAIN_SERVER_USERNAME:
			if(0 != strcmp((char *)setting_params->main_server_username,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->main_server_username,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_MAIN_SERVER_PASSWORD:
			if(0 != strcmp((char *)setting_params->main_server_password,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->main_server_password,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_MAIN_SERVER_IPADDR:
			if(0 != strcmp((char *)setting_params->main_server_ipaddr,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->main_server_ipaddr,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_VICE_SERVER_APN:
			if(0 != strcmp((char *)setting_params->vice_server_apn,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->vice_server_apn,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;	

		case PARAMS_ID_VICE_SERVER_USERNAME:
			if(0 != strcmp((char *)setting_params->vice_server_username,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->vice_server_username,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_VICE_SERVER_PASSWORD:
			if(0 != strcmp((char *)setting_params->vice_server_password,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->vice_server_password,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_VICE_SERVER_IPADDR:
			if(0 != strcmp((char *)setting_params->vice_server_ipaddr,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->vice_server_ipaddr,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_MAIN_SERVER_TCP_PORT:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->main_server_tcp_port)
			{
				setting_params->main_server_tcp_port = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_VICE_SERVER_TCP_PORT:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->vice_server_tcp_port)
			{
				setting_params->vice_server_tcp_port = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_LOCATION_REPORT_POLICY:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->location_report_policy)
			{
				setting_params->location_report_policy = temp;
				set_setting_params();
			}
			break;


		case PARAMS_ID_LOCATION_REPORT_SCHEME:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->location_report_scheme)
			{
				setting_params->location_report_scheme = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_TIME_REPORT_LOGOUT_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->time_report_logout_interval)
			{
				setting_params->time_report_logout_interval = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_TIME_REPORT_ACCOFF_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->time_report_accoff_interval)
			{
				setting_params->time_report_accoff_interval = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_TIME_REPORT_ACCON_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->time_report_accon_interval)
			{
				setting_params->time_report_accon_interval = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_TIME_REPORT_VACANT_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->time_report_vacant_internal)
			{
				setting_params->time_report_vacant_internal = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_TIME_REPORT_OCCUPIED_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->time_report_occupied_internal)
			{
				setting_params->time_report_occupied_internal = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_TIME_REPORT_IDLE_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->time_report_idle_interval)
			{
				setting_params->time_report_idle_interval = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_TIME_REPORT_ALARM_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->time_report_alarm_interval)
			{
				setting_params->time_report_alarm_interval = temp;
				set_setting_params();
			}
			break;
		
		case PARAMS_ID_DISTANCE_LOGOUT_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->distance_report_logout_interval)
			{
				setting_params->distance_report_logout_interval = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_DISTANCE_ACCOFF_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->distance_report_accoff_interval)
			{
				setting_params->distance_report_accoff_interval = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_DISTANCE_ACCON_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->distance_report_accon_interval)
			{
				setting_params->distance_report_accon_interval = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_DISTANCE_VACANT_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->distance_report_vacant_interval)
			{
				setting_params->distance_report_vacant_interval = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_DISTANCE_OCCUPIED_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->distance_report_occupied_interval)
			{
				setting_params->distance_report_occupied_interval = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_DISTANCE_IDLE_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->distance_report_idle_interval)
			{
				setting_params->distance_report_idle_interval = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_DISTANCE_ALARM_INTERVAL:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->distance_report_alarm_interval)
			{
				setting_params->distance_report_alarm_interval = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_PHONE_MONITOR_CENTER:
			if(0 != strcmp((char *)setting_params->phone_number_monitor_center,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->phone_number_monitor_center,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_PHONE_RESET:
			if(0 != strcmp((char *)setting_params->phone_number_reset,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->phone_number_reset,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_PHONE_FACTORY_RECOVERY:
			if(0 != strcmp((char *)setting_params->phone_number_factory_recovery,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->phone_number_factory_recovery,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_PHONE_SMS_CENTER:
			if(0 != strcmp((char *)setting_params->phone_number_sms_center,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->phone_number_sms_center,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_PHONE_SMS_REV:
			if(0 != strcmp((char *)setting_params->phone_number_sms_rev,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->phone_number_sms_rev,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_PHONE_ANSWER_POLICY:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->telephone_answer_policy)
			{
				setting_params->telephone_answer_policy = temp;
				set_setting_params();
			}
			break;


		case PARAMS_ID_MAX_TIME_TALK_ONCE:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->max_time_talk_once)
			{
				setting_params->max_time_talk_once = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_MAX_TIME_TALK_MONTH:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->max_time_talk_month)
			{
				setting_params->max_time_talk_month = temp;
				set_setting_params();
			}
			break;


		case PARAMS_ID_SHORT_PHONE_NUMBER_LEN:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->short_phone_number_len)
			{
				setting_params->short_phone_number_len = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_PHONE_NUMBER_MONITOR:
			if(0 != strcmp((char *)setting_params->phone_number_monitor,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->phone_number_monitor,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_DEVICE_PASSWORD:
			if(0 != strcmp((char *)setting_params->device_password,(char *)&msg_params->content));
			{
				strcpy((char *)setting_params->device_password,(const char *)&msg_params->content);
				set_setting_params();
			}
			break;

		case PARAMS_ID_TTS_VOLUMN:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->tts_volume)
			{
				setting_params->tts_volume = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_ALARM_MASK:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->alarm_mask)
			{
				setting_params->alarm_mask = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_ALARM_SMS_MASK:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->alarm_sms_mask)
			{
				setting_params->alarm_sms_mask = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_ALARM_VIDEO_MASK:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->alarm_video_mask)
			{
				setting_params->alarm_video_mask = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_ALARM_VIDEO_SAVE_MASK:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->alarm_video_save_mask)
			{
				setting_params->alarm_video_save_mask = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_MAX_SPEED:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->max_speed)
			{
				setting_params->max_speed = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_OVER_SPEED_DURATION:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->over_speed_duration)
			{
				setting_params->over_speed_duration = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_OVER_DRIVE:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->over_driver)
			{
				setting_params->over_driver = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_MIN_REST_PERIOD:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->min_rest_period)
			{
				setting_params->min_rest_period = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_MAX_PARKING_TIME:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->max_parking_time)
			{
				setting_params->max_parking_time = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_DRIVE_TIME_ONE_DAY:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->driver_time_one_day)
			{
				setting_params->driver_time_one_day = temp;
				set_setting_params();
			}
			break;


		case PARAMS_ID_VIDEO_QUALITY:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->video_quality)
			{
				setting_params->video_quality = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_VIDEO_BRIGHTNESS:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->video_brightness)
			{
				setting_params->video_brightness = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_VIDEO_CONTRAST:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->video_contrast)
			{
				setting_params->video_contrast = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_VIDEO_SATUATION:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->video_saturation)
			{
				setting_params->video_saturation = temp;
				set_setting_params();
			}
			break;

			
		case PARAMS_ID_VIDEO_CHROMA:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->video_chroma)
			{
				setting_params->video_chroma = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_VEHICLE_MILOMETER:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->vehicle_milometer)
			{
				setting_params->vehicle_milometer = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_VEHICLE_PROVINCE_ID:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->vehicle_province_id)
			{
				setting_params->vehicle_province_id = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_VEHICLE_CITY_ID:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->vehicle_city_id)
			{
				setting_params->vehicle_city_id = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_METER_OPERATION_COUNT_LIMIT:
			temp = EndianReverse32(msg_params->content);
			if(temp != setting_params->meter_operation_count_limit)
			{
				setting_params->meter_operation_count_limit = temp;
				set_setting_params();
			}
			break;

		case PARAMS_ID_METER_OPERATION_COUNT_TIME:
			if(0 != memcmp((void *)setting_params->meter_operation_count_time,(const void *)&msg_params->content,sizeof(setting_params->meter_operation_count_time)));
			{
				memcpy((void *)setting_params->meter_operation_count_time,(const void *)&msg_params->content,sizeof(setting_params->meter_operation_count_time));
				set_setting_params();
			}
			break;

		case PARAMS_ID_TIME_IDLE_AFTER_ACCOFF:
			memcpy(&temp1,&msg_params->content,sizeof(temp1));			
			temp1 = EndianReverse16(temp1);
			if(temp1 != setting_params->time_idle_after_accoff)
			{
				setting_params->time_idle_after_accoff = temp1;
				set_setting_params();
			}
			break;
			
		default:
			DbgWarn("msg id is not supported!\r\n");
			break;
	}

	DbgFuncExit();
}

static int get_pamams_id_len(unsigned short id)
{
	int len;

	setting_params_t * setting_params;
	
	DbgFuncEntry();

	setting_params = get_setting_params();
	
	switch(id)
	{
		case PARAMS_ID_MAIN_SERVER_APN:
			len = strlen((const char *)setting_params->main_server_apn) + 1;
			break;

		case PARAMS_ID_MAIN_SERVER_USERNAME:
			len = strlen((const char *)setting_params->main_server_username) + 1;
			break;

		case PARAMS_ID_MAIN_SERVER_PASSWORD:
			len = strlen((const char *)setting_params->main_server_password) + 1;
			break;

		case PARAMS_ID_MAIN_SERVER_IPADDR:
			len = strlen((const char *)setting_params->main_server_ipaddr) + 1;
			break;

		case PARAMS_ID_VICE_SERVER_APN:
			len = strlen((const char *)setting_params->vice_server_apn) + 1;
			break;	

		case PARAMS_ID_VICE_SERVER_USERNAME:
			len = strlen((const char *)setting_params->vice_server_username) + 1;
			break;

		case PARAMS_ID_VICE_SERVER_PASSWORD:
			len = strlen((const char *)setting_params->vice_server_password) + 1;
			break;

		case PARAMS_ID_VICE_SERVER_IPADDR:
			len = strlen((const char *)setting_params->vice_server_ipaddr) + 1;
			break;

		case PARAMS_ID_PHONE_MONITOR_CENTER:
			len = strlen((const char *)setting_params->phone_number_monitor_center) + 1;
			break;

		case PARAMS_ID_PHONE_RESET:
			len = strlen((const char *)setting_params->phone_number_reset) + 1;
			break;

		case PARAMS_ID_PHONE_FACTORY_RECOVERY:
			len = strlen((const char *)setting_params->phone_number_factory_recovery) + 1;
			break;

		case PARAMS_ID_PHONE_SMS_CENTER:
			len = strlen((const char *)setting_params->phone_number_sms_center) + 1;
			break;

		case PARAMS_ID_PHONE_SMS_REV:
			len = strlen((const char *)setting_params->phone_number_sms_rev) + 1;
			break;


		case PARAMS_ID_PHONE_NUMBER_MONITOR:
			len = strlen((const char *)setting_params->phone_number_monitor) + 1;
			break;


		case PARAMS_ID_DEVICE_PASSWORD:
			len = strlen((const char *)setting_params->device_password) + 1;
			break;

		case PARAMS_ID_TIME_IDLE_AFTER_ACCOFF:
			len = 2;
			break;
			
		default:
			len = 4;
			break;
	}

	DbgFuncExit();

	return len;
}


static int gb905_build_id(unsigned short id,unsigned char *buf)
{
	int len = 0;
	unsigned int temp;
	unsigned short temp1;
	setting_params_t * setting_params;
	
	DbgFuncEntry();

	setting_params = get_setting_params();

	DbgPrintf("id = 0x%x\r\n",id);
	
	switch(id)
	{
		case PARAMS_ID_INTERVAL_HEARTBEAT:
			temp = EndianReverse32(setting_params->heartbeat_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;
			
		case PARAMS_ID_TCP_MSG_ACK_TIMEOUT:
			temp = EndianReverse32(setting_params->tcp_msg_ack_timeout);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_TCP_MSG_RETRY_COUNT:
			temp = EndianReverse32(setting_params->tcp_msg_retry_count);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;
			
		case PARAMS_ID_SMS_MSG_ACK_TIMEOUT:
			temp = EndianReverse32(setting_params->sms_msg_ack_timeout);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_SMS_MSG_RETRY_COUNT:
			temp = EndianReverse32(setting_params->sms_msg_retry_count);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_MAIN_SERVER_APN:
			len = strlen((const char *)setting_params->main_server_apn) + 1;
			strcpy((char *)buf,(const char *)setting_params->main_server_apn);
			break;

		case PARAMS_ID_MAIN_SERVER_USERNAME:
			len = strlen((const char *)setting_params->main_server_apn) + 1;
			strcpy((char *)buf,(const char *)setting_params->main_server_username);
			break;

		case PARAMS_ID_MAIN_SERVER_PASSWORD:
			len = strlen((const char *)setting_params->main_server_password) + 1;
			strcpy((char *)buf,(const char *)setting_params->main_server_password);
			break;
			

		case PARAMS_ID_MAIN_SERVER_IPADDR:
			len = strlen((const char *)setting_params->main_server_ipaddr) + 1;
			strcpy((char *)buf,(const char *)setting_params->main_server_ipaddr);
			break;

		case PARAMS_ID_VICE_SERVER_APN:
			len = strlen((const char *)setting_params->vice_server_apn) + 1;
			strcpy((char *)buf,(const char *)setting_params->vice_server_apn);
			break;	

		case PARAMS_ID_VICE_SERVER_USERNAME:
			len = strlen((const char *)setting_params->vice_server_username) + 1;
			strcpy((char *)buf,(const char *)setting_params->vice_server_username);
			break;

		case PARAMS_ID_VICE_SERVER_PASSWORD:
			len = strlen((const char *)setting_params->vice_server_password) + 1;
			strcpy((char *)buf,(const char *)setting_params->vice_server_password);
			break;

		case PARAMS_ID_VICE_SERVER_IPADDR:
			len = strlen((const char *)setting_params->vice_server_ipaddr) + 1;
			strcpy((char *)buf,(const char *)setting_params->vice_server_ipaddr);
			break;

		case PARAMS_ID_MAIN_SERVER_TCP_PORT:
			temp = EndianReverse32(setting_params->main_server_tcp_port);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_VICE_SERVER_TCP_PORT:
			temp = EndianReverse32(setting_params->vice_server_tcp_port);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_LOCATION_REPORT_POLICY:
			temp = EndianReverse32(setting_params->location_report_policy);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;


		case PARAMS_ID_LOCATION_REPORT_SCHEME:
			temp = EndianReverse32(setting_params->location_report_scheme);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_TIME_REPORT_LOGOUT_INTERVAL:
			temp = EndianReverse32(setting_params->time_report_logout_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_TIME_REPORT_ACCOFF_INTERVAL:
			temp = EndianReverse32(setting_params->time_report_accoff_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_TIME_REPORT_ACCON_INTERVAL:
			temp = EndianReverse32(setting_params->time_report_accon_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_TIME_REPORT_VACANT_INTERVAL:
			temp = EndianReverse32(setting_params->time_report_vacant_internal);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_TIME_REPORT_OCCUPIED_INTERVAL:
			temp = EndianReverse32(setting_params->time_report_occupied_internal);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_TIME_REPORT_IDLE_INTERVAL:
			temp = EndianReverse32(setting_params->time_report_idle_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_TIME_REPORT_ALARM_INTERVAL:
			temp = EndianReverse32(setting_params->time_report_alarm_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;
		
		case PARAMS_ID_DISTANCE_LOGOUT_INTERVAL:
			temp = EndianReverse32(setting_params->distance_report_logout_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_DISTANCE_ACCOFF_INTERVAL:
			temp = EndianReverse32(setting_params->distance_report_accoff_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_DISTANCE_ACCON_INTERVAL:
			temp = EndianReverse32(setting_params->distance_report_accon_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_DISTANCE_VACANT_INTERVAL:
			temp = EndianReverse32(setting_params->distance_report_vacant_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_DISTANCE_OCCUPIED_INTERVAL:
			temp = EndianReverse32(setting_params->distance_report_occupied_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_DISTANCE_IDLE_INTERVAL:
			temp = EndianReverse32(setting_params->distance_report_idle_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_DISTANCE_ALARM_INTERVAL:
			temp = EndianReverse32(setting_params->distance_report_alarm_interval);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_PHONE_MONITOR_CENTER:
			len = strlen((const char *)setting_params->phone_number_monitor_center) + 1;
			strcpy((char *)buf,(const char *)setting_params->phone_number_monitor_center);
			break;

		case PARAMS_ID_PHONE_RESET:
			len = strlen((const char *)setting_params->phone_number_reset) + 1;
			strcpy((char *)buf,(const char *)setting_params->phone_number_reset);
			break;

		case PARAMS_ID_PHONE_FACTORY_RECOVERY:
			len = strlen((const char *)setting_params->phone_number_factory_recovery) + 1;
			strcpy((char *)buf,(const char *)setting_params->phone_number_factory_recovery);
			break;

		case PARAMS_ID_PHONE_SMS_CENTER:
			len = strlen((const char *)setting_params->phone_number_sms_center) + 1;
			strcpy((char *)buf,(const char *)setting_params->phone_number_sms_center);
			break;

		case PARAMS_ID_PHONE_SMS_REV:
			len = strlen((const char *)setting_params->phone_number_sms_rev) + 1;
			strcpy((char *)buf,(const char *)setting_params->phone_number_sms_rev);
			break;

		case PARAMS_ID_PHONE_ANSWER_POLICY:
			temp = EndianReverse32(setting_params->telephone_answer_policy);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_MAX_TIME_TALK_ONCE:
			temp = EndianReverse32(setting_params->max_time_talk_once);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_MAX_TIME_TALK_MONTH:
			temp = EndianReverse32(setting_params->max_time_talk_month);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;


		case PARAMS_ID_SHORT_PHONE_NUMBER_LEN:
			temp = EndianReverse32(setting_params->short_phone_number_len);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_PHONE_NUMBER_MONITOR:
			len = strlen((const char *)setting_params->phone_number_monitor) + 1;
			strcpy((char *)buf,(const char *)setting_params->phone_number_monitor);
			break;


		case PARAMS_ID_DEVICE_PASSWORD:
			len = strlen((const char *)setting_params->device_password) + 1;
			strcpy((char *)buf,(const char *)setting_params->device_password);
			break;

		case PARAMS_ID_TTS_VOLUMN:
			temp = EndianReverse32(setting_params->tts_volume);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;


		case PARAMS_ID_ALARM_MASK:
			temp = EndianReverse32(setting_params->alarm_mask);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_ALARM_SMS_MASK:
			temp = EndianReverse32(setting_params->alarm_sms_mask);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_ALARM_VIDEO_MASK:
			temp = EndianReverse32(setting_params->alarm_video_mask);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_ALARM_VIDEO_SAVE_MASK:
			temp = EndianReverse32(setting_params->alarm_video_save_mask);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_MAX_SPEED:
			temp = EndianReverse32(setting_params->max_speed);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_OVER_SPEED_DURATION:
			temp = EndianReverse32(setting_params->over_speed_duration);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_OVER_DRIVE:
			temp = EndianReverse32(setting_params->over_driver);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_MIN_REST_PERIOD:
			temp = EndianReverse32(setting_params->min_rest_period);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_MAX_PARKING_TIME:
			temp = EndianReverse32(setting_params->max_parking_time);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_DRIVE_TIME_ONE_DAY:
			temp = EndianReverse32(setting_params->driver_time_one_day);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;


		case PARAMS_ID_VIDEO_QUALITY:
			temp = EndianReverse32(setting_params->video_quality);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_VIDEO_BRIGHTNESS:
			temp = EndianReverse32(setting_params->video_brightness);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_VIDEO_CONTRAST:
			temp = EndianReverse32(setting_params->video_contrast);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_VIDEO_SATUATION:
			temp = EndianReverse32(setting_params->video_saturation);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_VIDEO_CHROMA:
			temp = EndianReverse32(setting_params->video_chroma);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_VEHICLE_MILOMETER:
			temp = EndianReverse32(setting_params->vehicle_milometer);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_VEHICLE_PROVINCE_ID:
			temp = EndianReverse32(setting_params->vehicle_province_id);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_VEHICLE_CITY_ID:
			temp = EndianReverse32(setting_params->vehicle_city_id);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_METER_OPERATION_COUNT_LIMIT:
			temp = EndianReverse32(setting_params->meter_operation_count_limit);
			len = sizeof(temp);
			memcpy(buf,&temp,len);
			break;

		case PARAMS_ID_METER_OPERATION_COUNT_TIME:
			len = strlen((const char *)setting_params->meter_operation_count_time) + 1;
			strcpy((char *)buf,(const char *)setting_params->meter_operation_count_time);
			break;

		case PARAMS_ID_TIME_IDLE_AFTER_ACCOFF:
			temp1 = EndianReverse16(setting_params->time_idle_after_accoff);
			len = sizeof(temp1);
			memcpy(buf,&temp1,len);
			break;
			
		default:
			DbgWarn("msg id is not supported!\r\n");
			break;
	}

	DbgFuncExit();

	return len;
}


static void gb905_build_params(unsigned char *buf,int len,unsigned short *id_buf,int id_len,unsigned short msg_serial_number)
{
	gb905_header_t * header;
	msg_params_t * msg_params;
	unsigned short msg_id;
	unsigned short msg_len;
		
	int offset;
	int size;
	int i;

	offset = 1;
	header = (gb905_header_t *)&buf[offset];

	msg_id = MESSAGE_GET_PARAMS_ACK;
	msg_len = len - 3 - sizeof(gb905_header_t);
	gb905_build_header(header,msg_id,msg_len);
	gb905_debug_header(header);

	offset += sizeof(gb905_header_t);
	DbgPrintf("offset = %d\r\n",offset);

	// magic_start gb905_msg_header_t sequence msg_params_t ... xor magic_end
	*(unsigned short *)(&buf[offset]) = EndianReverse16(msg_serial_number);
	offset += 2;

	for(i=0;i<id_len;i++)
	{
		msg_params = (msg_params_t *)&buf[offset];
		offset += sizeof(msg_params_t) - 4;
		
		size = gb905_build_id(id_buf[i],&buf[offset]);
		offset += size;
		DbgPrintf("offset = %d\r\n",offset);
		
		msg_params->len = size;
		msg_params->id = EndianReverse16(id_buf[i]);

		if(offset >= len)
		{
			DbgError("build params error!\r\n");
			break;
		}
	}
	
	DbgFuncExit();
}


//-----
/*
* @brief 	设置参数的处理
* @param buf	  传入线程的参数，此处为NULL
*
* @return  返回终端通用应答的状态
*/
void gb905_get_params_treat(unsigned char *buf,int len)
{
	int i;
	
	unsigned char * ack_buf;
	int ack_len = 0;

	unsigned short *id_list;
	int id_len;

	gb905_header_t * header;
	unsigned short sequence;

	DbgFuncEntry();

	//gb905_get_params_treat(msg->msg_buf + 1 + sizeof(gb905_msg_header_t),header->msg_len,header->msg_serial_number);

	header = (gb905_header_t *)&buf[1];
	id_list = (unsigned short *)&buf[1 + sizeof(gb905_header_t)];
	id_len = header->msg_len / 2;
	sequence = header->msg_serial_number;
	
	DbgPrintf("id_len = %d\r\n",id_len);
	DbgPrintf("sequence = %d\r\n",sequence);

	// magic_start header seq params_list xor magic_end = sizeof(gb905_msg_header_t) + 5 + params_list
	ack_len = sizeof(gb905_header_t) + 5;	
	for(i=0;i<id_len;i++)
	{
		id_list[i] = EndianReverse16(id_list[i]);
		ack_len += get_pamams_id_len(id_list[i]) + sizeof(msg_params_t) - 4;
	}
	
	DbgPrintf("ack_len = %d\r\n",ack_len);
	
	ack_buf = (unsigned char *)malloc(ack_len);
	if(!ack_buf)
	{
		DbgError("Get Params Memory Malloc Failed!\r\n");
		return;
	}
	memset((void*)ack_buf, 0, ack_len);

	gb905_build_params(ack_buf,ack_len,id_list,id_len,sequence);

	gb905_send_data(MAIN_SOCKET,ack_buf,ack_len);
	
	free(ack_buf);
	
	DbgFuncExit();
}



/*
* @brief 	设置参数的处理
* @param buf	  传入线程的参数，此处为NULL
*
* @return  返回终端通用应答的状态
*/
unsigned char gb905_set_params_treat(unsigned char *buf,int len)
{
	unsigned short id;
	int offset = 0;
	msg_params_t * msg_params;

	DbgFuncEntry();
	
	DbgPrintf("offset = %d\r\n",offset);
	DbgPrintf("len = %d\r\n",len);
	
	do{
		msg_params = (msg_params_t *)&buf[offset];
		id = EndianReverse16(msg_params->id);

		DbgPrintf("id = 0x%4x\r\n",id);
		DbgPrintf("params_len = 0x%2x\r\n",msg_params->len);

		gb905_params_single_treat(id,msg_params);
	
		offset += msg_params->len + sizeof(msg_params_t) - 4;
		len -= msg_params->len + sizeof(msg_params_t) - 4;

		DbgPrintf("offset = %d\r\n",offset);
		DbgPrintf("len = %d\r\n",len);
	}while(len > 0);
	
	DbgFuncExit();

	return GB905_RESULT_OK;
}





// 参数使用xml   文件

