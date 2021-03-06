/****************************************Copyright (c)**************************************************                         
** File name:			fleety_report.c
** Descriptions:		实现位置汇报，位置汇报应答等功能的应用线程
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

#include	"middleware/info/gps.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"

#include	"middleware/info/status.h"
#include	"middleware/info/setting.h"


#include	"app/report/fleety_report.h"


#define		DEBUG_Y
#include	"libs/debug.h"


static	pthread_t	fleety_report_thread_id;
static	pthread_mutex_t fleety_report_lock;
static	pthread_cond_t fleety_report_cond;

/**
* @brief 	得到需要下次位置汇报的周期
* 
* @return 需要汇报的时间间隔(单位s)
*/
static unsigned int report_get_period(void)
{

	unsigned int timeout = 1;

	taxi_status_t taxi_status;
	setting_params_t * setting;

	DbgFuncEntry();

	taxi_get_status(&taxi_status);
	setting = get_setting_params();
	
	if(0 == setting->location_report_policy)	// 定时汇报策略
	{
		if(taxi_status.alarm.flag.emergency)			// 处于报警状态
		{
			timeout = setting->time_report_alarm_interval;
		}
		else if(taxi_status.sw.flag.sleep)		// 处于休眠待机状态
		{
			timeout = setting->time_report_idle_interval;
		}
		else
		{
			// 位置汇报方案
			switch (setting->location_report_scheme)
			{
				case 0:		// 根据Acc on/off 状态汇报
					if(taxi_status.hw.flag.acc)
					{
						timeout = setting->time_report_accon_interval;
					}
					else
					{
						timeout = setting->time_report_accoff_interval;
					}
					break;

				case 1:		// 根据空/   重车状态汇报
					if(taxi_status.hw.flag.loading)
					{
						timeout = setting->time_report_occupied_internal;
					}
					else
					{
						timeout = setting->time_report_vacant_internal;
					}
					break;

			
				case 2:		// 根据登陆状态+   ACC   状态汇报
					if(taxi_status.sw.flag.login)
					{
						if(taxi_status.hw.flag.acc)
						{
							timeout = setting->time_report_accon_interval;
						}
						else
						{
							timeout = setting->time_report_accoff_interval;
						}
					}
					else
					{
						timeout = setting->time_report_logout_interval;
					}
					break;

				case 3:		// 根据登陆状态+   空/   重车状态汇报
					if(taxi_status.sw.flag.login)
					{
						if(taxi_status.hw.flag.loading)
						{
							timeout = setting->time_report_occupied_internal;
						}
						else
						{
							timeout = setting->time_report_vacant_internal;
						}
					}
					else
					{
						timeout = setting->time_report_logout_interval;
					}
					break;


				default:
					DbgError("location report scheme error!\r\n");
					break;
			}
		}
		
	}
	else if(1 != setting->location_report_policy && 	// distance period
			2 != setting->location_report_policy)		// timer + distance period	
	{
		DbgError("location report policy(%d) error!\r\n",setting->location_report_policy);
	}

	DbgPrintf("report timeout = %d \r\n",timeout);
	
	DbgFuncExit();

	return timeout;
}

/*
* @brief 	得到需要下次位置汇报的距离间隔
* 
* @return 需要汇报的距离间隔(单位m)
*/
static unsigned int report_get_distance(void)
{
	unsigned int distance = 0;

	taxi_status_t taxi_status;
	setting_params_t * setting;
	
	DbgFuncEntry();

	taxi_get_status(&taxi_status);
	setting = get_setting_params();

	if(1 == setting->location_report_policy	||		// 定距汇报策略
	   2 == setting->location_report_policy )		// 定时+    定距汇报策略
	{
		if(taxi_status.hw.flag.alarm)				// 处于报警状态
		{
			distance = setting->distance_report_alarm_interval;
		}
		else if(taxi_status.sw.flag.sleep)			// 处于休眠待机状态
		{
			distance = setting->distance_report_idle_interval;
		}
		else
		{
			// 位置汇报方案
			switch (setting->location_report_scheme)
			{
				case 0:		// 根据Acc on/off 状态汇报
					if(taxi_status.hw.flag.acc)
					{							
						distance = setting->distance_report_accon_interval;
					}
					else
					{
						distance = setting->distance_report_accon_interval;
					}
					break;

				case 1:		// 根据空/   重车状态汇报
					if(taxi_status.hw.flag.loading)
					{
						distance = setting->distance_report_occupied_interval;
					}
					else
					{
						distance = setting->distance_report_vacant_interval;
					}
					break;

			
				case 2:		// 根据登陆状态+   ACC   状态汇报
					if(taxi_status.sw.flag.login)
					{
						if(taxi_status.hw.flag.acc)
						{
							distance = setting->distance_report_accon_interval;
						}
						else
						{
							distance = setting->distance_report_accoff_interval;
						}
					}
					else
					{
						distance = setting->distance_report_logout_interval;
					}
					break;

				case 3:		// 根据登陆状态+   空/   重车状态汇报
					if(taxi_status.sw.flag.login)
					{
						if(taxi_status.hw.flag.loading)
						{
							distance = setting->distance_report_occupied_interval;
						}
						else
						{
							distance = setting->distance_report_vacant_interval;
						}
					}
					else
					{
						distance = setting->distance_report_logout_interval;
					}
					break;


				default:
					DbgError("location report scheme error!\r\n");
					break;
			}
		}
		
	}
	else if(0 != setting->location_report_policy)
	{
		DbgError("location report policy error!\r\n");
	}

	DbgPrintf("report distance = %d \r\n",distance);
	
	DbgFuncExit();

	return distance;
}

/*
* @brief 	位置汇报的处理
* @param report_params	位置汇报处理需要的参数(当前时间/ 距离和累计时间/  距离)
*
* @return 下次位置汇报处理的时间间隔，单位s
*/
static unsigned int report_period_treat(fleety_report_params_t * report_params)
{
	unsigned int timeout_threhold;
	unsigned int distance_threhold;

	gps_info_t gps_info;

	DbgFuncEntry();

	gps_get_info(&gps_info);
		
	timeout_threhold = report_get_period();
	distance_threhold = report_get_distance();

	if(distance_threhold)	// 需要汇报的距离
	{
		report_params->distance += gps_info.speed;

		if(report_params->distance >= distance_threhold)	// 
		{
			report_params->distance = 0;
			gb905_report_send();
		}		
		else if(timeout_threhold)	// 定时+  定距汇报
		{
			report_params->elapse += 1;
			if(report_params->elapse >= timeout_threhold)
			{
				report_params->elapse = 0;
				gb905_report_send();
			}
		}

		timeout_threhold = 1;
	}
	else if(timeout_threhold)	// 定时汇报
	{
		gb905_report_send();
	}
	else			// default: no settting params
	{
		timeout_threhold = 1;
	}

	DbgFuncExit();
	
	return timeout_threhold;
}

/*
* @brief 	位置跟踪的处理
* @param period_params	位置汇报处理需要的参数(当前时间/ 距离和累计时间/  距离)
*
* @return 下次位置汇报处理的时间间隔，单位s
*/
static unsigned int trace_period_treat(fleety_report_params_t * period_params)
{
	unsigned int timeout_threhold;
	unsigned int distance_threhold;
	gps_info_t gps_info;

	trace_params_t trace_params;

	DbgFuncEntry();
		
	gps_get_info(&gps_info);
		
	get_trace_setting(&trace_params);

	if(trace_params.policy == TIME_POLICY)
	{
		timeout_threhold = trace_params.interval;
		distance_threhold = 0;

		if(timeout_threhold)
		{
			gb905_trace_send();
			period_params->total_elapse += trace_params.interval;

			if(period_params->total_elapse >= trace_params.threhold)
			{
				period_params->total_elapse = 0;
				stop_trace_mode();
			}
		}
	}
	else if(trace_params.policy == DISTANCE_POLICY)
	{
		timeout_threhold = 1;
		distance_threhold = trace_params.interval;

		if(distance_threhold)
		{
			period_params->distance += gps_info.speed;
			period_params->total_distance += gps_info.speed;
					
			if(period_params->distance >= trace_params.interval)
			{
				period_params->distance = 0;
				gb905_trace_send();
			}

			if(period_params->total_distance >= trace_params.threhold)
			{
				period_params->total_distance = 0;
				stop_trace_mode();
			}
		}
	}
	else if(trace_params.policy == TIME_DISTANCE_POLICY)
	{
		timeout_threhold = 1;
		distance_threhold = 0;

		if(period_params->elapse >= trace_params.interval)
		{
			period_params->elapse = 0;
			gb905_trace_send();
		}
				
		period_params->elapse += timeout_threhold;
				
		period_params->total_distance += gps_info.speed;
		if(period_params->total_distance >= trace_params.threhold)
		{
			period_params->total_distance = 0;
			stop_trace_mode();
		}
	}
	else if(trace_params.policy == DISTANCE_TIME_POLICY)
	{
		timeout_threhold = 1;
		distance_threhold = trace_params.interval;

		if(distance_threhold)
		{
			period_params->distance += gps_info.speed;
			period_params->total_elapse += 1;
					
			if(period_params->distance >= trace_params.interval)
			{
				period_params->distance = 0;

				gb905_trace_send();
			}

			if(period_params->total_elapse >= trace_params.threhold)
			{
				period_params->total_elapse = 0;
				stop_trace_mode();
			}
		}

	}
	else
	{
		DbgWarn("don't support the policy!(0x%x)",trace_params.policy);
	}

	DbgFuncExit();

	return timeout_threhold;
}


/*
* @brief 	位置汇报循环线程
* @param arg	  传入线程的参数，此处为NULL
*
*/
static void * fleety_report_loop_func(void *arg)  
{
	int ret;

	unsigned int timeout_threhold;

	fleety_report_params_t report_params;
		
	struct timeval now;
	struct timespec outtime;

	DbgFuncEntry();

	report_params.distance = report_params.total_distance =0;
	report_params.elapse = report_params.total_elapse = 0;

	while(1)
	{
		pthread_mutex_lock(&fleety_report_lock);

		// 当前位置汇报模式or    位置跟踪模式
		if(get_trace_mode())
		{
			timeout_threhold = trace_period_treat(&report_params);	
		}
		else
		{
			timeout_threhold = report_period_treat(&report_params);
		}
		
		DbgPrintf("timeout_threhold = 0x%x\r\n",timeout_threhold);

		DbgPrintf("distance = 0x%x\r\n",report_params.distance);
		DbgPrintf("total_distance = 0x%x\r\n",report_params.total_distance);
		DbgPrintf("elapse = 0x%x\r\n",report_params.elapse);
		DbgPrintf("total_elapse = 0x%x\r\n",report_params.total_elapse);

		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec + timeout_threhold;
		outtime.tv_nsec = now.tv_usec * 1000;
    
		ret = pthread_cond_timedwait(&fleety_report_cond, &fleety_report_lock, &outtime);
        
		pthread_mutex_unlock(&fleety_report_lock);

		if(ret == 0)	// ---> condition ok
		{
			DbgPrintf("report cond ok!\r\n");
		}
		else	// ---> timeout
		{
			DbgPrintf("report timeout!\r\n");
		}
	}
	
	DbgFuncExit();
	
    return ((void *)0);
} 


/** 
* @brief 	初始化位置汇报功能(  包括位置汇报应答)
*
*/
void fleety_report_init(void)
{
	int err;

	DbgFuncEntry();
	
	pthread_mutex_init(&fleety_report_lock, NULL);
	pthread_cond_init(&fleety_report_cond, NULL);

	err = pthread_create(&fleety_report_thread_id, NULL, fleety_report_loop_func, NULL);
    if(err != 0)
    {
    	DbgError("create report thread error: %s\r\n",strerror(err));  
		return;
	}

	DbgFuncExit();
}

/*
* @brief 	需要立即发送位置汇报消息给服务器
* 
*/
void fleety_report(void)
{
	DbgFuncEntry();
	
	pthread_cond_signal(&fleety_report_cond);

	DbgFuncExit();
}
