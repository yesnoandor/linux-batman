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
	unsigned int	heartbeat_interval;			// �ն��������ͼ������
	unsigned int	tcp_msg_ack_timeout;		// TCP ��ϢӦ��ʱʱ�䣬��
	unsigned int	tcp_msg_retry_count;		// TCP ��Ϣ�ش�����
	unsigned int	sms_msg_ack_timeout;		// SMS ��ϢӦ��ʱʱ�䣬��
	unsigned int	sms_msg_retry_count;		// SMS ��Ϣ�ش�����
	
	unsigned char 	main_server_apn[32];		// ��������APN������ͨ�Ų��ŷ��ʵ�
	unsigned char 	main_server_username[32];	// ������������ͨ�Ų����û���
	unsigned char 	main_server_password[32];	// ������������ͨ�Ų�������
	unsigned char 	main_server_ipaddr[32];		// ����������ַ,IP ������
	unsigned char 	vice_server_apn[32];		// ���ݷ�����APN������ͨ�Ų��ŷ��ʵ�
	unsigned char 	vice_server_username[32];	// ���ݷ���������ͨ�Ų����û���
	unsigned char 	vice_server_password[32];	// ���ݷ���������ͨ�Ų�������
	unsigned char 	vice_server_ipaddr[32];		// ���ݷ�������ַ,IP ������
	unsigned int	main_server_tcp_port;		// ��������TCP �˿�
	unsigned int	vice_server_tcp_port;		// ���ݷ�����TCP �˿�

	unsigned char	pay_main_server_apn[32];	// ֧��ƽ̨����������ַ
	unsigned int	pay_main_server_tcp_port;	// ֧��ƽ̨��������TCP �˿�
	unsigned char	pay_vice_server_apn[32];	// ֧��ƽ̨���ݷ�������ַ
	unsigned int	pay_vice_server_tcp_port;	// ֧��ƽ̨���ݷ�����TCP �˿�

	unsigned int	location_report_policy;		// λ�û㱨���� : =0,��ʱ�㱨; =1,����㱨; =2,��ʱ + ����
	unsigned int	location_report_scheme;		// λ�û㱨���� : =0,����ACC״̬; =1,���ݿ��س�״̬;=2,���ݵ�¼״̬ + ACC ״̬;=3,���ݵ�¼״̬ + ���س�״̬

	unsigned int	time_report_logout_interval;		// δ��¼�㱨ʱ�������룬>0
	unsigned int	time_report_accoff_interval;		// ACC OFF �㱨ʱ�������룬>0
	unsigned int	time_report_accon_interval;			// ACC ON �㱨ʱ�������룬>0
	unsigned int	time_report_vacant_internal;		// �ճ��㱨ʱ�������룬>0
	unsigned int	time_report_occupied_internal;		// �س��㱨ʱ�������룬>0
	unsigned int	time_report_idle_interval;			// ����ʱ�㱨ʱ�������룬>0
	unsigned int	time_report_alarm_interval;			// ��������ʱ�㱨ʱ�������룬>0 

	unsigned int	distance_report_logout_interval;	// δ��¼�㱨���������ף�>0
	unsigned int	distance_report_accoff_interval;	// ACC OFF �㱨���������ף�>0
	unsigned int	distance_report_accon_interval;		// ACC ON �㱨���������ף�>0
	unsigned int	distance_report_vacant_interval;	// �ճ��㱨���������ף�>0
	unsigned int	distance_report_occupied_interval;	// �س��㱨���������ף�>0
	unsigned int	distance_report_idle_interval;		// ����ʱ�㱨���������ף�>0
	unsigned int	distance_report_alarm_interval;		// ��������ʱ�㱨���������ף�>0
	unsigned int	report_angle_interval;				// �յ㲹���Ƕȣ�<180
	
	unsigned char	phone_number_monitor_center[16];	// ���ָ�����ĵ绰����
	unsigned char	phone_number_reset[16];				// ��λ�绰����
	unsigned char	phone_number_factory_recovery[16];	// �ָ��������õ绰����
	unsigned char	phone_number_sms_center[16];		// ���ָ������SMS �绰����
	unsigned char	phone_number_sms_rev[16];			// �����ն�SMS �ı���������

	unsigned int	telephone_answer_policy;			// �ն˵绰�������ԣ�=0,�Զ�����;=1,ACC ON ʱ�Զ�����,ACC OFFʱ�ֶ�����
	unsigned int	max_time_talk_once;					// ÿ���ͨ��ʱ�䣬��
	unsigned int	max_time_talk_month;				// �����ͨ��ʱ�䣬��
	unsigned int	short_phone_number_len;				// �绰�̺ų���

	unsigned char	phone_number_monitor[16];			// �����绰����
	unsigned char 	device_password[32];				// �ն��豸ά������
	unsigned int	tts_volume;							// �ն˵�����������������0-9 : =0,����;=9,���

	unsigned int	alarm_mask;							// ����������: ��λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ,��ӦλΪ1,����Ӧ����������
	unsigned int	alarm_sms_mask;						// ���������ı�SMS����: ��λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ,��ӦλΪ1,����Ӧ����ʱ�����ı�SMS
	unsigned int	alarm_video_mask;					// �������㿪��:��λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ,��ӦλΪ1,����Ӧ����ʱ����ͷ����
	unsigned int	alarm_video_save_mask;				// ��������洢��־:��λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ,��ӦλΪ1,�����Ӧ����ʱ�ĵ���Ƭ���д洢,����ʵʱ�ϴ�
	
	unsigned int	max_speed;							// ����ٶȣ�KM/H
	unsigned int	over_speed_duration;				// ���ٳ���ʱ�䣬��
	unsigned int	over_driver;						// ������ʻʱ������
	unsigned int	min_rest_period;					// ��С��Ϣʱ�䣬��
	unsigned int	max_parking_time;					// �ͣ��ʱ�䣬��
	unsigned int	driver_time_one_day;				// �����ۼƼ�ʻʱ������
	
	unsigned int	video_quality;						// ͼ��/��Ƶ����: 1��10��1 ���			
	unsigned int	video_brightness;					// ����: 0��255
	unsigned int	video_contrast;						// �Աȶ�: 0��127
	unsigned int	video_saturation;					// ���Ͷ�: 0��127
	unsigned int	video_chroma;						// ɫ��: 0��255
	
	unsigned int	vehicle_milometer;					// ������̱����: 1/10KM
	unsigned int	vehicle_province_id;				// �������ڵ�ʡ��ID: 1��255
	unsigned int	vehicle_city_id;					// �������ڵ�����ID: 1��255
	unsigned int	meter_operation_count_limit;		// �Ƽ���Ӫ�˴�������: 0-9999;=0,��ʾ��������
	unsigned char	meter_operation_count_time[5];		// �Ƽ���Ӫ��ʱ������: YYYY-MM-DD-hh;=0000000000,��ʾ��������
	unsigned char	company_license_number[16];			// ���⳵��ҵӪ�����֤��
	unsigned char	company_name[32];					// ���⳵��ҵ���	
	unsigned char	plate_number[7];					// ���⳵���ƺ�(ASCII ��+ '\0' )


	unsigned int	voice_record_mode;					// �����ն�¼��ģʽ: =0X01,ȫ��¼��;=0X02,����¼��
	unsigned int	voice_record_period;				// �����ն�¼���ļ����ʱ��:��1-255������
	unsigned int	lcd_heart_beat_time;				// Һ����LCD������ʱ����: (��λS 1-60)
	unsigned int	led_heart_beat_time;				// LED ����ʱ����: (��λS 1-60)
	
	unsigned int	time_idle_after_accoff;				// ACC OFF ���������ģʽ��ʱ��: ��λS

	unsigned int	video_protocol_mode;				// ��Ƶ������Э��ģʽ(=0x00,TCP;	=0x01,UDP)
	unsigned char	video_server_apn[32];				// ��Ƶ������APN������ͨ�Ų��ŷ��ʵ�
	unsigned char 	video_server_username[32];			// ��Ƶ����������ͨ�Ų����û���
	unsigned char	video_server_password[32];			// ��Ƶ����������ͨ�Ų�������
	unsigned char	video_server_ipaddr[32];			// ��Ƶ��������ַ,IP ������
	unsigned int	vidoe_server_port;					// ��Ƶ�������˿�
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
	unsigned char company_license_number[16];			// ��˾��Ӫ���֤��(ASCII��,  ����16  �ֽڣ��Ҳ�0)
	unsigned char driver_license_number[19];			// ��ʻԱ��ҵ�ʸ�֤��(ASCII��,  ����19  �ֽڣ��Ҳ�0)
	unsigned char plate_number[6];						// ���ƺ�(ASCII ��)
}__packed license_params_t;

void get_license_params(license_params_t * params);
void set_license_params(license_params_t * params);
void set_driver_license_number(unsigned char * driver_license_number,int len);


#ifdef __cplusplus
}
#endif

#endif



