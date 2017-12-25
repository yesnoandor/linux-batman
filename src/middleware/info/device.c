/****************************************Copyright (c)**************************************************                         
** File name:			device.c
** Descriptions:		获取设备信息
**		1. mtd 
**		2. imei
**		3. imsi
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

#include	"middleware/db/file/db_file_bin.h"


#include	"middleware/info/device.h"


//#define		DEBUG_Y
#include	"libs/debug.h"

#define		DEVICE_INFO_FILE			"info.sys"
#define		DEVICE_INFO_BACKUP_FILE		"info.bak"

static device_info_t device_info;

/** 
* @brief 	从文件中获取设备信息数据
* 
*/
void init_device_info(void)
{
	int ret;
	int state = 0;
	device_info_t info,info_buckup;
	short crc;

	DbgFuncEntry();

	ret = file2buf(DEVICE_INFO_FILE,(unsigned char*)&info,sizeof(device_info_t));
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
	
	ret = file2buf(DEVICE_INFO_BACKUP_FILE,(unsigned char *)&info,sizeof(device_info_t));
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
			buf2file(DEVICE_INFO_FILE,(unsigned char *)&device_info,sizeof(device_info_t));
			buf2file(DEVICE_INFO_BACKUP_FILE,(unsigned char *)&device_info,sizeof(device_info_t));
			break;
			
		case 1:	// 备份文件正确
			memcpy((void *)(&device_info),(void *)&info_buckup,sizeof(device_info_t));
			buf2file(DEVICE_INFO_FILE,(unsigned char *)&device_info,sizeof(device_info_t));
			break;

		case 2:	// 主文件正确
			memcpy((void *)&device_info,(void *)&info,sizeof(device_info_t));
			buf2file(DEVICE_INFO_BACKUP_FILE,(unsigned char *)&device_info,sizeof(device_info_t));
			break;
			
		case 3:	// 两个文件都正确
			memcpy((void *)&device_info,(void *)&info,sizeof(device_info_t));
			break;

		default:
			DbgError("device info error!\r\n");
			break;
	}

	DbgFuncExit();
}

/** 
* @brief 	获取设备信息
* @param info		保存设备信息的数据结构
*
*/
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

/** 
* @brief 	设置设备信息,   并保存到文件
* @param info		保存设备信息的数据结构
*
*/
void set_device_info(device_info_t * info)
{
	DbgFuncEntry();
	
	memcpy(&device_info,info,sizeof(device_info_t));
	device_info.crc = crc16_compute((unsigned char *)info,sizeof(device_info_t)-2,NULL);
	
	DbgPrintf("mdt_id = %d\n",device_info.mtd_id);
	DbgPrintf("imei_id = %s\n",device_info.imei_id);
	DbgPrintf("imsi_id = %s\n",device_info.imsi_id);
	DbgPrintf("crc = 0x%x\r\n",device_info.crc);

	buf2file(DEVICE_INFO_FILE,(unsigned char *)&device_info,sizeof(device_info_t));
	buf2file(DEVICE_INFO_BACKUP_FILE,(unsigned char *)&device_info,sizeof(device_info_t));

	DbgFuncExit();
}


