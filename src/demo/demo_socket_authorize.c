/****************************************Copyright (c)**************************************************                         
** File name:			demo_socket_authorize.c
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

#define		DEBUG_Y
#include	"libs/debug.h"

int demo_authorize_socket(void)  
{
	DbgFuncEntry();

	itop_authorize_loop();

	DbgFuncExit();

	return 0;
}  
  

