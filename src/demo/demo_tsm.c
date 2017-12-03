/****************************************Copyright (c)**************************************************                         
** File name:			demo_tsm.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-25
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

#include	"module/itop/itop_common.h"
#include	"module/itop/authorize/itop_authorize.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"

#include	"middleware/uart/fleety_uart.h"
#include	"middleware/event/fleety_event.h"

#define		DEBUG_Y
#include	"libs/debug.h"


int demo_tsm(void)
{
	DbgFuncEntry();
	
	itop_authorize_loop();
		
	fleety_event_init();
	fleety_uart_init();

	
	//sleep(5);

	//gb905_tsm_query_state();
	//gb905_tsm_get_driver_info();
		
	do{
		sleep(30);
		//gb905_tsm_get_driver_info();
		//gb905_tsm_query_state();
	}while(1);
		
	DbgFuncExit();

	return 0;
}  

