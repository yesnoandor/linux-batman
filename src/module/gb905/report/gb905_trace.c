/****************************************Copyright (c)**************************************************                         
** File name:			gb905_trace.c
** Descriptions:		位置跟踪(0x8202/0x0202)   的协议解析
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
#include	"misc/endian.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905/report/gb905_trace.h"

#include	"middleware/info/setting.h"

#include	"middleware/socket/fleety_socket.h"



#define		DEBUG_Y
#include	"libs/debug.h"

/** 
* @brief 	构造整个位置跟踪的数据结构
* @param gb905_report 		整个位置汇报数据结构地址
*
*/
void gb905_build_trace(gb905_report_t * gb905_report)
{
	gb905_build_header(&gb905_report->header,MESSAGE_LOCATION_TRACE,sizeof(report_body_t));
#ifdef DEBUG_Y
	gb905_debug_header(&gb905_report->header);
#endif

	gb905_build_report_body(&gb905_report->report_body);
}



void gb905_trace_send(void)
{
	gb905_report_t gb905_trace;

	DbgFuncEntry();

	gb905_build_trace(&gb905_trace);
	
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&gb905_trace,sizeof(gb905_report_t));

	//gb905_record_msg(MAIN_SOCKET,&gb905_location_report.header,(unsigned char *)&gb905_location_report,sizeof(gb905_location_report_t));

	DbgFuncExit();

}


unsigned char  gb905_trace_treat(unsigned char *buf,int len)
{
	trace_body_t * trace_body;

	DbgFuncEntry();

	trace_body = (trace_body_t *)buf;
	
	trace_body->interval = EndianReverse16(trace_body->interval);
	trace_body->threhold = EndianReverse32(trace_body->threhold);
	
	set_trace_setting(trace_body);

	debug_trace_setting();
	
	DbgFuncExit();
	
	return GB905_RESULT_OK;
}

#if 0
unsigned int trace_period_treat(fleety_report_params_t * period_params)
{
	unsigned int timeout_threhold;
	unsigned int distance_threhold;
	gps_info_t gps_info;

	location_trace_params_t * trace_params;

	DbgFuncEntry();
		
	gps_get_info(&gps_info);
		
	trace_params = get_location_trace_params();

	if(trace_params->policy == LOCATION_TRACE_TIME_POLICY)
	{
		timeout_threhold = trace_params->interval;
		distance_threhold = 0;

		if(timeout_threhold)
		{
			gb905_send_trace();
			period_params->total_elapse += trace_params->interval;

			if(period_params->total_elapse >= trace_params->threhold)
			{
				period_params->total_elapse = 0;
				set_location_trace_status(false);
			}
		}
	}
	else if(trace_params->policy == LOCATION_TRACE_DISTANCE_POLICY)
	{
		timeout_threhold = 1;
		distance_threhold = trace_params->interval;

		if(distance_threhold)
		{
			period_params->distance += gps_info.speed;
			period_params->total_distance += gps_info.speed;
					
			if(period_params->distance >= trace_params->interval)
			{
				period_params->distance = 0;
				gb905_send_trace();
			}

			if(period_params->total_distance >= trace_params->threhold)
			{
				period_params->total_distance = 0;
				set_location_trace_status(false);
			}
		}
	}
	else if(trace_params->policy == LOCATION_TRACE_TIME_DISTANCE_POLICY)
	{
		timeout_threhold = 1;
		distance_threhold = 0;

		if(period_params->elapse >= trace_params->interval)
		{
			period_params->elapse = 0;
			gb905_send_trace();
		}
				
		period_params->elapse += timeout_threhold;
				
		period_params->total_distance += gps_info.speed;
		if(period_params->total_distance >= trace_params->threhold)
		{
			period_params->total_distance = 0;
			set_location_trace_status(false);
		}
	}
	else if(trace_params->policy == LOCATION_TRACE_DISTANCE_TIME_POLICY)
	{
		timeout_threhold = 1;
		distance_threhold = trace_params->interval;

		if(distance_threhold)
		{
			period_params->distance += gps_info.speed;
			period_params->total_elapse += 1;
					
			if(period_params->distance >= trace_params->interval)
			{
				period_params->distance = 0;

				gb905_send_trace();
			}

			if(period_params->total_elapse >= trace_params->threhold)
			{
				period_params->total_elapse = 0;
				set_location_trace_status(false);
			}
		}

	}

	DbgFuncExit();
	
	return timeout_threhold;
}
#endif

