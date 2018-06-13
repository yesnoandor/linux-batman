/****************************************Copyright (c)**************************************************                         
** File name:			gb905_update_isu.c
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

#include	"misc/bcd.h"
#include	"misc/check.h"
#include	"misc/endian.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/ctrl/gb905_control.h"
#include	"module/gb905_update/gb905_update_common.h"

#include	"middleware/info/device.h"
#include	"middleware/info/update.h"
#include	"middleware/socket/update_socket.h"

#define		DEBUG_Y
#include	"libs/debug.h"


#define		GB905_UPDATE_ISU_VENDOR_ID				0x06
#define		GB905_UPDATE_ISU_HW_VERSION				0X01
#define		GB905_UPDATE_ISU_SW_VERSION				0X0104

bool gb905_update_isu_version_check(void)
{
	bool ret = true;
	int sw_version;
	gb905_update_info_t update_info;
	
	DbgFuncEntry();
	
	get_update_info((unsigned char *)&update_info);
	
	if(update_info.base_info.vendor_id != GB905_UPDATE_ISU_VENDOR_ID)
	{
		set_update_result(GB905_UPDATE_VENDOR_MISMATCH);
		DbgWarn("verdor mismatch !!!\r\n");
		return false;
	}

	if(update_info.base_info.vendor_id != GB905_UPDATE_ISU_HW_VERSION)
	{
		set_update_result(GB905_UPDATE_HW_MISMATCH);
		DbgWarn("hw version mismatch !!!\r\n");
		return false;
	}

	sw_version = update_info.sw_version[0] << 8 | update_info.sw_version[1];
	if(sw_version <= GB905_UPDATE_ISU_SW_VERSION)
	{
		set_update_result(GB905_UPDATE_NONE);
		DbgWarn("sw version mismatch !!!\r\n");
		return false;
	}
	
	DbgFuncExit();

	return ret;
}


