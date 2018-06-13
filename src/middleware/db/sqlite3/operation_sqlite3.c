/****************************************Copyright (c)**************************************************                         
** File name:			operation_sqlite3.c
** Descriptions:		营运数据信息数据库功能
*
*				提问信息数据库表字段如下:
*
* 				@id				序列ID(数据库自动生成)
* 				@seq			流水号
* 				@state			营运数据上传应答状态(=0:未收到平台应答;=1:收到平台应答)
* 				@timestamp		插入数据库时间
				@operate_bin		营运数据内容(二进制数据)
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

#include	"middleware/socket/fleety_socket.h"
#include	"middleware/db/sqlite3/db_sqlite3_path.h"
#include	"middleware/db/sqlite3/db_sqlite3.h"

#include	"middleware/db/sqlite3/operation_sqlite3.h"

#include	"module/gb905/gb905_common.h"

#include	"app/record/fleety_record.h"


//#define		DEBUG_Y
#include	"libs/debug.h"


#define		SQL_OPERATION_TABLE			"operation_table"

static 		pthread_mutex_t				operation_sqlite3_mutex;

int operation_sqlite3_init(sqlite3 * db)
{
	int  ret = 0;
	char *err_msg = NULL;
	char sql[SQL_STR_MAX_LEN];

	DbgFuncEntry();
 	
	// 创建数据库表
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"create table if not exists %s(id INTEGER PRIMARY KEY AUTOINCREMENT,seq int,state int,timestamp datetime,operate_bin blob);",SQL_OPERATION_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	// 执行建表
	ret = sqlite3_exec(db,sql,0,0,&err_msg);
    if(ret != SQLITE_OK)
    {
        DbgError("SQL create table error:%s\n",err_msg);
        sqlite3_free(err_msg);
    }

	pthread_mutex_init(&operation_sqlite3_mutex, NULL);

	DbgFuncExit();
	
    return ret;
}

/*
* @breif	插入一条营运数据记录到数据库
* @param seq			发送营运数据的流水号
* @param buf			营运数据内容的地址
* @param len			营运数据内容的长度
*
*/	
int operation_sqlite3_insert_record(int seq,unsigned char * buf,int len)
{
	int	ret = 0;
	int	state = 0;	
	sqlite3 *db = 0;
	sqlite3_stmt *stmt;			// 记录预编译之后的SQL  语句句柄
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//构造数据文件路径
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&operation_sqlite3_mutex);	
	
	// 打开一个数据库
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&operation_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}

	// 创建数据库表
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"insert into %s (id,seq,state,timestamp,operate_bin) values (?,?,?,datetime(CURRENT_TIMESTAMP,'localtime'),?);",SQL_OPERATION_TABLE);
	DbgPrintf("sql = %s\r\n",sql);


 	// 准备SQL 预编译语句
 	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret == SQLITE_OK)
	{
		// 绑定预编译语句中的第1,2,3个参数，（sqlite3_bind_xxx 从1开始）
		// sqlite3_bind_int(stmt,1, id);
		sqlite3_bind_int(stmt,2, seq);
		sqlite3_bind_int(stmt,3, state);
		sqlite3_bind_blob(stmt,4,buf,len,NULL);
	
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

    pthread_mutex_unlock(&operation_sqlite3_mutex);

	DbgFuncExit();
	
    return ret;
}

/*
* @breif	收到平台应答后更新营运数据记录的状态
* @param seq			发送营运数据的流水号
*
*/	
int operation_sqlite3_update_record(int seq)
{
	int ret;
	sqlite3 *db = 0;
	sqlite3_stmt *stmt = NULL;			// 记录预编译之后的SQL语句句柄
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//构造数据文件路径
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&operation_sqlite3_mutex);	
	
	// 打开一个数据库
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{
		pthread_mutex_lock(&operation_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}
	
	//更新营运数据信息应答状态
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"update %s set state=1 where seq=%d and state = 0;",SQL_OPERATION_TABLE,seq);
	DbgPrintf("sql = %s\r\n",sql);

	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret != SQLITE_OK)
	{
		DbgError("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
		
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);
		pthread_mutex_unlock(&operation_sqlite3_mutex);
		return ret;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_DONE)
	{
		DbgError("sqlite3_step failed!(%d)\r\n",ret);

		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);
		pthread_mutex_unlock(&operation_sqlite3_mutex);
		
		return ret;
    }

	// 释放sqlite3_stmt
	if (stmt)
		sqlite3_finalize(stmt);
	
	// 关闭数据库
    sqlite3_close(db);

	pthread_mutex_unlock(&operation_sqlite3_mutex);

	DbgFuncExit();

	return ret;
}


// 恢复所有未上传成功的营运数据信息
int operation_sqlite3_restore_record(void)
{
	int ret,i;
	int records = 0;
	sqlite3 *db = 0;
	char *blob_buf = NULL;
	int blob_len = 0;

	gb905_header_t * header;
	sqlite3_stmt *stmt = NULL; 		// 记录预编译之后的SQL语句句柄
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	// 构造数据文件路径
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&operation_sqlite3_mutex);	
	
	// 打开一个数据库
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&operation_sqlite3_mutex);
		
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}
	
	// 获得满足条件的记录总的条数
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"select count(*) from %s where state=0;",SQL_OPERATION_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret != SQLITE_OK)
	{
		DbgError("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
		
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);
		pthread_mutex_unlock(&operation_sqlite3_mutex);
		
		return ret;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW)
	{
		DbgError("sqlite3_step failed!(%d)\r\n",ret);

		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);
		pthread_mutex_unlock(&operation_sqlite3_mutex);
		
		return ret;
	}

	records = sqlite3_column_int(stmt,0);
	DbgPrintf("records = %d\r\n",records);

	if (stmt)
		sqlite3_finalize(stmt);

	if(!records)
	{
		sqlite3_close(db);
		pthread_mutex_unlock(&operation_sqlite3_mutex);
		
		DbgPrintf("no need operation supplment!\r\n");
		return records;
	}

	// 获得满足最近需要补传的营运数据
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"select * from %s where state = 0 order by timestamp desc;",SQL_OPERATION_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	ret = sqlite3_prepare_v2(db,sql, -1, &stmt, 0);
	if (ret == SQLITE_OK)
	{
		while(SQLITE_DONE != sqlite3_step(stmt))
		{
			#if 0
			{
				int id,seq,state;
				char *date;
				time_t timep;
				struct tm p_tm;

				// （sqlite3_column_xxx 从0开始）
				id = sqlite3_column_int(stmt, 0);
				DbgPrintf("id = %d\r\n",id);

				seq = sqlite3_column_int(stmt, 1);
				DbgPrintf("seq = %d\r\n",seq);
				
				state = sqlite3_column_int(stmt, 2);
				DbgPrintf("state = %d\r\n",state);
			}
			
			
			date = (char *)sqlite3_column_text(stmt, 3);

			DbgPrintf("date = %s\r\n",date);
			sscanf(date,"%d-%d-%d %d:%d:%d", &p_tm.tm_year, &p_tm.tm_mon, &p_tm.tm_mday, &p_tm.tm_hour, &p_tm.tm_min, &p_tm.tm_sec);  

			DbgPrintf("%d-%d-%d %d:%d:%d\r\n",p_tm.tm_year - 1900,p_tm.tm_mon - 1,\
				p_tm.tm_mday,p_tm.tm_hour,p_tm.tm_min,p_tm.tm_sec);
			
			timep = mktime(&p_tm);
			DbgPrintf("timep = 0x%x\r\n",timep);
			#endif
			
			blob_buf = (char *)sqlite3_column_blob(stmt,4);
			blob_len = sqlite3_column_bytes(stmt, 4);

			DbgGood("blob_len = %d\r\n",blob_len);
			for(i=0;i<blob_len;i++)
			{
				DbgGood("0x%02x ",blob_buf[i]);
			}
			DbgPrintf("\r\n");

			header = (gb905_header_t *)&blob_buf[1];
			header->msg_serial_number = EndianReverse16(header->msg_serial_number);
			gb905_record_msg(MAIN_SOCKET,header,(unsigned char *)blob_buf,blob_len);
			
		}
	}
	else
	{
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);
		pthread_mutex_unlock(&operation_sqlite3_mutex);
	
		return ret;
	}

	if (stmt)
		sqlite3_finalize(stmt);
	
	//关闭数据库
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&operation_sqlite3_mutex);
	
	DbgFuncExit();
	
	return records;
}

