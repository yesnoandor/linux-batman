/****************************************Copyright (c)**************************************************                         
** File name:			demo_gps_nmea.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-23
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

#include	"module/gps/gps_nmea.h"

#include	"middleware/uart/fleety_uart.h"
#include	"middleware/event/fleety_event.h"
#include	"middleware/socket/fleety_socket.h"

#define		DEBUG_Y
#include	"libs/debug.h"


int demo_gps_nmea(void)	
{
	DbgFuncEntry();
	
	//itop_authorize_loop();

	//fleety_event_init();
	//fleety_socket_init();

	gps_nmea_init();
	
	fleety_uart_init();
	
	DbgFuncExit();

	while(1);
	
	return 0;
}  
	 

