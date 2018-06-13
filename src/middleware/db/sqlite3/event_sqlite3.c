/****************************************Copyright (c)**************************************************                         
** File name:			event_sqlite3.c
** Descriptions:		事件数据库功能
*
*				事件数据库表字段如下:
*
* 				@id				序列ID(数据库自动生成)
* 				@event_id		事件ID
* 				@event_time		保存事件信息的时间
* 				@descriptor		事件内容描述
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

// 事件数据库的初始化
int event_sqlite3_init(sqlite3 * db)
{
	int  ret = 0;
	char * err_msg = NULL;
	char sql[SQL_STR_MAX_LEN];

	DbgFuncEntry();
 	
	// 创建数据库表
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"create table if not exists %s(id INTEGER PRIMARY KEY AUTOINCREMENT,event_id int,event_time datetime,descriptor char(20));",SQL_EVENT_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	// 执行建表
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
* @brief 		  向数据库插入一条事件信息
* @param id	  事件ID
* @param buf	  事件内容地址
* @param len	  事件内容长度
*
*/
int event_sqlite3_insert_record(int id,char *buf,int len)
{
	int	ret = 0;
	sqlite3 * db = 0;
	sqlite3_stmt * stmt;			// 记录预编译之后的SQL语句句柄
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//构造数据文件路径
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&event_sqlite3_mutex);
		
	// 打开一个数据库
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{	
		pthread_mutex_unlock(&event_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}


	// 创建数据库表
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"insert into %s (id,event_id,event_time,descriptor) values (?,?,datetime(CURRENT_TIMESTAMP,'localtime'),?);",SQL_EVENT_TABLE);
	DbgPrintf("sql = %s\r\n",sql);


 	// 准备SQL 预编译语句
 	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret == SQLITE_OK)
	{
		// 绑定预编译语句中的第1,2,3个参数，（sqlite3_bind_xxx 从1开始）
		// sqlite3_bind_int(stmt,1, id);
		sqlite3_bind_int(stmt,2, id);
		sqlite3_bind_text(stmt,3, (const char *)buf,len,NULL);
	
		// 执行一次预编译SQL语句语句
		sqlite3_step(stmt);
	
		// 重置一个准备语句对象到它的初始状态
		sqlite3_reset(stmt);
	}
	else
	{
		DbgPrintf("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
	}
	
	//释放sqlite3_stmt
	if (stmt)
		sqlite3_finalize(stmt);
	
	//关闭数据库
    sqlite3_close(db);
    db = 0;
	pthread_mutex_unlock(&event_sqlite3_mutex);

	DbgFuncExit();
	
    return ret;
}

/*
* @brief 		  通过ID判断数据库中是否存在该事件
* @param id	  事件ID
*
* return 	判断结果
*/
bool event_sqlite3_judge_event_exist(int event_id)
{
	int	ret = false;
	sqlite3 * db = 0;
	sqlite3_stmt * stmt;			// 记录预编译之后的SQL语句句柄
	char sql[SQL_STR_MAX_LEN];
	int record_count =0;
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//构造数据文件路径
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&event_sqlite3_mutex);
		
	// 打开一个数据库
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{	
	
		pthread_mutex_unlock(&event_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		ret = false;
		return ret;
	}


	// 创建数据库表
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

	//判断该事件ID是否已经存在
	if(record_count>0)
	{
		ret = true;
	}
	else
	{
		ret = false;
	}
	
	// 关闭数据库
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&event_sqlite3_mutex);

	DbgFuncExit();
	
	return ret;
}

/*
* @brief 		  根据ID更新该ID的事件内容
* @param id	  事件内容地址
*
*/
int event_sqlite3_update_by_id(int event_id,char *descriptor)
{
	int	ret = 0;
	sqlite3 * db = 0;
	sqlite3_stmt * stmt;			// 记录预编译之后的SQL语句句柄
	char sql[SQL_STR_MAX_LEN];
	
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//构造数据文件路径
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&event_sqlite3_mutex);
		
	// 打开一个数据库
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{	
		pthread_mutex_unlock(&event_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}


	// 创建数据库表
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
	
	// 关闭数据库
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&event_sqlite3_mutex);

	DbgFuncExit();

	return ret;
}


//从数据库恢复所有的事件信息
int event_sqlite3_restore_record(void)
{
	int ret,event_id;
	sqlite3 *db = 0;
	sqlite3_stmt * stmt = NULL; 		// 记录预编译之后的SQL语句句柄
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];
	save_event_t event_info = {0};
	unsigned char *content;

	DbgFuncEntry();

	// 构造数据文件路径
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&event_sqlite3_mutex);
		
	// 打开一个数据库
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&event_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}
	
	// 恢复所有事件数据
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
	
	
	//关闭数据库
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
	sqlite3_stmt *stmt = NULL; 		// 记录预编译之后的SQL语句句柄
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//构造数据文件路径
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&event_sqlite3_mutex);
		
	// 打开一个数据库
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		pthread_mutex_unlock(&event_sqlite3_mutex);
		return ret;
	}
	
	// 删除所有记录
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
	
	
	// 关闭数据库
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&event_sqlite3_mutex);

	DbgFuncExit();

	return ret;
}

