/****************************************Copyright (c)**************************************************                         
** File name:			device_info.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-10-30
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

#include	"misc/check.h"

#include	"module/info/info2file.h"
#include	"module/info/device_info.h"


//#define		DEBUG_Y
#include	"libs/debug.h"

#define		DEVICE_INFO_FILE			"info.sys"
#define		DEVICE_INFO_BACKUP_FILE		"info.bak"

static device_info_t device_info;

void init_device_info(void)
{
	int ret;
	int state = 0;
	device_info_t info,info_buckup;
	short crc;

	DbgFuncEntry();

	ret = file2info(DEVICE_INFO_FILE,(unsigned char*)&info,sizeof(device_info_t));
	if(ret)
	{
		crc = crc16_compute((unsigned char *)&info,sizeof(device_info_t)-2,NULL);
		if(info.crc != crc)
		{
			memset(&info,0x00,sizeof(device_info_t));
			state &= ~0x01;
		}
		else
		{
			state |= 0x01;
		}
	}
	
	ret = file2info(DEVICE_INFO_BACKUP_FILE,(unsigned char *)&info,sizeof(device_info_t));
	if(ret)
	{
		crc = crc16_compute((unsigned char *)&info,sizeof(device_info_t)-2,NULL);
		if(info.crc != crc)
		{
			memset(&info,0x00,sizeof(device_info_t));
			state &= ~0x02;
		}
		else
		{
			state |= 0x02;
		}
	}

	DbgPrintf("state = 0x%x\r\n",state);

	switch (state)
	{
		case 0:	// 两个文件都不正确
			memset(&device_info,0x00,sizeof(device_info_t));
			info2file(DEVICE_INFO_FILE,(unsigned char *)&device_info,sizeof(device_info_t));
			info2file(DEVICE_INFO_BACKUP_FILE,(unsigned char *)&device_info,sizeof(device_info_t));
			break;
			
		case 1:	// 备份文件正确
			memcpy((void *)(&device_info),(void *)&info_buckup,sizeof(device_info_t));
			info2file(DEVICE_INFO_FILE,(unsigned char *)&device_info,sizeof(device_info_t));
			break;

		case 2:	// Info  文件正确
			memcpy((void *)&device_info,(void *)&info,sizeof(device_info_t));
			info2file(DEVICE_INFO_BACKUP_FILE,(unsigned char *)&device_info,sizeof(device_info_t));
			break;
			
		case 3:	// 两个文件不正确
			memcpy((void *)&device_info,(void *)&info,sizeof(device_info_t));
			break;

		default:
			DbgError("device info error!\r\n");
			break;
	}

	DbgFuncExit();
}

void get_device_info(device_info_t * info)
{
	DbgFuncEntry();
	
	memcpy(info,&device_info,sizeof(device_info_t));

	DbgPrintf("mdt_id = %d\n",info->mtd_id);
	DbgPrintf("imei_id = %s\n",info->imei_id);
	DbgPrintf("imsi_id = %s\n",info->imsi_id);
	DbgPrintf("crc = 0x%x\r\n",info->crc);

	DbgFuncExit();
}

void set_device_info(device_info_t * info)
{
	DbgFuncEntry();
	
	memcpy(&device_info,info,sizeof(device_info_t));
	device_info.crc = crc16_compute((unsigned char *)info,sizeof(device_info_t)-2,NULL);
	
	DbgPrintf("mdt_id = %d\n",device_info.mtd_id);
	DbgPrintf("imei_id = %s\n",device_info.imei_id);
	DbgPrintf("imsi_id = %s\n",device_info.imsi_id);
	DbgPrintf("crc = 0x%x\r\n",device_info.crc);

	info2file(DEVICE_INFO_FILE,(unsigned char *)&device_info,sizeof(device_info_t));
	info2file(DEVICE_INFO_BACKUP_FILE,(unsigned char *)&device_info,sizeof(device_info_t));

	DbgFuncExit();
}


