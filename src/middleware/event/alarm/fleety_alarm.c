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
* @brief 	 alarm  �¼��Ĵ���
* @param alarm		alarm ��ť״̬
* 
*/
void fleety_alarm_treat(int alarm)
{
	
	DbgFuncEntry();

	DbgPrintf("fleety alarm params %d \r\n",alarm);
	
	//���±�����ť
	if(!alarm)
	{
		gettimeofday(&alarm_push_time, NULL);
	}

	//�ɿ�������ť
	else
	{
		
		gettimeofday(&alarm_pop_time, NULL);
		//���±�����ť��ʱ����ڵ���3���ӣ�����Ԥ��
		if(alarm_push_time.tv_sec && (alarm_pop_time.tv_sec-alarm_push_time.tv_sec)>=3)
		{
			//����Ԥ��״̬
			set_pre_alarm_status();

			//Ԥ����������һ��λ�û㱨
			fleety_report();

			alarm_push_time.tv_sec = 0;
		}
	}
	
	DbgFuncExit();
	
}

//�յ�Ӧ�����0�ı�������
void fleety_clr_alarm_after_report_ack(void)
{
    taxi_status_t status;
    
    DbgFuncEntry();

    taxi_get_status(&status);

    //��������/·��
    if(status.alarm.flag.out_of_area)
    {
        
    }
    
    //·����ʻʱ�䲻��/����
    if(status.alarm.flag.road_driving)
    {
        
    }
    
    //����·����ʻ
    if(status.alarm.flag.prohibited_area)
    {
        
    }
    
    //�����Ƿ����
    if(status.alarm.flag.illegal_ignition)
    {
        clr_illegal_ignition_alarm_status();
    }
    
    //�����Ƿ�λ��
    if(status.alarm.flag.illegal_displacement)
    {
        
    }
    
    DbgFuncExit();
    
}

