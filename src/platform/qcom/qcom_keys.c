/****************************************Copyright (c)**************************************************                         
** File name:			qcom_keys.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-10
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
#include	"middleware/event/fleety_event.h"


#define		DEBUG_Y
#include	"libs/debug.h"



#define		QCOM_KEYPAD_EVENT_DEV		"/dev/input/event2"
#define		QCOM_KEYPAD_BASE_CODE		59



typedef struct{
	int				event_fd;
	
	pthread_t		thread_id;
	pthread_cond_t 	event_cond;
	pthread_mutex_t event_mutex;
	int				pipe_fd[2];

} scorpion_event_private_t,*p_scorpion_event_private_t;


static scorpion_event_private_t scorpion_event_private_data;


/**
*  @brief 		通过本地GPIO   作为acc on/acc off 的输入
* 
*/
static void qcom_keys_acc(int level)
{
	fleety_event_t event;
	
	DbgFuncEntry();
	
	//acc_status = get_acc_status();
	set_acc_status(level);
	
	event.id = ACC_EVENT;
	event.param = level;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);

	DbgFuncExit();
}

/**
*  @brief 		通过本地GPIO   作为loading /unloading 的输入
* 
*/
static void qcom_keys_loaded(int level)
{
	fleety_event_t event;

	DbgFuncEntry();

	//loading_status = get_loading_status();
	set_loading_status(level);
	
	event.id = LOADING_EVENT;
	event.param = level;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);

	DbgFuncExit();
}

/**
*  @brief 		通过本地GPIO   作为报警信号的输入
* 
*/
static void qcom_keys_alarm(int level)
{
	fleety_event_t event;
	
	DbgFuncEntry();

	set_alarm_status(level);
	if(level)
	{
		set_emergency_alarm_status();
	}

	event.id = ALARM_EVENT;
	event.param = level;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);

	DbgFuncExit();
}

/**
*  @brief 		解析本地GPIO  输入的功能
* 
*/
static void event_protocol_ayalyze(unsigned short code,unsigned int value)
{

	unsigned int index;
	unsigned int level;

	DbgFuncEntry();

	index = code - QCOM_KEYPAD_BASE_CODE;
	level = value;

	
	DbgPrintf("index = %d\r\n",index);
	DbgPrintf("level = %d\r\n",level);

	//scorpion_report_event_in(index,level);

	switch (index){
		case 0:
			qcom_keys_loaded(level);
			break;

		case 1:
			qcom_keys_alarm(level);
			break;

		case 2:
			qcom_keys_acc(level);
			break;

		default:
			DbgError("don't support  this key code(%d)!\r\n",code);
			break;
	}

	
	DbgFuncExit();

}


static void * qcom_keys_loop_func(void *dummy)  
{
	p_scorpion_event_private_t scorpion_event;

	struct input_event data;

	int len;

	int exit = 0;

	fd_set readset;
  	int max_fd=0;
  	int ret;

	DbgFuncEntry();

	scorpion_event = (p_scorpion_event_private_t)(dummy);

	if((scorpion_event->event_fd = open(QCOM_KEYPAD_EVENT_DEV, O_RDONLY)) < 0)
    {
        DbgError("can't open event dev %s\r\n", QCOM_KEYPAD_EVENT_DEV);
        return NULL;
    }

	FD_ZERO(&readset);
	FD_SET(scorpion_event->event_fd, &readset);

	max_fd = scorpion_event->event_fd > max_fd ? scorpion_event->event_fd : max_fd;

	FD_SET(scorpion_event->pipe_fd[0],&readset);
	max_fd = scorpion_event->pipe_fd[0] > max_fd ? scorpion_event->pipe_fd[0] : max_fd;

    while(!exit)
    {

    	DbgPrintf("Waiting for event select");

    	ret = select(max_fd+1, &readset, NULL, NULL,NULL);
		DbgPrintf("ret = 0x%x\r\n",ret);

		switch(ret)
		{
			case -1:
				exit = 1;
				DbgError("event select failed!(err = %s)\r\n",strerror(errno));
				break;

			case 0:
				DbgWarn("event timeout!\r\n");
				break;

			default:			
				if(FD_ISSET(scorpion_event->event_fd, &readset))
				{
					len = read(scorpion_event->event_fd, &data, sizeof(data));
      				DbgPrintf("len = 0x%x\r\n",len);
					
					if(len == sizeof(data))
					{
						DbgPrintf("type = 0x%x,code = 0x%x,value = 0x%x\r\n",data.type,data.code,data.value);
						if (data.type == EV_KEY)
        				{
        					event_protocol_ayalyze(data.code,data.value);
        				}

					}
					else
					{
						DbgError("read failed! len = 0x%x\r\n",len);
					}

				}

				if(FD_ISSET(scorpion_event->pipe_fd[0],&readset))
				{
					exit = 1;
					DbgWarn("Exit event thread!\r\n");
				}
        }
	}

	DbgFuncEntry();
	
    return ((void *)0);
}  

//-----
int qcom_keys_init(void)
{
	int ret;

	DbgFuncEntry();
	
	ret = pipe(scorpion_event_private_data.pipe_fd);
	if(ret != 0)
	{
		DbgError("Create event pipe failed!\r\n");
		return ret;
	}
	
	ret = pthread_cond_init(&(scorpion_event_private_data.event_cond),NULL);
	if(ret != 0)
	{
		DbgError("Init event cond failed!\r\n");
		return ret;
	}
	
	ret = pthread_mutex_init(&(scorpion_event_private_data.event_mutex),NULL);
	if(ret != 0)
	{
		DbgError("Init event mutex failed!\r\n");
		return ret;
	}
	
	ret = pthread_create(&(scorpion_event_private_data.thread_id),NULL,qcom_keys_loop_func,(void *)&scorpion_event_private_data); 
	if(ret != 0)
	{
		DbgError("Create event thread failed!\r\n");
		return ret;
	}

	DbgFuncExit();

	return 0;
}

