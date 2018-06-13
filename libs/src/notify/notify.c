/****************************************Copyright (c)**************************************************                         
** File name:			notify.c
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

#include	<stdio.h>
#include	<string.h>
#include	<stdbool.h>
#include	<stdlib.h>

#include 	<pthread.h>

#include	"notify.h"




static void * notify_loop_func(void *arg)
{
	notify_object_t *notify;

	notify = (notify_object_t *)arg;

	pthread_mutex_lock(&notify->thread_lock);
	pthread_cond_wait(&notify->thread_cond,&notify->thread_lock);
	pthread_mutex_unlock(&notify->thread_lock);
	
    return ((void *)0);
}  



/** 
* @brief 	初始化异步通知
*
*/
void * notify_init(void)
{
	int err;

	notify_object_t *notify = (notify_object_t *)malloc(sizeof(notify_object_t)) ; 
		
	pthread_mutex_init(&notify->thread_lock, NULL);
	pthread_cond_init(&notify->thread_cond, NULL);

	err = pthread_create(&notify->thread_id, NULL, notify_loop_func, notify);
    if(err != 0)
    {
    	DbgError("create notify thread error: %s\r\n",strerror(err));  
		return (void *)-1;
	}

	return (void *)notify;
}

/*
* @brief 	异步等待
* 
*/
void notify_wait(notify_object_t *notify)
{
	pthread_join(notify->thread_id,NULL);

	free(notify);
}


/*
* @brief 	异步通知完成
* 
*/
void notify_stop(notify_object_t *notify)
{	
	pthread_cond_signal(&notify->thread_cond);
}

