/****************************************Copyright (c)**************************************************                         
** File name:			demo_gb905_heartbeat.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-10-28
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

#define		DEBUG_Y
#include	"libs/debug.h"

int demo_gb905_heartbeat(void)  
{
	DbgFuncEntry();

	itop_authorize_loop();
	
	fleety_socket_init();

	while(1);

	DbgFuncExit();

	return 0;
}  
  

