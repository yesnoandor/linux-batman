/****************************************Copyright (c)**************************************************                         
** File name:			notify.h
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

#ifndef __NOTIFY_H
#define __NOTIFY_H

#ifdef __cplusplus
extern "C" {
#endif

/*
* √Ë ˆ“Ï≤Ω∂‘œÛ
*/
typedef struct
{
	pthread_t	thread_id;
	pthread_mutex_t thread_lock;
	pthread_cond_t thread_cond;
}notify_object_t;

void * notify_init(void);
void notify_wait(notify_object_t *notify);
void notify_stop(notify_object_t *notify);


#ifdef __cplusplus
}
#endif



#endif

