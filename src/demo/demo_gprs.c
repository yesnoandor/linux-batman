/****************************************Copyright (c)**************************************************                         
** File name:			demo_gprs.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-06
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

#include	"module/gprs/gprs_at_cmd.h"

#include	"middleware/info/imei.h"
#include	"middleware/uart/fleety_uart.h"
#include	"middleware/event/fleety_event.h"


#define		DEBUG_Y
#include	"libs/debug.h"

void test_imei(void)
{	
	char imei[16];

	DbgFuncEntry();

	gprs_at_protocol_init();

	fleety_event_init();
	fleety_uart_init();
	
	do{
		sleep(10);
		gprs_get_imei();

		sleep(2);
		get_imei(imei);

		DbgPrintf("imei = %s\r\n",imei);
	}while(1);

	sleep(10);
	get_imei(imei);

	DbgPrintf("imei = %s\r\n",imei);

	DbgFuncExit();
}

int demo_gprs(void)	
{
	test_imei();
	while(1);
	
	return 0;
}

