/****************************************Copyright (c)**************************************************                         
** File name:			demo_libevent_uart.c
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

#include	"middleware/uart/fleety_uart.h"

#define		DEBUG_Y
#include	"libs/debug.h"

int demo_libevent_uart(void)  
{
	int i;
	unsigned char buff[1024];
	
	DbgFuncEntry();

	memset(buff,0,sizeof(buff));
	for(i=0;i<20;i++)
		buff[i] = 3*i+1;
	
	fleety_uart_init();

	while(1)
	{
		sleep(10);
		
		fleety_uart_send(0,buff,10);
	}

	DbgFuncExit();

	return 0;
}  
  

