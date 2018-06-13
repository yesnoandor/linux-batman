/****************************************Copyright (c)**************************************************                         
** File name:			fleety_update.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-20
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

#define		DEBUG_Y
#include	"libs/debug.h"

void fleety_update_peripheral_treat(unsigned char device_type)
{
	DbgFuncEntry();
		
	switch(device_type)
	{
		case GB905_DEVICE_MCU:
			//clear_update_info();
			//clear_update_socket_info();
			break;

		case GB905_DEVICE_METER:
			//update_meter_test();
			//gb905_send_update_cmd_to_meter();
			break;

		case GB905_DEVICE_TOPLIGHT:
			break;
			
		default:
			DbgWarn("don't support this device type(%d)\r\n",device_type);
			break;
		
	}
	
	DbgFuncExit();
}



