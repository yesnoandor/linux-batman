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

// ���״̬����
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned login:1;				// =0,ǩ�� ;=1,��½
		unsigned trace:1;				// =0,λ�û㱨;=1,λ�ø���
		unsigned sleep:1;				// =0,����״̬;=1,���ߴ���״̬
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
}taxi_status_t;

void taxi_get_status(taxi_status_t * status);


bool get_trace_mode(void);
void start_trace_mode(void);
void stop_trace_mode(void);


bool get_meter_connected_status(void);
void set_meter_connected(void);
void set_meter_unconnected(void);

bool get_toplight_connected_status(void);
void set_toplight_connected(void);
void set_toplight_unconnected(void);


bool get_acc_status(void);
void set_acc_status(bool on);

bool get_loading_status(void);
void set_loading_status(bool loading);

bool get_alarm_status(void);
void set_alarm_status(bool alarm);

void set_emergency_alarm_status(void);
void reset_emergency_alarm_status(void);


#ifdef __cplusplus
}
#endif

#endif

