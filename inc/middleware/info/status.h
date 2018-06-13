/****************************************Copyright (c)**************************************************                         
** File name:			status.h
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

#ifndef _STATUS_H
#define _STATUS_H

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"


#ifdef __cplusplus
extern "C" {
#endif

#define		ACC_ON			1
#define		ACC_OFF			0

#define		LOADING_ON		1
#define		LOADING_OFF		0

#define		ALARM_POP		1
#define		ALARM_PUSH		0

// ���״̬����
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned login:1;				// =0,ǩ�� ;=1,��½
		unsigned trace:1;				// =0,λ�û㱨;=1,λ�ø���
		unsigned sleep:1;				// =0,����״̬;=1,���ߴ���״̬
		unsigned gateway:1;				// =0,����δ����;=1������������
		unsigned order:1;               // =0�����ڵ���״̬;=1,�ڵ���״̬
		unsigned led_insp:1;			// =0,���Ʋ���Ѳ��;=1,������Ѳ��
		unsigned tsm_insp:1;			// =0,TSMģ�鲻��Ѳ��;=1,TSMģ����Ѳ��
		unsigned acc_monitor:1;         // =0,������acc����״̬;=1,����acc����״̬
		unsigned :0;
    }flag;
}sw_status_t;

// Ӳ��״̬����
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned acc:1;					// =0,ACC ��;=1,ACC ��
		unsigned loading:1;				// =0,�ճ� ;=1,�س�
		unsigned alarm:1;				// =0,ȡ������; =1,����
		
		unsigned oil:1;					// =0,������·����;=1,������·�Ͽ�
		unsigned circuit:1;				// =0,������·����;=1,������·�Ͽ�
		unsigned door_lock:1;			// =0,���Ž���;=1, ���ż���
		unsigned car_lock:1;			// =0,��������;=1, ��������
		unsigned :0;
    }flag;
}hw_status_t;




// ģ��״̬����
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned meter:1;				// =0,unconnected;	=1,connected
		unsigned toplight:1;			// =0,unconnected;	=1,connected
		unsigned tsm:1;					// =0,unconnected;	=1,connected
		unsigned :0;
    }flag;
}module_status_t;


// 
typedef struct {
	sw_status_t	sw;				
	hw_status_t hw;				
	module_status_t module;
	report_alarm_t alarm;
	report_status_t status;
    unsigned short crc;
}__packed taxi_status_t;

void fleety_taxi_status_init(void);

void taxi_get_status(taxi_status_t * status);


bool get_trace_mode(void);
void start_trace_mode(void);
void stop_trace_mode(void);

bool get_login_mode(void);
void set_login_mode(void);
void set_logout_mode(void);

bool get_gateway_mode(void);
void set_gateway_connected(void);
void set_gateway_unconnected(void);

bool get_order_state(void);
void set_order_state(bool state);

void set_led_insp_status(bool state);
bool get_led_insp_status(void);

void set_tsm_insp_status(bool state);
bool get_tsm_insp_status(void);

bool get_acc_status(void);
void set_acc_status(bool on);

bool get_acc_monitor_status(void);
void set_acc_monitor_status(bool state);

bool get_terminal_sleep_status(void);
void set_terminal_sleep_status(bool state);

bool get_loading_status(void);
void set_loading_status(bool loading);

void set_oil_status(unsigned char state);
void set_circuit_status(unsigned char state);
void set_door_lock_status(unsigned char state);
void set_car_lock_status(unsigned char state);

bool get_alarm_status(void);
void set_alarm_status(bool alarm);

void set_pre_alarm_status(void);
void reset_pre_alarm_status(void);
void set_emergency_alarm_status(void);
void reset_emergency_alarm_status(void);

void set_meter_alarm_status(void);
void clr_meter_alarm_status(void);

void set_led_light_alarm_status(void);
void clr_led_light_alarm_status(void);

void set_tsm_alarm_status(void);
void clr_tsm_alarm_status(void);

void set_weight2empty_status(void);
void set_empty2weight_status(void);
void reset_weight_empty_status(void);

void set_under_voltage_alarm_status(void);
void clr_under_voltage_alarm_status(void);

void set_power_down_alarm_status(void);
void clr_power_down_alarm_status(void);

void set_camera_alarm_status(void);
void clr_camera_alarm_status(void);

void set_illegal_ignition_alarm_status(void);
void clr_illegal_ignition_alarm_status(void);

#ifdef __cplusplus
}
#endif

#endif

