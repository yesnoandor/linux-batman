/****************************************Copyright (c)**************************************************                         
** File name:			demo_gb905_report.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-11
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
	
#include	"middleware/uart/fleety_uart.h"
#include	"middleware/event/fleety_event.h"
#include	"middleware/socket/fleety_socket.h"

#include	"main/fleety_report.h"

#define		DEBUG_Y
#include	"libs/debug.h"
	
int demo_gb905_report(void)	
{
	DbgFuncEntry();
	
	itop_authorize_loop();

	fleety_event_init();
	fleety_socket_init();
	fleety_uart_init();

	fleety_report_init();
	
	DbgFuncExit();

	while(1);
	
	return 0;
}  
	  

