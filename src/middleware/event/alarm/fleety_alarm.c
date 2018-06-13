/****************************************Copyright (c)**************************************************                         
** File name:			fleety_alarm.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-16
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

#include	"middleware/info/status.h"

#include	"app/report/fleety_report.h"



#define		DEBUG_Y
#include	"libs/debug.h"

static struct timeval alarm_push_time;
static struct timeval alarm_pop_time;

/** 
* @brief 	 alarm  事件的处理
* @param alarm		alarm 按钮状态
* 
*/
void fleety_alarm_treat(int alarm)
{
	
	DbgFuncEntry();

	DbgPrintf("fleety alarm params %d \r\n",alarm);
	
	//按下报警按钮
	if(!alarm)
	{
		gettimeofday(&alarm_push_time, NULL);
	}

	//松开报警按钮
	else
	{
		
		gettimeofday(&alarm_pop_time, NULL);
		//按下报警按钮的时间大于等于3秒钟，触发预警
		if(alarm_push_time.tv_sec && (alarm_pop_time.tv_sec-alarm_push_time.tv_sec)>=3)
		{
			//设置预警状态
			set_pre_alarm_status();

			//预警立即发送一条位置汇报
			fleety_report();

			alarm_push_time.tv_sec = 0;
		}
	}
	
	DbgFuncExit();
	
}

//收到应答后清0的报警处理
void fleety_clr_alarm_after_report_ack(void)
{
    taxi_status_t status;
    
    DbgFuncEntry();

    taxi_get_status(&status);

    //进出区域/路线
    if(status.alarm.flag.out_of_area)
    {
        
    }
    
    //路段行驶时间不足/过长
    if(status.alarm.flag.road_driving)
    {
        
    }
    
    //禁行路段行驶
    if(status.alarm.flag.prohibited_area)
    {
        
    }
    
    //车辆非法点火
    if(status.alarm.flag.illegal_ignition)
    {
        clr_illegal_ignition_alarm_status();
    }
    
    //车辆非法位移
    if(status.alarm.flag.illegal_displacement)
    {
        
    }
    
    DbgFuncExit();
    
}

