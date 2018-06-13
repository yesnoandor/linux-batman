/****************************************Copyright (c)**************************************************                         
** File name:			demo_toplight.c
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
#include	"module/gb905_peri/toplight/gb905_toplight.h"

#include	"middleware/info/toplight.h"
#include	"middleware/uart/fleety_uart.h"
#include	"middleware/event/fleety_event.h"

#define		DEBUG_Y
#include	"libs/debug.h"

int demo_toplight(void)	
{
	DbgFuncEntry();
	
	fleety_event_init();
	fleety_uart_init();

	sleep(5);
	
	do{
		//gb905_toplight_query_state();
		//sleep(5);
		//gb905_toplight_reset();

		set_toplight_operation_status(TOPLIGHT_STATUS_EMPTY);
		sleep(5);
		set_toplight_operation_status(TOPLIGHT_STATUS_LOADING);
		sleep(5);
		set_toplight_operation_status(TOPLIGHT_STATUS_STOP);
		sleep(5);
		set_toplight_operation_status(TOPLIGHT_STATUS_ORDER);
		sleep(5);
		set_toplight_operation_status(TOPLIGHT_STATUS_ALARM);
		sleep(5);
		set_toplight_operation_status(TOPLIGHT_STATUS_SECURITY);
		sleep(20);


		gb905_toplight_set_star_status(TOPLIGHT_STAR_NONE);
		sleep(5);
		gb905_toplight_set_star_status(TOPLIGHT_STAR_ONE);
		sleep(5);
		gb905_toplight_set_star_status(TOPLIGHT_STAR_TWO);
		sleep(5);
		gb905_toplight_set_star_status(TOPLIGHT_STAR_THREE);
		sleep(5);
		gb905_toplight_set_star_status(TOPLIGHT_STAR_FOUR);
		sleep(5);
		gb905_toplight_set_star_status(TOPLIGHT_STAR_FIVE);

		sleep(60);
	}while(1);
	
	DbgFuncExit();

	return 0;
}  
	  

