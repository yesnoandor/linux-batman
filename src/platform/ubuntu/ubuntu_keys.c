/****************************************Copyright (c)**************************************************                         
** File name:			ubuntu_keys.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-24
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
#include	"middleware/event/fleety_event.h"



#define		DEBUG_Y
#include	"libs/debug.h"


static		pthread_t			ubuntu_keys_simu_thread_id;
static		pthread_mutex_t 	ubuntu_keys_simu_lock;

/*
*  @brief 		ͨ������ģ��IN  ͨ������
* 
*/
static void ubuntu_keys_simu_in_channel(int index)
{
	fleety_event_t event;

	DbgFuncEntry();

	DbgPrintf("index = %d\r\n",index);

	event.id = IN_CHANNEL_EVENT;
	event.param = index;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);

	DbgFuncExit();
}

/*
*  @brief 		ͨ������ģ��acc on/acc off ������
* 
*/
static void ubuntu_keys_simu_acc(void)
{
	bool acc_status;
	fleety_event_t event;
	
	DbgFuncEntry();
	
	acc_status = get_acc_status();
	set_acc_status(!acc_status);
	
	event.id = ACC_EVENT;
	event.param = acc_status;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);

	DbgFuncExit();
}

/*
*  @brief 		ͨ������ģ��loading /unloading ������
* 
*/
static void ubuntu_keys_simu_loaded(void)
{
	bool loading_status;
	fleety_event_t event;

	DbgFuncEntry();

	loading_status = get_loading_status();
	set_loading_status(!loading_status);
	
	event.id = LOADING_EVENT;
	event.param = loading_status;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);

	DbgFuncExit();
}

/*
*  @brief 		ͨ������ģ�ⱨ���źŵ�����
* 
*/
static void ubuntu_keys_simu_alarm(void)
{
	fleety_event_t event;
	
	DbgFuncEntry();

	set_alarm_status(true);

	event.id = ALARM_EVENT;
	event.param = 0;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);

	DbgFuncExit();
}


/*
*  @brief 		ͨ������ģ����������� ������
* 
*/
static void  ubuntu_keys_simu_eval(int index)
{
	fleety_event_t event;
	
	DbgFuncEntry();

	event.id = EVAL_EVENT;
	event.param = index;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);

	DbgFuncExit();
}



static void ubuntu_keys_simu_exit(void)
{
	fleety_event_t event;

	DbgFuncEntry();
	
	event.id = EXIT_EVENT;
	event.param = 0;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);

	DbgFuncExit();
}


static void * ubuntu_keys_simu_loop_func(void *arg)  
{
	char str;
	DbgFuncEntry();
	
	while(1)
	{
		pthread_mutex_lock(&ubuntu_keys_simu_lock);

		scanf("%c",&str);
		
		DbgGood("cmd:%c\r\n",str);
		fflush(stdin);			//	getchar();
		
		pthread_mutex_unlock(&ubuntu_keys_simu_lock);
		
		// ģ������ͨ��0 ~ 4  ����
		if(str >= '0' && str <= '4')
		{
			ubuntu_keys_simu_in_channel((int)(str - '0'));
		}
		
		// ģ��acc on/acc off
		if(str == 'c')
		{
			ubuntu_keys_simu_acc();
		}
		
		// ģ��load on/load off
		if(str == 'd')
		{
			ubuntu_keys_simu_loaded();
		}

		// ģ�ⱨ����ť
		if(str == 'p')
		{
			ubuntu_keys_simu_alarm();
		}

		// ģ���������(  7 ~ 9  ����)
		if(str >= '7' && str <= '9')
		{
			ubuntu_keys_simu_eval((int)(str - '7'));
		}

		// �˳�������
		if(str == 'q')
		{
			ubuntu_keys_simu_exit();
		}

		#if 0
		if(str == 'm')
		{
			simulate_monitor_car();
		}
		if(str == 'r')
		{
			simulate_call_phone();
		}

		if(str == 'n')
		{
			simulate_login();
		}

		if(str == 'f')
		{
			simulate_call_status1();
		}

		if(str == 'g')
		{
			simulate_call_status2();
		}

		if(str == 'h')
		{
			simulate_call_coming();
		}
		#endif
		
	}
	
	DbgFuncExit();
	
    return ((void *)0);
}  


//-----
int ubuntu_keys_simu_init(void)
{
	int ret;
	
	pthread_mutex_init(&ubuntu_keys_simu_lock, NULL);

	ret = pthread_create(&ubuntu_keys_simu_thread_id, NULL, ubuntu_keys_simu_loop_func, NULL);
	if(ret != 0)
	{
    	DbgError("create thread error: %s\r\n",strerror(ret));
	}

	return ret;
}

