/****************************************Copyright (c)**************************************************                         
** File name:			demo_notify.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-02-25
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

#include	"libs/notify.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static	pthread_t	notify_thread_id;

static notify_object_t * notify = NULL;

static void * demo_notify_loop_func(void *arg)  
{	
	DbgFuncEntry();

	sleep(5);

	notify_stop(notify);
	
	DbgFuncExit();
	
    return ((void *)0);
} 


int demo_notify(void)	
{
	int ret;
	
	DbgFuncEntry();

	notify = (notify_object_t *)notify_init();

	ret = pthread_create(&notify_thread_id, NULL, demo_notify_loop_func, NULL);
    if(ret != 0)
    {
    	DbgError("create demo notify thread error: %s\r\n",strerror(errno));  
		return -1;
	}

	notify_wait(notify);
	
	DbgFuncExit();

	return 0;
}  


