/****************************************Copyright (c)**************************************************                         
** File name:			fleety_event.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-10
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

#include	"middleware/socket/fleety_socket.h"
#include	"middleware/event/fleety_event.h"

#include	"libs/fifo.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define		MAX_EVENT_SIZE				256
#define		MAX_EVENT_LOOP_TIMEOUT		5


static 	fleety_event_t	event_queue[MAX_PRIORITY][MAX_EVENT_SIZE];
static	fifo_t event_fifo[MAX_PRIORITY];

void fleety_event_init(void)
{
	int i;
	
	DbgFuncEntry();

	for(i=0;i<MAX_PRIORITY;i++)
	{
		fifo_init(&event_fifo[i],event_queue,sizeof(fleety_event_t),MAX_EVENT_SIZE);	
	}
	
	DbgFuncExit();
}

void fleety_event_push(fleety_event_t * event)
{
	if(event->priority >= MAX_PRIORITY){
		DbgWarn("priority(%d) error,set default priority!\r\n",event->priority);
		event->priority = DAFAULT_PRIORITY;
	}
	
	fifo_in(&event_fifo[event->priority],(void *)event);
	fleety_pipe_send();
}

bool fleety_event_fetch(fleety_event_t * event)
{
	int i;
	bool ret = false;

	for(i=0;i<MAX_PRIORITY;i++)
	{
		if(fifo_status(&event_fifo[i]) != FIFO_EMPTY)
		{
			fifo_out(&event_fifo[i],(void *)event);
			ret = true;
			break;
		}				
	}

	return ret;
}

void fleety_event_treat(void)  
{
	fleety_event_t event;
	bool ret = false;

	DbgFuncEntry();

	do{
		ret = fleety_event_fetch(&event);

		if(ret)
		{
			DbgPrintf("event id = %d\r\n",event.id);
			DbgPrintf("event param = 0x%x\r\n",event.param);
			DbgPrintf("event priority = %d\r\n",event.priority);
			
			switch (event.id)
			{
				case SIMU_EVENT:
					//fleety_simu_treat(event.event_param);
					break;
					
				case LOADING_EVENT:
					//event_pro_init->loading_treat(event.event_param);
					break;
					
				case LOGINING_EVENT:
					//fleety_logining_treat(event.event_param);
					break;
					
				case EVAL_EVENT:
					//set_eval_state(event.event_param);
					//		event_pro_init->eval_treat();
					//		gb905_take_picture(TAKE_PIC_REASON_EVALUTION);
					break;
					
				case ACC_EVENT:
					//fleety_acc_treat(event.event_param);
					break;
								
				case ALARM_EVENT:
					//fleety_alarm_treat(event.event_param);
					break;
							
				case EXIT_EVENT:
					//exit = 1;
					break;
						
				case EXPORT_EVENT:
					//fleety_export_treat(event.event_param);
					break;
							
				case UDEV_UDISK_EVENT:
					//fleety_udev_treat(event.event_param);
					break;
							
				default:
					DbgWarn("invaild event_id = %d\r\n",event.id);
					break;
			}	
		}
	}while(ret);
	
	DbgFuncExit();
}
