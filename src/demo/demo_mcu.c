/****************************************Copyright (c)**************************************************                         
** File name:			demo_mcu.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-03
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

#include	"module/gb905_ex/mcu/mcu_common.h"

#include	"middleware/uart/fleety_uart.h"
#include	"middleware/event/fleety_event.h"

#define		DEBUG_Y
#include	"libs/debug.h"


int demo_mcu(void)	
{
	DbgFuncEntry();

	mcu_protocol_init();
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


