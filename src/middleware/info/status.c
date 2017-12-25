/****************************************Copyright (c)**************************************************                         
** File name:			status.c
** Descriptions:		��õ�ǰ���еĳ�������״̬
**						1. ��λor    
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-11
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

//#include	"module/gb905/gb905_common.h"
//#include	"module/gb905/report/gb905_report.h"

#include	"middleware/info/status.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static taxi_status_t taxi_status;

void taxi_get_status(taxi_status_t * status)
{
	memcpy(status,&taxi_status,sizeof(taxi_status_t));
}

// ---- λ�ø���/  λ�û㱨״̬����
bool get_trace_mode(void)
{
	return taxi_status.sw.flag.trace;
}

void start_trace_mode(void)
{
	taxi_status.sw.flag.trace = 1;
}

void stop_trace_mode(void)
{
	taxi_status.sw.flag.trace = 0;
}


// ---- ��½/  ǩ��״̬����
bool get_login_mode(void)
{
	return taxi_status.sw.flag.login;
}

void set_login_mode(void)
{
	taxi_status.sw.flag.login = 1;
}

void set_logout_mode(void)
{
	taxi_status.sw.flag.login = 0;
}


// �Ƽ���״̬����
bool get_meter_connected_status(void)
{
	return taxi_status.module.flag.meter;
}

void set_meter_connected(void)
{
	taxi_status.module.flag.meter = 1;
}

void set_meter_unconnected(void)
{
	taxi_status.module.flag.meter = 0;
}



// ����״̬����
bool get_toplight_connected_status(void)
{
	return taxi_status.module.flag.toplight;
}

void set_toplight_connected(void)
{
	taxi_status.module.flag.toplight = 1;
}

void set_toplight_unconnected(void)
{
	taxi_status.module.flag.toplight = 0;
}

// ACC  ״̬����
bool get_acc_status(void)
{
	return taxi_status.hw.flag.acc;
}

void set_acc_status(bool on)
{
	taxi_status.hw.flag.acc = on;
}

// ���س�״̬����
bool get_loading_status(void)
{
	return taxi_status.hw.flag.loading;
}

void set_loading_status(bool loading)
{
	taxi_status.hw.flag.loading = loading;
}


// ������ť״̬����
bool get_alarm_status(void)
{
	return taxi_status.hw.flag.alarm;
}

void set_alarm_status(bool alarm)
{
	taxi_status.hw.flag.alarm = alarm;
}


// ���ý�������״̬( ������ť����)
void set_emergency_alarm_status(void)
{
	taxi_status.alarm.flag.emergency = 1;
}

// �����������״̬( ���������)
void reset_emergency_alarm_status(void)
{
	taxi_status.alarm.flag.emergency = 0;
}