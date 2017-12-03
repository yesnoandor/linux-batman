/****************************************Copyright (c)**************************************************                         
** File name:			demo_meter.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-18
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

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/meter/gb905_meter.h"

#include	"middleware/uart/fleety_uart.h"
#include	"middleware/event/fleety_event.h"

#define		DEBUG_Y
#include	"libs/debug.h"

int demo_meter(void)	
{
	DbgFuncEntry();
	
	fleety_event_init();
	fleety_uart_init();
	
	sleep(5);
		
	do{
		//gb905_meter_query_state();
	
		sleep(60);
	}while(1);
		
	DbgFuncExit();

	return 0;
}  
	  

