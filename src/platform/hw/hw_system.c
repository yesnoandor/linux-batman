/****************************************Copyright (c)**************************************************                         
** File name:			hw_system.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-28
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

#include	"platform/hw/hw_system.h"

#define		DEBUG_Y
#include	"libs/debug.h"

/*
*  @brief 		在hi3520D    平台上系统关机
* 
*/
void hw_system_power_off(void)
{
	DbgFuncEntry();

	DbgFuncExit();
}

/*
*  @brief 		在hi3520D    平台上系统重启
* 
*/
void hw_system_reset(void)
{
	DbgFuncEntry();

    system("reboot");
    
	DbgFuncExit();
}

/*
*  @brief 		在hi3520D    平台上系统恢复出厂设置
* 
*/
void hw_system_recovery(void)
{
	DbgFuncEntry();

	DbgFuncExit();
}

/*
*  @brief 		在hi3520D    平台上系统加载U    盘
* 
*/
void hw_system_mount_disk(void)
{
	char cmd[MAX_CMD_CHARS_SIZE];
	
	DbgFuncEntry();

	//snprintf(cmd,80,"mount -t vfat /dev/disk1 /mnt/udisk");
	snprintf(cmd,MAX_CMD_CHARS_SIZE,"mount -o -rw -t vfat /dev/sdc1 /mnt/udisk");
	DbgPrintf("cmd = %s\r\n",cmd);

	system(cmd);			// popen()/fgets()

	DbgFuncExit();
}

