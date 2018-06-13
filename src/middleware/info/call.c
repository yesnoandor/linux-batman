/****************************************Copyright (c)**************************************************                         
** File name:			call.c
** Descriptions:		通话信息保存
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-22
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

#include	"misc/misc.h"

#include	"middleware/db/file/db_file_xml.h"
#include	"middleware/db/file/db_file_path.h"

#include	"module/gb905/phone_book/gb905_phone_book.h"

#include	"middleware/info/call.h"


#define		DEBUG_Y
#include	"libs/debug.h"

static clcc_info_t clcc_info;

static void debug_clcc_info(void)
{
	DbgFuncEntry();
	
	DbgWarn("idx = %d\r\n",clcc_info.idx);
	DbgWarn("dir = %d\r\n",clcc_info.dir);
	DbgWarn("stat = %d\r\n",clcc_info.stat);

	DbgWarn("mode = %d\r\n",clcc_info.mode);
	DbgWarn("mpty = %d\r\n",clcc_info.mpty);
	DbgWarn("phone = %s\r\n",clcc_info.phone);

	DbgFuncExit();
}
void set_clcc_info(clcc_info_t *info)
{
	DbgFuncEntry();

	memset(&clcc_info,0x00,sizeof(clcc_info_t));
	memcpy(&clcc_info,info,sizeof(clcc_info_t));

	debug_clcc_info();
	
	DbgFuncExit();
}

void get_clcc_info(clcc_info_t *info)
{

	DbgFuncEntry();

	memcpy(info,&clcc_info,sizeof(clcc_info_t));

	DbgFuncExit();
}


static call_info_t call_info;

static unsigned char *call_info_point[] =
{
	(unsigned char *)&call_info.start_time,
	(unsigned char *)&call_info.total_time,
	(unsigned char *)&call_info.status,
		
};

static char *call_info_type[] =
{
	"DWORD",
	"DWORD",
	"DWORD",
};

void call_info_init(void)
{
	bool ret;
	char path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	build_db_path(path,CALL_INFO_FILE);
	ret = xml2data(path,call_info_point);
	if(!ret)
	{
		memset(&call_info,0x00,sizeof(call_info_t));

		DbgPrintf("list size = %d\r\n",ARRAY_SIZE(call_info_point));

		build_db_path(path,CALL_INFO_FILE);
		init_xml(path,call_info_point,call_info_type,ARRAY_SIZE(call_info_point));
	}
	
	DbgFuncExit();
}

void set_call_info(void)
{
	char path[MAX_PATH_CHARS_SIZE];
	
	DbgFuncEntry();
	
	//将通话信息写入文件
	build_db_path(path,CALL_INFO_FILE);
	data2xml(path,call_info_point);
	
	DbgFuncExit();
}

call_info_t * get_call_info(void)
{
	return &call_info;
}
