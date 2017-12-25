/****************************************Copyright (c)**************************************************                         
** File name:			demo_ui.c
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

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_imei.h"


#include	"middleware/socket/fleety_socket.h"
#include	"middleware/event/fleety_event.h"

#define		DEBUG_Y
#include	"libs/debug.h"


int demo_ui(void)	
{
	DbgFuncEntry();

	ui_protocol_init();
	fleety_event_init();
	fleety_socket_init();
	
	sleep(5);
		
	do{
		ui_imei_info_req();
	
		sleep(60);
	}while(1);
		
	DbgFuncExit();

	return 0;
}  


