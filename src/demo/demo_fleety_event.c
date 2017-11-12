/****************************************Copyright (c)**************************************************                         
** File name:			demo_event.c
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

#include	"module/itop/authorize/itop_authorize.h"

#include	"middleware/socket/fleety_socket.h"
#include	"middleware/event/fleety_event.h"

#define		DEBUG_Y
#include	"libs/debug.h"

int demo_fleety_event(void)  
{
	DbgFuncEntry();

	itop_authorize_loop();

	fleety_event_init();
	fleety_socket_init();

	DbgFuncExit();

	while(1)
	{
		fleety_event_t event;

		sleep(15);
		event.id = SIMU_EVENT;
		event.param = 0;
		event.priority = DAFAULT_PRIORITY;
		fleety_event_push(&event);
	}
	
	return 0;
}  
  

