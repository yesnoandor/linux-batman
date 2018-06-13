/****************************************Copyright (c)**************************************************                         
** File name:			gb905_update_meter.c
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

#include	"libs/x_modem.h"

#include	"misc/check.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/ctrl/gb905_control.h"
#include	"module/gb905_update/gb905_update_common.h"

#include	"middleware/info/update.h"
#include	"middleware/db/file/db_file_path.h"
#include	"middleware/db/file/db_file_bin.h"
#include	"middleware/uart/xmodem_uart.h"



#define		DEBUG_Y
#include	"libs/debug.h"


#define		GB905_UPDATE_TSM_DEVICE_TYPE				0x02
#define		GB905_UPDATE_TSM_VENDOR_ID					0x06
#define		GB905_UPDATE_TSM_HW_VERSION					0X01
#define		GB905_UPDATE_TSM_SW_VERSION					0X0104

bool gb905_update_tsm_version_check(void)
{
	bool ret = true;
	int sw_version;
	gb905_update_info_t update_info;
	
	DbgFuncEntry();
	
	get_update_info((unsigned char *)&update_info);
	
	if(update_info.base_info.vendor_id != GB905_UPDATE_TSM_VENDOR_ID)
	{
		set_update_result(GB905_UPDATE_VENDOR_MISMATCH);
		DbgWarn("verdor mismatch !!!\r\n");
		return false;
	}

	if(update_info.base_info.vendor_id != GB905_UPDATE_TSM_HW_VERSION)
	{
		set_update_result(GB905_UPDATE_HW_MISMATCH);
		DbgWarn("hw version mismatch !!!\r\n");
		return false;
	}

	sw_version = update_info.sw_version[0] << 8 | update_info.sw_version[1];
	if(sw_version <= GB905_UPDATE_TSM_SW_VERSION)
	{
		set_update_result(GB905_UPDATE_NONE);
		DbgWarn("sw version mismatch !!!\r\n");
		return false;
	}
	
	DbgFuncExit();

	return ret;
}

void gb905_update_tsm_start(void)
{
	bool ret;
	struct stat file_stat;
	int size;
	unsigned char * buf;
	int result;
	char path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	build_db_path(path,UPDATE_TSM_FILE);
	ret = is_file_exist(path);
	if(!ret)
	{
		DbgWarn("update file is not exist!!\r\n");
		return;
	}

	if(stat(path,&file_stat) < 0)
	{
		DbgError("get update file size failed!! \r\n");
		return;
	}
		
	size = (int)file_stat.st_size;
	buf = malloc(size);

	ret = file2buf(path,0,buf,size);
	if(!ret)
	{
		DbgError("get file content failed!! \r\n");
		return;
	}

	xmodem_open();
	result = xmodemTransmit(buf,size);		//start xmodem protocol
	DbgWarn("\r\n xmodemTransmit result =%d \r\n",result);
	xmodem_close();

	free(buf);

	DbgFuncExit();	
}

