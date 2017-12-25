/****************************************Copyright (c)**************************************************                         
** File name:			setting.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-11-12
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _SETTING_H
#define	_SETTING_H

#include	"module/gb905/report/gb905_trace.h"
#include	"module/gb905_peri/meter/gb905_meter.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	unsigned int	heartbeat_interval;			// 终端心跳发送间隔，秒
	unsigned int	tcp_msg_ack_timeout;		// TCP 消息应答超时时间，秒
	unsigned int	tcp_msg_retry_count;		// TCP 消息重传次数
	unsigned int	sms_msg_ack_timeout;		// SMS 消息应答超时时间，秒
	unsigned int	sms_msg_retry_count;		// SMS 消息重传次数
	
	unsigned char 	main_server_apn[32];		// 主服务器APN，无线通信拨号访问点
	unsigned char 	main_server_username[32];	// 主服务器无线通信拨号用户名
	unsigned char 	main_server_password[32];	// 主服务器无线通信拨号密码
	unsigned char 	main_server_ipaddr[32];		// 主服务器地址,IP 或域名
	unsigned char 	vice_server_apn[32];		// 备份服务器APN，无线通信拨号访问点
	unsigned char 	vice_server_username[32];	// 备份服务器无线通信拨号用户名
	unsigned char 	vice_server_password[32];	// 备份服务器无线通信拨号密码
	unsigned char 	vice_server_ipaddr[32];		// 备份服务器地址,IP 或域名
	unsigned int	main_server_tcp_port;		// 主服务器TCP 端口
	unsigned int	vice_server_tcp_port;		// 备份服务器TCP 端口

	unsigned char	pay_main_server_apn[32];	// 支付平台主服务器地址
	unsigned int	pay_main_server_tcp_port;	// 支付平台主服务器TCP 端口
	unsigned char	pay_vice_server_apn[32];	// 支付平台备份服务器地址
	unsigned int	pay_vice_server_tcp_port;	// 支付平台备份服务器TCP 端口

	unsigned int	location_report_policy;		// 位置汇报策略 : =0,定时汇报; =1,定距汇报; =2,定时 + 定距
	unsigned int	location_report_scheme;		// 位置汇报方案 : =0,根据ACC状态; =1,根据空重车状态;=2,根据登录状态 + ACC 状态;=3,根据登录状态 + 空重车状态

	unsigned int	time_report_logout_interval;		// 未登录汇报时间间隔，秒，>0
	unsigned int	time_report_accoff_interval;		// ACC OFF 汇报时间间隔，秒，>0
	unsigned int	time_report_accon_interval;			// ACC ON 汇报时间间隔，秒，>0
	unsigned int	time_report_vacant_internal;		// 空车汇报时间间隔，秒，>0
	unsigned int	time_report_occupied_internal;		// 重车汇报时间间隔，秒，>0
	unsigned int	time_report_idle_interval;			// 休眠时汇报时间间隔，秒，>0
	unsigned int	time_report_alarm_interval;			// 紧急报警时汇报时间间隔，秒，>0 

	unsigned int	distance_report_logout_interval;	// 未登录汇报距离间隔，米，>0
	unsigned int	distance_report_accoff_interval;	// ACC OFF 汇报距离间隔，米，>0
	unsigned int	distance_report_accon_interval;		// ACC ON 汇报距离间隔，米，>0
	unsigned int	distance_report_vacant_interval;	// 空车汇报距离间隔，米，>0
	unsigned int	distance_report_occupied_interval;	// 重车汇报距离间隔，米，>0
	unsigned int	distance_report_idle_interval;		// 休眠时汇报距离间隔，米，>0
	unsigned int	distance_report_alarm_interval;		// 紧急报警时汇报距离间隔，米，>0
	unsigned int	report_angle_interval;				// 拐点补传角度，<180
	
	unsigned char	phone_number_monitor_center[16];	// 监控指挥中心电话号码
	unsigned char	phone_number_reset[16];				// 复位电话号码
	unsigned char	phone_number_factory_recovery[16];	// 恢复出厂设置电话号码
	unsigned char	phone_number_sms_center[16];		// 监控指挥中心SMS 电话号码
	unsigned char	phone_number_sms_rev[16];			// 接收终端SMS 文本报警号码

	unsigned int	telephone_answer_policy;			// 终端电话接听策略，=0,自动接听;=1,ACC ON 时自动接听,ACC OFF时手动接听
	unsigned int	max_time_talk_once;					// 每次最长通话时间，秒
	unsigned int	max_time_talk_month;				// 当月最长通话时间，秒
	unsigned int	short_phone_number_len;				// 电话短号长度

	unsigned char	phone_number_monitor[16];			// 监听电话号码
	unsigned char 	device_password[32];				// 终端设备维护密码
	unsigned int	tts_volume;							// 终端的语音播报音量控制0-9 : =0,静音;=9,最高

	unsigned int	alarm_mask;							// 报警屏蔽字: 与位置信息汇报消息中的报警标志相对应,相应位为1,则相应报警被屏蔽
	unsigned int	alarm_sms_mask;						// 报警发送文本SMS开关: 与位置信息汇报消息中的报警标志相对应,相应位为1,则相应报警时发送文本SMS
	unsigned int	alarm_video_mask;					// 报警拍摄开关:与位置信息汇报消息中的报警标志相对应,相应位为1,则相应报警时摄像头拍摄
	unsigned int	alarm_video_save_mask;				// 报警拍摄存储标志:与位置信息汇报消息中的报警标志相对应,相应位为1,则对相应报警时拍的照片进行存储,否则实时上传
	
	unsigned int	max_speed;							// 最高速度，KM/H
	unsigned int	over_speed_duration;				// 超速持续时间，秒
	unsigned int	over_driver;						// 连续驾驶时间门限
	unsigned int	min_rest_period;					// 最小休息时间，秒
	unsigned int	max_parking_time;					// 最长停车时间，秒
	unsigned int	driver_time_one_day;				// 当天累计驾驶时间门限
	
	unsigned int	video_quality;						// 图像/视频质量: 1～10，1 最好			
	unsigned int	video_brightness;					// 亮度: 0～255
	unsigned int	video_contrast;						// 对比度: 0～127
	unsigned int	video_saturation;					// 饱和度: 0～127
	unsigned int	video_chroma;						// 色度: 0～255
	
	unsigned int	vehicle_milometer;					// 车辆里程表读数: 1/10KM
	unsigned int	vehicle_province_id;				// 车辆所在的省域ID: 1～255
	unsigned int	vehicle_city_id;					// 车辆所在的市域ID: 1～255
	unsigned int	meter_operation_count_limit;		// 计价器营运次数限制: 0-9999;=0,表示不做限制
	unsigned char	meter_operation_count_time[5];		// 计价器营运时间限制: YYYY-MM-DD-hh;=0000000000,表示不做限制
	unsigned char	company_license_number[16];			// 出租车企业营运许可证号
	unsigned char	company_name[32];					// 出租车企业简称	
	unsigned char	plate_number[7];					// 出租车车牌号(ASCII 码+ '\0' )


	unsigned int	voice_record_mode;					// 智能终端录音模式: =0X01,全程录音;=0X02,翻牌录音
	unsigned int	voice_record_period;				// 智能终端录音文件最大时长:（1-255）分钟
	unsigned int	lcd_heart_beat_time;				// 液晶（LCD）心跳时间间隔: (单位S 1-60)
	unsigned int	led_heart_beat_time;				// LED 心跳时间间隔: (单位S 1-60)
	
	unsigned int	time_idle_after_accoff;				// ACC OFF 后进入休眠模式的时间: 单位S

	unsigned int	video_protocol_mode;				// 视频服务器协议模式(=0x00,TCP;	=0x01,UDP)
	unsigned char	video_server_apn[32];				// 视频服务器APN，无线通信拨号访问点
	unsigned char 	video_server_username[32];			// 视频服务器无线通信拨号用户名
	unsigned char	video_server_password[32];			// 视频服务器无线通信拨号密码
	unsigned char	video_server_ipaddr[32];			// 视频服务器地址,IP 或域名
	unsigned int	vidoe_server_port;					// 视频服务器端口
}__packed setting_params_t;

void init_setting_params(void);
setting_params_t * get_setting_params(void);
void set_setting_params(void);
void debug_setting_params(void);



typedef trace_body_t trace_params_t;
void get_trace_setting(trace_params_t * setting);
void set_trace_setting(trace_params_t * setting);
void debug_trace_setting(void);


typedef gb905_meter_parameter_body_t meter_params_t;
meter_params_t * get_meter_params(void);
void debug_meter_params(void);



typedef struct {
	unsigned char company_license_number[16];			// 公司经营许可证号(ASCII码,  不足16  字节，右补0)
	unsigned char driver_license_number[19];			// 驾驶员从业资格证号(ASCII码,  不足19  字节，右补0)
	unsigned char plate_number[6];						// 车牌号(ASCII 码)
}__packed license_params_t;

void get_license_params(license_params_t * params);
void set_license_params(license_params_t * params);
void set_driver_license_number(unsigned char * driver_license_number,int len);


#ifdef __cplusplus
}
#endif

#endif



