/****************************************Copyright (c)**************************************************                         
** File name:			fleety_acc.c
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

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_notice.h"

#include	"middleware/event/acc/fleety_acc.h"

#include	"middleware/info/status.h"
#include	"middleware/info/setting.h"

#include	"app/report/fleety_report.h"



#define		DEBUG_Y
#include	"libs/debug.h"

#define	ACC_MONITOR_SUCCESS_INFO	"�����������ܳɹ�"
#define	ACC_MONITOR_FAIL_INFO	    "������������ʧ�ܣ����ڵ�������"


static	pthread_t	acc_thread_id;
static	pthread_mutex_t acc_lock;
static	pthread_cond_t acc_cond;

/** 
* @brief 	�ն�����״̬��⹦�ܺ���
* 
*/
static void *check_terminal_sleep_func(void *arg)  
{
	int ret;
	setting_params_t * setting;
	struct timeval now;
	struct timespec outtime;

	DbgFuncEntry();
	
	setting = get_setting_params();
	

	pthread_mutex_lock(&acc_lock);

	gettimeofday(&now, NULL);
	outtime.tv_sec = now.tv_sec + setting->time_idle_after_accoff;
	outtime.tv_nsec = now.tv_usec * 1000;

	ret = pthread_cond_timedwait(&acc_cond, &acc_lock, &outtime);
    
	pthread_mutex_unlock(&acc_lock);

	if(ret == 0)	// ---> condition ok
	{
		DbgPrintf("acc on cond ok!\r\n");
	}
	else	// ---> timeout
	{
        set_terminal_sleep_status(true);
		DbgPrintf("acc on timeout!\r\n");
	}
	
	
	DbgFuncExit();
	
    return ((void *)0);
}  

/** 
* @brief 	 �����ն�����״̬����߳�
* 
*/
static void fleety_check_terminal_sleep(void)
{
	int err;

	DbgFuncEntry();
	
	pthread_mutex_init(&acc_lock, NULL);
	pthread_cond_init(&acc_cond, NULL);

	err = pthread_create(&acc_thread_id, NULL, check_terminal_sleep_func, NULL);
    if(err != 0)
    {
    	DbgError("create acc thread error: %s\r\n",strerror(err));  
		return;
	}

	DbgFuncExit();
}

/** 
* @brief 	 ����ն�����״̬
* 
*/
static void fleety_clear_terminal_sleep_state(void)
{
    
    DbgFuncEntry();
    
    pthread_cond_signal(&acc_cond);
    set_terminal_sleep_status(false);

    DbgFuncExit();
}
/** 
* @brief 	 acc  �¼��Ĵ���
* @param acc_on		acc ״̬
* 
*/
void fleety_acc_treat(int acc_state)
{
	
	taxi_status_t status;

    DbgFuncEntry();
    
    taxi_get_status(&status);

	
	DbgPrintf("fleety acc params %d \r\n",acc_state);

	//����ACC״̬
	set_acc_status(acc_state);

	//ACC �仯����������һ��λ�û㱨
	fleety_report();
	
	if(acc_state)
	{
        fleety_clear_terminal_sleep_state();
        
        //�ڼ����Ƿ�����״̬
		if(get_acc_monitor_status())
		{
            if(!status.alarm.flag.illegal_ignition)
            {
                set_illegal_ignition_alarm_status();
                fleety_report();
            }
        
        }
	}
	else
	{
		fleety_check_terminal_sleep();
	}
	
	DbgFuncExit();
}

/** 
* @brief 	 acc ��������(�����Ƿ�������)
* 
*/
void fleety_acc_monitor_func(unsigned char params)
{
    
    DbgFuncEntry();
    
    if(params)
    {
        //��ACC ON״̬�´����Ƿ����������ܲ���Ч
        if(get_acc_status())
        {
            set_acc_monitor_status(true);
            ui_send_terminal_remind((unsigned char*)ACC_MONITOR_SUCCESS_INFO,sizeof(ACC_MONITOR_SUCCESS_INFO));
        }
        else
        {
            ui_send_terminal_remind((unsigned char*)ACC_MONITOR_FAIL_INFO,sizeof(ACC_MONITOR_FAIL_INFO));
        }        
    }
    else
    {
       set_acc_monitor_status(false); 
    }

    DbgFuncExit();
}
