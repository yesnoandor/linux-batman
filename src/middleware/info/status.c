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

#include	"misc/check.h"



//#include	"module/gb905/gb905_common.h"
//#include	"module/gb905/report/gb905_report.h"

#include	"middleware/db/file/db_file_bin.h"
#include	"middleware/db/file/db_file_path.h"
#include	"middleware/info/status.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define     STATUS_FILE                "status_info.sys"
#define     STATUS_BACKUP_FILE         "status_info.bak"


static taxi_status_t taxi_status;

static void fleety_taxi_status_save(taxi_status_t *info)
{
    char path[MAX_PATH_CHARS_SIZE];

    DbgFuncEntry();

	info->crc = crc16_compute((unsigned char *)info,sizeof(taxi_status_t)-2,NULL);

	DbgPrintf("crc = 0x%x\r\n",info->crc);

	build_db_path(path,STATUS_FILE);
	buf2file(path,0,(unsigned char *)info,sizeof(taxi_status_t));
	build_db_path(path,STATUS_BACKUP_FILE);
	buf2file(path,0,(unsigned char *)info,sizeof(taxi_status_t));

    DbgFuncExit();
}

void fleety_taxi_status_init(void)
{
	int ret;
	int state = 0;
	taxi_status_t info,info_buckup;
	unsigned short crc;
	char path[MAX_PATH_CHARS_SIZE];
	
	DbgFuncEntry();
	
	build_db_path(path,STATUS_FILE);
	ret = file2buf(path,0,(unsigned char*)&info,sizeof(taxi_status_t));
    DbgPrintf("ret %d\r\n",ret);
    if(ret)
	{
		crc = crc16_compute((unsigned char *)&info,sizeof(taxi_status_t)-2,NULL);
        DbgPrintf("crc /info.crc 0x%x/0x%x\r\n",crc,info.crc);
		if(info.crc != crc)
		{
			memset(&info,0x00,sizeof(taxi_status_t));
			state &= ~0x01;
		}
		else
		{
			state |= 0x01;
		}
	}
	
	build_db_path(path,STATUS_BACKUP_FILE);
	ret = file2buf(path,0,(unsigned char *)&info_buckup,sizeof(taxi_status_t));
    DbgPrintf("ret %d\r\n",ret);
    if(ret)
	{
		crc = crc16_compute((unsigned char *)&info_buckup,sizeof(taxi_status_t)-2,NULL);
        DbgPrintf("crc /info.crc 0x%x / 0x%x\r\n",crc,info_buckup.crc);
        if(info_buckup.crc != crc)
		{
			memset(&info_buckup,0x00,sizeof(taxi_status_t));
			state &= ~0x02;
		}
		else
		{
			state |= 0x02;
		}
	}
    
	DbgPrintf("state = 0x%x\r\n",state);

	switch (state)
	{
		case 0:	// �����ļ�������ȷ
			memset(&taxi_status,0x00,sizeof(taxi_status_t));
			
			build_db_path(path,STATUS_FILE);
			buf2file(path,0,(unsigned char *)&taxi_status,sizeof(taxi_status_t));

			build_db_path(path,STATUS_BACKUP_FILE);
			buf2file(path,0,(unsigned char *)&taxi_status,sizeof(taxi_status_t));
			break;
			
		case 1:	// �����ļ���ȷ
			memcpy((void *)(&taxi_status),(void *)&info_buckup,sizeof(taxi_status_t));
			build_db_path(path,STATUS_FILE);
			buf2file(path,0,(unsigned char *)&taxi_status,sizeof(taxi_status_t));
			break;

		case 2:	// ���ļ���ȷ
			memcpy((void *)&taxi_status,(void *)&info,sizeof(taxi_status_t));
			build_db_path(path,STATUS_BACKUP_FILE);
			buf2file(path,0,(unsigned char *)&taxi_status,sizeof(taxi_status_t));
			break;
			
		case 3:	// �����ļ�����ȷ
			memcpy((void *)&taxi_status,(void *)&info,sizeof(taxi_status_t));
			break;

		default:
			DbgError("taxi status info error!\r\n");
			break;
	}

	DbgFuncExit();
}


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
    fleety_taxi_status_save(&taxi_status);
}

void set_logout_mode(void)
{
	taxi_status.sw.flag.login = 0;
    fleety_taxi_status_save(&taxi_status);
}

//��������״̬����
bool get_gateway_mode(void)
{
	return taxi_status.sw.flag.gateway;
}

void set_gateway_connected(void)
{
	taxi_status.sw.flag.gateway = 1;
}

void set_gateway_unconnected(void)
{
	taxi_status.sw.flag.gateway = 0;
}

//����״̬����
bool get_order_state(void)
{
    return taxi_status.sw.flag.order;
}

void set_order_state(bool state)
{
    taxi_status.sw.flag.order = state;
    fleety_taxi_status_save(&taxi_status);
}


//���ö����Ƿ���Ѳ��״̬
void set_led_insp_status(bool state)
{
	taxi_status.sw.flag.led_insp = state;
    fleety_taxi_status_save(&taxi_status);
}

//��ȡ�����Ƿ���Ѳ��״̬
bool get_led_insp_status(void)
{
	return taxi_status.sw.flag.led_insp;
}


//����TSMģ���Ƿ���Ѳ��״̬
void set_tsm_insp_status(bool state)
{
	taxi_status.sw.flag.tsm_insp = state;
    fleety_taxi_status_save(&taxi_status);
}

//��ȡ�����Ƿ���Ѳ��״̬
bool get_tsm_insp_status(void)
{
	return taxi_status.sw.flag.tsm_insp;
}

// ACC  ״̬����
bool get_acc_status(void)
{
	return taxi_status.hw.flag.acc;
}

void set_acc_status(bool on)
{
	taxi_status.hw.flag.acc = on;
    fleety_taxi_status_save(&taxi_status);
}

//acc ����״̬����(�����Ƿ������)
bool get_acc_monitor_status(void)
{
   return taxi_status.sw.flag.acc_monitor;
}

void set_acc_monitor_status(bool state)
{
    taxi_status.sw.flag.acc_monitor = state;
    fleety_taxi_status_save(&taxi_status);
}

//�ն�����״̬����
bool get_terminal_sleep_status(void)
{
    return taxi_status.sw.flag.sleep;
}

void set_terminal_sleep_status(bool state)
{
    taxi_status.sw.flag.sleep = state;
    fleety_taxi_status_save(&taxi_status);
}

// ���س�״̬����
bool get_loading_status(void)
{
	return taxi_status.hw.flag.loading;
}

void set_loading_status(bool loading)
{
	taxi_status.hw.flag.loading = loading;
    fleety_taxi_status_save(&taxi_status);
}


//������·״̬����
void set_oil_status(unsigned char state)
{
	taxi_status.hw.flag.oil = state;
    fleety_taxi_status_save(&taxi_status);
}

//���õ�·״̬����
void set_circuit_status(unsigned char state)
{
	taxi_status.hw.flag.circuit = state;
    fleety_taxi_status_save(&taxi_status);
}

//���ó�����״̬����
void set_door_lock_status(unsigned char state)
{
	taxi_status.hw.flag.door_lock= state;
    fleety_taxi_status_save(&taxi_status);
}

//���ó�����״̬����
void set_car_lock_status(unsigned char state)
{
	taxi_status.hw.flag.car_lock= state;
    fleety_taxi_status_save(&taxi_status);
}

// ������ť״̬����
bool get_alarm_status(void)
{
	return taxi_status.hw.flag.alarm;
}

void set_alarm_status(bool alarm)
{
	taxi_status.hw.flag.alarm = alarm;
    fleety_taxi_status_save(&taxi_status);
}


// ����Ԥ��״̬( ������ť����)
void set_pre_alarm_status(void)
{
	taxi_status.alarm.flag.early_warning = 1;
    fleety_taxi_status_save(&taxi_status);
}

// ���Ԥ��״̬( ���������)
void reset_pre_alarm_status(void)
{
	taxi_status.alarm.flag.early_warning = 0;
    fleety_taxi_status_save(&taxi_status);
}

// ���ý�������״̬( ����������)
void set_emergency_alarm_status(void)
{
	taxi_status.alarm.flag.emergency = 1;
    fleety_taxi_status_save(&taxi_status);
}

// �����������״̬( ���������)
void reset_emergency_alarm_status(void)
{
	taxi_status.alarm.flag.emergency = 0;
    fleety_taxi_status_save(&taxi_status);
}

unsigned int get_alarm_whole_status(void)
{
	return taxi_status.alarm.whole;
}

void set_alarm_whole_status(int status)
{
	taxi_status.alarm.whole = status;
    fleety_taxi_status_save(&taxi_status);
}



void set_weight2empty_status(void)
{
	taxi_status.status.flag.weighting = 0;
	taxi_status.status.flag.emptying = 1;
}

void set_empty2weight_status(void)
{
	taxi_status.status.flag.weighting = 1;
	taxi_status.status.flag.emptying = 0;
}

void reset_weight_empty_status(void)
{
	taxi_status.status.flag.weighting = 0;
	taxi_status.status.flag.emptying = 0;
}


//�Ƽ������ϱ���״̬����
void set_meter_alarm_status(void)
{
	taxi_status.alarm.flag.meter_fault = 1;
    fleety_taxi_status_save(&taxi_status);
}

void clr_meter_alarm_status(void)
{
	taxi_status.alarm.flag.meter_fault = 0;
    fleety_taxi_status_save(&taxi_status);
		
}

//���ƹ��ϱ���״̬����
void set_led_light_alarm_status(void)
{
	taxi_status.alarm.flag.led_light_fault = 1;
    fleety_taxi_status_save(&taxi_status);
}

void clr_led_light_alarm_status(void)
{
	taxi_status.alarm.flag.led_light_fault = 0;
    fleety_taxi_status_save(&taxi_status);
		
}

// TSMģ����ϱ���״̬����
void set_tsm_alarm_status(void)
{
	taxi_status.alarm.flag.tsm_fault = 1;
    fleety_taxi_status_save(&taxi_status);
}

void clr_tsm_alarm_status(void)
{
	taxi_status.alarm.flag.tsm_fault = 0;
    fleety_taxi_status_save(&taxi_status);
		
}

//Ƿѹ����״̬����
void set_under_voltage_alarm_status(void)
{
    taxi_status.alarm.flag.under_voltage = 1;
    fleety_taxi_status_save(&taxi_status);
}

void clr_under_voltage_alarm_status(void)
{
    taxi_status.alarm.flag.under_voltage = 0;
    fleety_taxi_status_save(&taxi_status);
}

//���籨��״̬����
void set_power_down_alarm_status(void)
{
    taxi_status.alarm.flag.power_down = 1;
    fleety_taxi_status_save(&taxi_status);
}

void clr_power_down_alarm_status(void)
{
    taxi_status.alarm.flag.power_down = 0;
   fleety_taxi_status_save(&taxi_status);
}

//����ͷ���ϱ���״̬����
void set_camera_alarm_status(void)
{
    taxi_status.alarm.flag.camera_fault = 1;
    fleety_taxi_status_save(&taxi_status);
}

void clr_camera_alarm_status(void)
{
    taxi_status.alarm.flag.camera_fault = 0;
    fleety_taxi_status_save(&taxi_status);
}

//�����Ƿ���𱨾�״̬����
void set_illegal_ignition_alarm_status(void)
{
    taxi_status.alarm.flag.illegal_ignition = 1;
    fleety_taxi_status_save(&taxi_status);
}

void clr_illegal_ignition_alarm_status(void)
{
    taxi_status.alarm.flag.illegal_ignition = 0;
    fleety_taxi_status_save(&taxi_status);
}
