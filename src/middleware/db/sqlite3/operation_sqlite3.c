/****************************************Copyright (c)**************************************************                         
** File name:			operation_sqlite3.c
** Descriptions:		Ӫ��������Ϣ���ݿ⹦��
*
*				������Ϣ���ݿ���ֶ�����:
*
* 				@id				����ID(���ݿ��Զ�����)
* 				@seq			��ˮ��
* 				@state			Ӫ�������ϴ�Ӧ��״̬(=0:δ�յ�ƽ̨Ӧ��;=1:�յ�ƽ̨Ӧ��)
* 				@timestamp		�������ݿ�ʱ��
				@operate_bin		Ӫ����������(����������)
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
 	
	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"create table if not exists %s(id INTEGER PRIMARY KEY AUTOINCREMENT,seq int,state int,timestamp datetime,operate_bin blob);",SQL_OPERATION_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	// ִ�н���
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
* @breif	����һ��Ӫ�����ݼ�¼�����ݿ�
* @param seq			����Ӫ�����ݵ���ˮ��
* @param buf			Ӫ���������ݵĵ�ַ
* @param len			Ӫ���������ݵĳ���
*
*/	
int operation_sqlite3_insert_record(int seq,unsigned char * buf,int len)
{
	int	ret = 0;
	int	state = 0;	
	sqlite3 *db = 0;
	sqlite3_stmt *stmt;			// ��¼Ԥ����֮���SQL  �����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&operation_sqlite3_mutex);	
	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&operation_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}

	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"insert into %s (id,seq,state,timestamp,operate_bin) values (?,?,?,datetime(CURRENT_TIMESTAMP,'localtime'),?);",SQL_OPERATION_TABLE);
	DbgPrintf("sql = %s\r\n",sql);


 	// ׼��SQL Ԥ�������
 	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret == SQLITE_OK)
	{
		// ��Ԥ��������еĵ�1,2,3����������sqlite3_bind_xxx ��1��ʼ��
		// sqlite3_bind_int(stmt,1, id);
		sqlite3_bind_int(stmt,2, seq);
		sqlite3_bind_int(stmt,3, state);
		sqlite3_bind_blob(stmt,4,buf,len,NULL);
	
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

    pthread_mutex_unlock(&operation_sqlite3_mutex);

	DbgFuncExit();
	
    return ret;
}

/*
* @breif	�յ�ƽ̨Ӧ������Ӫ�����ݼ�¼��״̬
* @param seq			����Ӫ�����ݵ���ˮ��
*
*/	
int operation_sqlite3_update_record(int seq)
{
	int ret;
	sqlite3 *db = 0;
	sqlite3_stmt *stmt = NULL;			// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&operation_sqlite3_mutex);	
	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{
		pthread_mutex_lock(&operation_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}
	
	//����Ӫ��������ϢӦ��״̬
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

	// �ͷ�sqlite3_stmt
	if (stmt)
		sqlite3_finalize(stmt);
	
	// �ر����ݿ�
    sqlite3_close(db);

	pthread_mutex_unlock(&operation_sqlite3_mutex);

	DbgFuncExit();

	return ret;
}


// �ָ�����δ�ϴ��ɹ���Ӫ��������Ϣ
int operation_sqlite3_restore_record(void)
{
	int ret,i;
	int records = 0;
	sqlite3 *db = 0;
	char *blob_buf = NULL;
	int blob_len = 0;

	gb905_header_t * header;
	sqlite3_stmt *stmt = NULL; 		// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	// ���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&operation_sqlite3_mutex);	
	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&operation_sqlite3_mutex);
		
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}
	
	// ������������ļ�¼�ܵ�����
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

	// ������������Ҫ������Ӫ������
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

				// ��sqlite3_column_xxx ��0��ʼ��
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
	
	//�ر����ݿ�
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&operation_sqlite3_mutex);
	
	DbgFuncExit();
	
	return records;
}

