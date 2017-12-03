/****************************************Copyright (c)**************************************************                         
** File name:			ubuntu_relay.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-24
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


#define		DEBUG_Y
#include	"libs/debug.h"

void ubuntu_relay_simu_output(int index,int level)
{
	DbgFuncEntry();

	DbgGood("Out Channal %d,Level = %d\r\n",index,level);

	DbgFuncExit();
}

