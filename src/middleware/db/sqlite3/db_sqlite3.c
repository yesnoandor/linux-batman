/****************************************Copyright (c)**************************************************                         
** File name:			db_sqlite3_init.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-09
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

#include	"libs/sqlite3/sqlite3.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/av/gb905_photo.h"

#include	"middleware/event/export/fleety_export.h"

#include	"middleware/db/sqlite3/db_sqlite3_path.h"
#include	"middleware/db/sqlite3/db_sqlite3.h"
#include	"middleware/db/sqlite3/event_sqlite3.h"
#include	"middleware/db/sqlite3/report_sqlite3.h"
#include	"middleware/db/sqlite3/notice_sqlite3.h"
#include	"middleware/db/sqlite3/phone_book_sqlite3.h"
#include	"middleware/db/sqlite3/operation_sqlite3.h"
#include	"middleware/db/sqlite3/order_sqlite3.h"
#include	"middleware/db/sqlite3/photo_sqlite3.h"

#define		DEBUG_Y
#include	"libs/debug.h"


int db_sqlite3_init(void)
{
	int  ret = 0;
	sqlite3 * db = 0;
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}
	else
	{
		DbgGood("sqlite3 db connect success��\r\n");
	}
	
	//�绰�����ݿ��ʼ��
	ret = phone_book_sqlite3_init(db);
	if(ret != SQLITE_OK)
	{
		DbgError("phone book db init failed!\r\n");
		
		 //�ر����ݿ�
	    sqlite3_close(db);
    	db = 0;

		return ret;
	}

	//�¼����ݿ��ʼ��
	ret = event_sqlite3_init(db);
	if(ret != SQLITE_OK)
	{
		DbgError("event db init failed!\r\n");
		
		 //�ر����ݿ�
	    sqlite3_close(db);
    	db = 0;

		return ret;
	}
	
	//�ı���Ϣ���ݿ��ʼ��
	ret = notice_sqlite3_init(db);
	if(ret != SQLITE_OK)
	{
		DbgError("notice db init failed!\r\n");
		
		 //�ر����ݿ�
	    sqlite3_close(db);
    	db = 0;

		return ret;
	}

	//Ӫ��������Ϣ���ݿ��ʼ��
	ret = operation_sqlite3_init(db);
	if(ret != SQLITE_OK)
	{
		DbgError("operation db init failed!\r\n");
		
		 //�ر����ݿ�
	    sqlite3_close(db);
    	db = 0;

		return ret;
	}
	
	//��ʷ������Ϣ���ݿ��ʼ��
	ret = order_sqlite3_init(db);
	if(ret != SQLITE_OK)
	{
		DbgError("order db init failed!\r\n");
		
		 //�ر����ݿ�
	    sqlite3_close(db);
    	db = 0;

		return ret;
	}
	
    //��Ƭ���ݿ��ʼ��
	ret = photo_sqlite3_init(db);
	if(ret != SQLITE_OK)
	{
		DbgError("photo db init failed!\r\n");
		
		 //�ر����ݿ�
	    sqlite3_close(db);
    	db = 0;

		return ret;
	}
    
	#if 0
	ret = audio_db_init(db);
	if(ret != SQLITE_OK)
	{
		DbgError("recorded db init failed!\r\n");
		
		 //�ر����ݿ�
	    sqlite3_close(db);
    	db = 0;

		return ret;
	}


	#endif	

	//λ�û㱨���ݿ��ʼ��
	ret = report_sqlite3_init(db);
	if(ret != SQLITE_OK)
	{
		DbgError("report db init failed!\r\n");
	}


    //�ر����ݿ�
    sqlite3_close(db);
    db = 0;

	DbgFuncExit();

	return ret;
}

