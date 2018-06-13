/****************************************Copyright (c)**************************************************                         
** File name:			phone_book.c
** Descriptions:		记录电话薄信息
*
*
*
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-12
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

#include	"module/gb905/phone_book/gb905_phone_book.h"

#include	"middleware/info/phone_book.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static save_phone_book_t phone_book_now;

//设置电话薄信息
void phone_book_set_info(save_phone_book_t *phone_book_info)
{
	DbgFuncEntry();
	
	memset(&phone_book_now,0x00,sizeof(save_phone_book_t));
	memcpy(&phone_book_now,phone_book_info,sizeof(save_phone_book_t));

	DbgFuncExit();
}

//获取电话薄信息
void phone_book_get_info(save_phone_book_t *phone_book_info)
{
	DbgFuncEntry();
	
	memset(phone_book_info,0x00,sizeof(save_phone_book_t));
	memcpy(phone_book_info,&phone_book_now,sizeof(save_phone_book_t));

	DbgFuncExit();
}

//打印电话薄信息
void phone_book_info_debug(void)
{
	int i;
	save_phone_book_t phone_book_info;
	DbgFuncEntry();

	phone_book_get_info(&phone_book_info);
	for(i=0;i<phone_book_info.num;i++)
	{
		DbgPrintf("phone flag 0x%02x \r\n",phone_book_info.phone_book_msg[i].flag);
		DbgPrintf("phone number %s \r\n",phone_book_info.phone_book_msg[i].phone_num);
		DbgPrintf("phone name %s \r\n",phone_book_info.phone_book_msg[i].name);
	}

	DbgFuncExit();
}
