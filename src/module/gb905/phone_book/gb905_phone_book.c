/****************************************Copyright (c)**************************************************                         
** File name:			gb905_phone_book.c
** Descriptions:		设置电话薄(0x8401)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2017-12-29
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

#include	"middleware/db/sqlite3/phone_book_sqlite3.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/phone_book/gb905_phone_book.h"

#define		DEBUG_Y
#include	"libs/debug.h"



// 电话薄设置的消息体格式
typedef struct{
    unsigned char number;						// 联系人总数
    int content;								// 电话薄起始字节
}__packed gb905_phone_book_t;


// 电话薄组成数据格式
typedef struct{
	unsigned char flag;								// 标志:1:呼入;2:呼出;3:呼入/呼出
	char number[PHONE_NUMBER_MAX_LEN];				// 电话号码
	char name[PNONE_NAME_MAX_LEN];					// 联系人
}__packed phone_book_list_t;


static phone_book_list_t phone_book_list[PHONE_MEMBER_MAX_NUMBER];



/**
* @brief 	平台设置电话薄的处理
* @param buf		设置电话薄消息体的数据地址
* @param len		设置电话薄消息体的数据长度
*
* @return  返回终端通用应答的状态
*/
unsigned char gb905_set_phone_book_treat(unsigned char *buf,int len)
{
    gb905_phone_book_t * phone_book;
    
    int i;
	int offset;
	char *point;

	DbgFuncEntry();

	phone_book = (gb905_phone_book_t *)buf;
	point = (char *)&phone_book->content;  // 指针指向电话薄内容

	// 如果联系人总数量为0，则删除现有所有联系人
    if(0 == phone_book->number)
	{
		memset(phone_book_list,0x00,sizeof(phone_book_list));

		// 删除终端数据库中的所有联系人	
		phone_book_sqlite3_delete_record();
	}
	else
    {	
    	//删除原来的联系人信息
    	phone_book_sqlite3_delete_record();
		
    	for(i = 0;i<phone_book->number;i++)
		{
    		memset(&phone_book_list[i],0x00,sizeof(phone_book_list_t));

			phone_book_list[i].flag = *point;
			point++;
			offset = strlen((char *)point) + 1;// 1:'/0'
            
    		if(offset > PHONE_NUMBER_MAX_LEN)
    		{
    			memcpy(phone_book_list[i].number,point,PHONE_NUMBER_MAX_LEN - 1);
    		}
    		else
    		{
    			memcpy(phone_book_list[i].number,point,offset);
    		}

			point += offset;

			offset = strlen((char *)point) + 1; // 1:'/0'
			if(offset > PNONE_NAME_MAX_LEN)
    		{
    			memcpy(phone_book_list[i].name,point,PNONE_NAME_MAX_LEN - 1);
    		}
    		else
    		{
    			memcpy(phone_book_list[i].name,point,offset);
    		}

			//插入联系人信息到数据库
			phone_book_sqlite3_insert_record((int)phone_book_list[i].flag,phone_book_list[i].number,phone_book_list[i].name);
			
    		point += offset;
    		
    	}

		
    }
	
	phone_book_sqlite3_restore_record();//for test
		
	//gb905_debug_phone_book();

	DbgFuncExit();

    return GB905_RESULT_OK;
}


/**
* @brief 	打印电话本的信息
*
*/
void gb905_debug_phone_book(void)
{
	int i = 0;

	DbgFuncEntry();
	
	for(i=0;i < PHONE_MEMBER_MAX_NUMBER;i++)
	{
		if(strlen(phone_book_list[i].number) == 0 || strlen(phone_book_list[i].name) == 0)
			break;
	
		DbgPrintf("phone flag 0x%02x \r\n",phone_book_list[i].flag);
		DbgPrintf("phone number %s \r\n",phone_book_list[i].number);
		DbgPrintf("phone name %s \r\n",phone_book_list[i].name);

	}

	DbgFuncExit();
} 

