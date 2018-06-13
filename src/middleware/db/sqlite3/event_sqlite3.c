/****************************************Copyright (c)**************************************************                         
** File name:			event_sqlite3.c
** Descriptions:		�¼����ݿ⹦��
*
*				�¼����ݿ���ֶ�����:
*
* 				@id				����ID(���ݿ��Զ�����)
* 				@event_id		�¼�ID
* 				@event_time		�����¼���Ϣ��ʱ��
* 				@descriptor		�¼���������
*
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-18
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

#include	"middleware/info/event.h"
#include	"middleware/db/sqlite3/db_sqlite3_path.h"
#include	"middleware/db/sqlite3/db_sqlite3.h"
#include	"middleware/db/sqlite3/event_sqlite3.h"

#include	"module/gb905/gb905_common.h"

//#define		DEBUG_Y
#include	"libs/debug.h"

#define		SQL_EVENT_TABLE			"event_table"

static 		pthread_mutex_t				event_sqlite3_mutex;

// �¼����ݿ�ĳ�ʼ��
int event_sqlite3_init(sqlite3 * db)
{
	int  ret = 0;
	char * err_msg = NULL;
	char sql[SQL_STR_MAX_LEN];

	DbgFuncEntry();
 	
	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"create table if not exists %s(id INTEGER PRIMARY KEY AUTOINCREMENT,event_id int,event_time datetime,descriptor char(20));",SQL_EVENT_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	// ִ�н���
	ret = sqlite3_exec(db,sql,0,0,&err_msg);
    if(ret != SQLITE_OK)
    {
        DbgError("SQL create table error:%s\n",err_msg);
        sqlite3_free(err_msg);
    }
    pthread_mutex_init(&event_sqlite3_mutex, NULL);

	DbgFuncExit();
	
    return ret;
}

/*
* @brief 		  �����ݿ����һ���¼���Ϣ
* @param id	  �¼�ID
* @param buf	  �¼����ݵ�ַ
* @param len	  �¼����ݳ���
*
*/
int event_sqlite3_insert_record(int id,char *buf,int len)
{
	int	ret = 0;
	sqlite3 * db = 0;
	sqlite3_stmt * stmt;			// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&event_sqlite3_mutex);
		
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{	
		pthread_mutex_unlock(&event_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}


	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"insert into %s (id,event_id,event_time,descriptor) values (?,?,datetime(CURRENT_TIMESTAMP,'localtime'),?);",SQL_EVENT_TABLE);
	DbgPrintf("sql = %s\r\n",sql);


 	// ׼��SQL Ԥ�������
 	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret == SQLITE_OK)
	{
		// ��Ԥ��������еĵ�1,2,3����������sqlite3_bind_xxx ��1��ʼ��
		// sqlite3_bind_int(stmt,1, id);
		sqlite3_bind_int(stmt,2, id);
		sqlite3_bind_text(stmt,3, (const char *)buf,len,NULL);
	
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
	pthread_mutex_unlock(&event_sqlite3_mutex);

	DbgFuncExit();
	
    return ret;
}

/*
* @brief 		  ͨ��ID�ж����ݿ����Ƿ���ڸ��¼�
* @param id	  �¼�ID
*
* return 	�жϽ��
*/
bool event_sqlite3_judge_event_exist(int event_id)
{
	int	ret = false;
	sqlite3 * db = 0;
	sqlite3_stmt * stmt;			// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	int record_count =0;
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&event_sqlite3_mutex);
		
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{	
	
		pthread_mutex_unlock(&event_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		ret = false;
		return ret;
	}


	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"select count(*) from %s where event_id = %d;",SQL_EVENT_TABLE,event_id);
	DbgPrintf("sql = %s\r\n",sql);

	ret = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if(ret == SQLITE_OK)
	{
		sqlite3_step(stmt);
			
		record_count = sqlite3_column_int(stmt,0);
		DbgPrintf("record_count = %d\r\n",record_count);

		sqlite3_finalize(stmt);
	}
	else
	{
		DbgError("sql command failed!(%d)\r\n",ret);

		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);
		pthread_mutex_unlock(&event_sqlite3_mutex);

		ret = false;
		return ret;
	}

	//�жϸ��¼�ID�Ƿ��Ѿ�����
	if(record_count>0)
	{
		ret = true;
	}
	else
	{
		ret = false;
	}
	
	// �ر����ݿ�
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&event_sqlite3_mutex);

	DbgFuncExit();
	
	return ret;
}

/*
* @brief 		  ����ID���¸�ID���¼�����
* @param id	  �¼����ݵ�ַ
*
*/
int event_sqlite3_update_by_id(int event_id,char *descriptor)
{
	int	ret = 0;
	sqlite3 * db = 0;
	sqlite3_stmt * stmt;			// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&event_sqlite3_mutex);
		
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{	
		pthread_mutex_unlock(&event_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}


	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"update %s set descriptor = '%s' where event_id = %d;",SQL_EVENT_TABLE,descriptor,event_id);
	DbgPrintf("sql = %s\r\n",sql);

	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret != SQLITE_OK)
	{
		DbgError("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
		
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);
		pthread_mutex_unlock(&event_sqlite3_mutex);

		return ret;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_DONE)
	{
		DbgError("sqlite3_step failed!(%d)\r\n",ret);

		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);
		pthread_mutex_unlock(&event_sqlite3_mutex);

		return ret;
    }
	
	// �ر����ݿ�
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&event_sqlite3_mutex);

	DbgFuncExit();

	return ret;
}


//�����ݿ�ָ����е��¼���Ϣ
int event_sqlite3_restore_record(void)
{
	int ret,event_id;
	sqlite3 *db = 0;
	sqlite3_stmt * stmt = NULL; 		// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];
	save_event_t event_info = {0};
	unsigned char *content;

	DbgFuncEntry();

	// ���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&event_sqlite3_mutex);
		
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&event_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}
	
	// �ָ������¼�����
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"select * from %s;",SQL_EVENT_TABLE);
	DbgPrintf("sql = %s\r\n",sql);
	
	ret = sqlite3_prepare_v2(db,sql, -1, &stmt, 0);
	if (ret == SQLITE_OK)
	{
		event_info.num = 0;
		
		while(SQLITE_DONE != sqlite3_step(stmt))
		{
			#if 0
			{
				int id;
				
				id = sqlite3_column_int(stmt, 0);
				DbgPrintf("id = %d\r\n",id);
			}
			#endif

			event_id = sqlite3_column_int(stmt, 1);
			event_info.event_msg[event_info.num].id = (unsigned char)event_id;
			DbgPrintf("event_id = 0x%x\r\n",event_info.event_msg[event_info.num].id);

			content = (unsigned char *)sqlite3_column_text(stmt, 3);
			memcpy(event_info.event_msg[event_info.num].content,content,strlen((char *)content));
			DbgPrintf("event_content = %s\r\n",event_info.event_msg[event_info.num].content);

			event_info.num++;
		}

		event_set_info(&event_info);
		event_info_debug();
	}
	else
	{
		DbgPrintf("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
		if (stmt)
			sqlite3_finalize(stmt);	
		sqlite3_close(db);
		pthread_mutex_unlock(&event_sqlite3_mutex);
	
		return ret;
	}

	if (stmt)
		sqlite3_finalize(stmt);
	
	
	//�ر����ݿ�
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&event_sqlite3_mutex);

	DbgFuncExit();

	return ret;
}

int event_sqlite3_delete_record(void)
{
	int ret = 0;
	sqlite3 *db = 0;
	sqlite3_stmt *stmt = NULL; 		// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&event_sqlite3_mutex);
		
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		pthread_mutex_unlock(&event_sqlite3_mutex);
		return ret;
	}
	
	// ɾ�����м�¼
	snprintf(sql,SQL_STR_MAX_LEN,"delete from %s;",SQL_EVENT_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
	{
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

	}
	else
	{
		DbgError("sql command failed!(%s)\r\n",sql);
		if (stmt)
			sqlite3_finalize(stmt);

		sqlite3_close(db);
		pthread_mutex_unlock(&event_sqlite3_mutex);
		return ret;
	}
	
	
	// �ر����ݿ�
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&event_sqlite3_mutex);

	DbgFuncExit();

	return ret;
}

