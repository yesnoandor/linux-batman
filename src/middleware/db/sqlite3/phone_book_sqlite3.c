/****************************************Copyright (c)**************************************************                         
** File name:			phone_book_sqlite3.c
** Descriptions:		�绰����Ϣ���ݿ⹦��
*
*				�绰����Ϣ���ݿ���ֶ�����:
*
* 				@id				����ID(���ݿ��Զ�����)
* 				@flag			�绰��־
* 				@phonenumber	�绰����
* 				@name			��ϵ��
*
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-09
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

#include	"misc/endian.h"

#include	"libs/sqlite3/sqlite3.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/phone_book/gb905_phone_book.h"

#include	"middleware/info/phone_book.h"

#include	"middleware/db/sqlite3/db_sqlite3_path.h"
#include	"middleware/db/sqlite3/db_sqlite3.h"
#include	"middleware/db/sqlite3/phone_book_sqlite3.h"



//#define		DEBUG_Y
#include	"libs/debug.h"


#define		SQL_PHONEBOOK_TABLE			"phonebook_table"
static 		pthread_mutex_t				phone_book_sqlite3_mutex;

//�绰�����ݿ��ʼ��
int phone_book_sqlite3_init(sqlite3 * db)
{
	int  ret = 0;
	char * err_msg = NULL;
	char sql[SQL_STR_MAX_LEN];

	DbgFuncEntry();
 	
	// �������ݿ��
	snprintf(sql,SQL_STR_MAX_LEN,"create table if not exists %s(id INTEGER PRIMARY KEY AUTOINCREMENT,flag int,phonenumber char(20),name char(10));",SQL_PHONEBOOK_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	// ִ�н���
	ret = sqlite3_exec(db,sql,0,0,&err_msg);
    if(ret != SQLITE_OK)
    {
        DbgError("SQL create table error:%s\n",err_msg);
        sqlite3_free(err_msg);
    }
    pthread_mutex_init(&phone_book_sqlite3_mutex, NULL);
	
	DbgFuncExit();
	
    return ret;
}

/*
* @brief 		  		�����ݿ����һ���绰����Ϣ
* @param flag  		��־	   
* @param phone_num	�绰�������ݵ�ַ
* @param name		��ϵ�����ݵ�ַ
*
* return 	�жϽ��
*/
int phone_book_sqlite3_insert_record(int flag,char * phone_num,char *name)
{
	int ret = 0;
	sqlite3 * db = 0;
	sqlite3_stmt * stmt;			// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&phone_book_sqlite3_mutex);	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{		
		pthread_mutex_unlock(&phone_book_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}

	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"insert into %s (id,flag,phonenumber,name) values (?,?,?,?);",SQL_PHONEBOOK_TABLE);
	DbgPrintf("sql = %s\r\n",sql);


	// ׼��SQL Ԥ�������
	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret == SQLITE_OK)
	{
		// ��Ԥ��������еĵ�1,2,3����������sqlite3_bind_xxx ��1��ʼ��
		// sqlite3_bind_int(stmt,1, id);
		sqlite3_bind_int(stmt,2, flag);
		sqlite3_bind_text(stmt,3, (const char*)phone_num,-1,NULL);
		sqlite3_bind_text(stmt,4, (const char*)name,-1,NULL);
	
		// ִ��һ��Ԥ����SQL������
		sqlite3_step(stmt);
	
		// ����һ��׼�����������ĳ�ʼ״̬
		sqlite3_reset(stmt);
	}
	else
	{
		DbgPrintf("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
	}
	
	//�ͷ�sqlite3_stmt
	if (stmt)
		sqlite3_finalize(stmt);
	
	//�ر����ݿ�
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&phone_book_sqlite3_mutex);

	DbgFuncExit();
	
	return ret;
}

//�����ݿ��лָ����е绰����Ϣ
int phone_book_sqlite3_restore_record(void)
{	
	int flag;
	int ret = 0;
	sqlite3 * db = 0;
	sqlite3_stmt * stmt = NULL; 		// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];	
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];
	char *phone_num;
	char *name;
	save_phone_book_t phonebook_info = {0};

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&phone_book_sqlite3_mutex);
	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{		
		pthread_mutex_unlock(&phone_book_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}

	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"select * from %s;",SQL_PHONEBOOK_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	ret = sqlite3_prepare_v2(db,sql, -1, &stmt, 0);
	if (ret == SQLITE_OK)
	{
		phonebook_info.num = 0;
		
		while(SQLITE_DONE != sqlite3_step(stmt))
		{
			//int id,
			
			// ��sqlite3_column_xxx ��0��ʼ��
			// id = sqlite3_column_int(stmt, 0);
			//DbgPrintf("id = %d\r\n",id);

			flag = sqlite3_column_int(stmt, 1);
			phonebook_info.phone_book_msg[phonebook_info.num].flag = (unsigned char)flag;
			DbgPrintf("flag = %d\r\n",phonebook_info.phone_book_msg[phonebook_info.num].flag);
			
			phone_num = (char *)sqlite3_column_text(stmt, 2);
			memcpy(phonebook_info.phone_book_msg[phonebook_info.num].phone_num,phone_num,strlen(phone_num));
			DbgPrintf("phone_num = %s\r\n",phonebook_info.phone_book_msg[phonebook_info.num].phone_num);

			name = (char *)sqlite3_column_text(stmt, 3);
			memcpy(phonebook_info.phone_book_msg[phonebook_info.num].name,name,strlen(name));
			DbgPrintf("name = %s\r\n",phonebook_info.phone_book_msg[phonebook_info.num].name);

			phonebook_info.num++;
			
		}

		phone_book_set_info(&phonebook_info);
		phone_book_info_debug();
	}
	else
	{
		if (stmt)
			sqlite3_finalize(stmt);	
		sqlite3_close(db);
		pthread_mutex_unlock(&phone_book_sqlite3_mutex);

		return ret;
	}

	if (stmt)
		sqlite3_finalize(stmt);
	
	//�ر����ݿ�
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&phone_book_sqlite3_mutex);

	DbgFuncExit();

	return ret;
}

unsigned char phone_book_sqlite3_get_falg_by_number(unsigned char * phone_num)
{
	int ret = 0;
	sqlite3 * db = 0;
	sqlite3_stmt * stmt;			// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	
	unsigned char flag;
	
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&phone_book_sqlite3_mutex);
	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{	
		pthread_mutex_unlock(&phone_book_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return -1;
	}

	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"select * from %s where phonenumber = %s;",SQL_PHONEBOOK_TABLE,phone_num);
	DbgPrintf("sql = %s\r\n",sql);

	ret = sqlite3_prepare_v2(db,sql, -1, &stmt, 0);
	if (ret == SQLITE_OK)
	{
		while(SQLITE_DONE != sqlite3_step(stmt))
		{
			//int id;
			//char *number;
			//char *name;
	
			// ��sqlite3_column_xxx ��0��ʼ��
			//id = sqlite3_column_int(stmt, 0);
			//DbgPrintf("id = %d\r\n",id);

			flag = (unsigned char)sqlite3_column_int(stmt, 1)&0xFF;
			DbgPrintf("flag = 0x%02x\r\n",flag);
			
			//number = (char *)sqlite3_column_text(stmt, 2);
			//DbgPrintf("phone_num = %s\r\n",number);

			//name = (char *)sqlite3_column_text(stmt, 3);
			//DbgPrintf("name = %s\r\n",name);
		}
	}
	else
	{
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);
		pthread_mutex_unlock(&phone_book_sqlite3_mutex);
	
		return -1;
	}

	if (stmt)
		sqlite3_finalize(stmt);
	
	//�ر����ݿ�
    sqlite3_close(db);
    db = 0;
	pthread_mutex_unlock(&phone_book_sqlite3_mutex);

	DbgFuncExit();

	return flag;
}

int phone_book_sqlite3_delete_record(void)
{
	int ret = 0;
	sqlite3 *db = 0;
	sqlite3_stmt * stmt = NULL; 		// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&phone_book_sqlite3_mutex);
	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&phone_book_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}
	
	// ɾ���������ݿ��¼
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"delete from %s ;",SQL_PHONEBOOK_TABLE);
	DbgPrintf("sql = %s\r\n",sql);


	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret != SQLITE_OK)
	{
		DbgError("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
		
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);
		pthread_mutex_unlock(&phone_book_sqlite3_mutex);
		return ret;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_DONE)
	{
		DbgError("sqlite3_step failed!(%d)\r\n",ret);

		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);
		pthread_mutex_unlock(&phone_book_sqlite3_mutex);
		return ret;
	}

	if (stmt)
		sqlite3_finalize(stmt);

	// �ر����ݿ�
	sqlite3_close(db);
	
	pthread_mutex_unlock(&phone_book_sqlite3_mutex);
	
	DbgFuncExit();

	return ret;
}

