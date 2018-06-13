/****************************************Copyright (c)**************************************************                         
** File name:			status.c
** Descriptions:		获得当前运行的车辆各种状态
**						1. 定位or    
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
		case 0:	// 两个文件都不正确
			memset(&taxi_status,0x00,sizeof(taxi_status_t));
			
			build_db_path(path,STATUS_FILE);
			buf2file(path,0,(unsigned char *)&taxi_status,sizeof(taxi_status_t));

			build_db_path(path,STATUS_BACKUP_FILE);
			buf2file(path,0,(unsigned char *)&taxi_status,sizeof(taxi_status_t));
			break;
			
		case 1:	// 备份文件正确
			memcpy((void *)(&taxi_status),(void *)&info_buckup,sizeof(taxi_status_t));
			build_db_path(path,STATUS_FILE);
			buf2file(path,0,(unsigned char *)&taxi_status,sizeof(taxi_status_t));
			break;

		case 2:	// 主文件正确
			memcpy((void *)&taxi_status,(void *)&info,sizeof(taxi_status_t));
			build_db_path(path,STATUS_BACKUP_FILE);
			buf2file(path,0,(unsigned char *)&taxi_status,sizeof(taxi_status_t));
			break;
			
		case 3:	// 两个文件都正确
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

// ---- 位置跟踪/  位置汇报状态函数
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


// ---- 登陆/  签退状态函数
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

//网关连接状态函数
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

//电召状态函数
bool get_order_state(void)
{
    return taxi_status.sw.flag.order;
}

void set_order_state(bool state)
{
    taxi_status.sw.flag.order = state;
    fleety_taxi_status_save(&taxi_status);
}


//设置顶灯是否在巡检状态
void set_led_insp_status(bool state)
{
	taxi_status.sw.flag.led_insp = state;
    fleety_taxi_status_save(&taxi_status);
}

//获取顶灯是否在巡检状态
bool get_led_insp_status(void)
{
	return taxi_status.sw.flag.led_insp;
}


//设置TSM模块是否在巡检状态
void set_tsm_insp_status(bool state)
{
	taxi_status.sw.flag.tsm_insp = state;
    fleety_taxi_status_save(&taxi_status);
}

//获取顶灯是否在巡检状态
bool get_tsm_insp_status(void)
{
	return taxi_status.sw.flag.tsm_insp;
}

// ACC  状态函数
bool get_acc_status(void)
{
	return taxi_status.hw.flag.acc;
}

void set_acc_status(bool on)
{
	taxi_status.hw.flag.acc = on;
    fleety_taxi_status_save(&taxi_status);
}

//acc 监听状态函数(监听非法点火功能)
bool get_acc_monitor_status(void)
{
   return taxi_status.sw.flag.acc_monitor;
}

void set_acc_monitor_status(bool state)
{
    taxi_status.sw.flag.acc_monitor = state;
    fleety_taxi_status_save(&taxi_status);
}

//终端休眠状态函数
bool get_terminal_sleep_status(void)
{
    return taxi_status.sw.flag.sleep;
}

void set_terminal_sleep_status(bool state)
{
    taxi_status.sw.flag.sleep = state;
    fleety_taxi_status_save(&taxi_status);
}

// 空重车状态函数
bool get_loading_status(void)
{
	return taxi_status.hw.flag.loading;
}

void set_loading_status(bool loading)
{
	taxi_status.hw.flag.loading = loading;
    fleety_taxi_status_save(&taxi_status);
}


//设置油路状态函数
void set_oil_status(unsigned char state)
{
	taxi_status.hw.flag.oil = state;
    fleety_taxi_status_save(&taxi_status);
}

//设置电路状态函数
void set_circuit_status(unsigned char state)
{
	taxi_status.hw.flag.circuit = state;
    fleety_taxi_status_save(&taxi_status);
}

//设置车门锁状态函数
void set_door_lock_status(unsigned char state)
{
	taxi_status.hw.flag.door_lock= state;
    fleety_taxi_status_save(&taxi_status);
}

//设置车辆锁状态函数
void set_car_lock_status(unsigned char state)
{
	taxi_status.hw.flag.car_lock= state;
    fleety_taxi_status_save(&taxi_status);
}

// 报警按钮状态函数
bool get_alarm_status(void)
{
	return taxi_status.hw.flag.alarm;
}

void set_alarm_status(bool alarm)
{
	taxi_status.hw.flag.alarm = alarm;
    fleety_taxi_status_save(&taxi_status);
}


// 设置预警状态( 报警按钮触发)
void set_pre_alarm_status(void)
{
	taxi_status.alarm.flag.early_warning = 1;
    fleety_taxi_status_save(&taxi_status);
}

// 清除预警状态( 服务器清除)
void reset_pre_alarm_status(void)
{
	taxi_status.alarm.flag.early_warning = 0;
    fleety_taxi_status_save(&taxi_status);
}

// 设置紧急报警状态( 服务器设置)
void set_emergency_alarm_status(void)
{
	taxi_status.alarm.flag.emergency = 1;
    fleety_taxi_status_save(&taxi_status);
}

// 清除紧急报警状态( 服务器清除)
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


//计价器故障报警状态函数
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

//顶灯故障报警状态函数
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

// TSM模块故障报警状态函数
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

//欠压报警状态函数
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

//掉电报警状态函数
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

//摄像头故障报警状态函数
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

//车辆非法点火报警状态函数
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
