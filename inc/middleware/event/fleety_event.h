/****************************************Copyright (c)**************************************************                         
** File name:			fleety_event.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-11-10
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _FLEETY_EVENT_H
#define	_FLEETY_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif


enum{
	HIGH_PRIORITY = 0,
	DAFAULT_PRIORITY,
	MAX_PRIORITY
};

enum{
	IN_CHANNEL_EVENT = 0,
	
	LOADING_EVENT,
	LOGINING_EVENT,
	ACC_EVENT,
	ALARM_EVENT,
	EVAL_EVENT,
	
	UART_RECV_EVENT,
	UART_EXIT_EVENT,
	SOCKET_RECV_EVENT,

	UPDATE_SOCKET_EXIT_EVENT,
	UPDATE_SOCKET_INIT_EVENT,
	UPDATE_SOCKET_RECV_EVENT,

	UDEV_UDISK_EVENT,

	SIMU_EVENT,
	EXIT_EVENT,
	
	EXPORT_EVENT
};


typedef struct {
	int id;
	int param;
	int priority;
}fleety_event_t;

void fleety_event_init(void);
void fleety_event_push(fleety_event_t * event);
void fleety_event_treat(void);


#ifdef __cplusplus
}
#endif

#endif

