/****************************************Copyright (c)**************************************************                         
** File name:			notice_sqlite3.c
** Descriptions:		�ı���Ϣ���ݿ⹦��
*
*				�ı���Ϣ���ݿ���ֶ�����:
*
* 				@id				����ID(���ݿ��Զ�����)
* 				@notice_flag		�ı���Ϣ��־
* 				@notice_time		�����ı���Ϣ��ʱ��
* 				@descriptor		�ı���Ϣ��������
*
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-17
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

#include	"middleware/info/notice.h"

#include	"middleware/db/sqlite3/db_sqlite3_path.h"
#include	"middleware/db/sqlite3/db_sqlite3.h"
#include	"middleware/db/sqlite3/notice_sqlite3.h"

#include	"module/gb905/gb905_common.h"

//#define		DEBUG_Y
#include	"libs/debug.h"


#define		SQL_NOTICE_TABLE			"notice_table"

static 		pthread_mutex_t				notice_sqlite3_mutex;


//�ı���Ϣ���ݿ��ʼ������
int notice_sqlite3_init(sqlite3 * db)
{
	int  ret = 0;
	char * err_msg = NULL;
	char sql[SQL_STR_MAX_LEN];

	DbgFuncEntry();
 	
	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"create table if not exists %s(id INTEGER PRIMARY KEY AUTOINCREMENT,notice_flag int,notice_time datetime,descriptor char(512));",SQL_NOTICE_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	// ִ�н���
	ret = sqlite3_exec(db,sql,0,0,&err_msg);
    if(ret != SQLITE_OK)
    {
        DbgError("SQL create table error:%s\n",err_msg);
        sqlite3_free(err_msg);
    }
	
	pthread_mutex_init(&notice_sqlite3_mutex, NULL);
	
	DbgFuncExit();
	
    return ret;
}



/*
* @breif	����һ���ı���Ϣ��¼�����ݿ�
* @param flag		�ı���Ϣ��־
* @param buf			�ı���Ϣ�������ݵĵ�ַ
* @param len			�ı���Ϣ�������ݵĳ���
*
*/	
int notice_sqlite3_insert_record(int flag,unsigned char *buf,int len)
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

	pthread_mutex_lock(&notice_sqlite3_mutex);
	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&notice_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}


	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"insert into %s (id,notice_flag,notice_time,descriptor) values (?,?,datetime(CURRENT_TIMESTAMP,'localtime'),?);",SQL_NOTICE_TABLE);
	DbgPrintf("sql = %s\r\n",sql);


 	// ׼��SQL Ԥ�������
 	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret == SQLITE_OK)
	{
		// ��Ԥ��������еĵ�1,2,3����������sqlite3_bind_xxx ��1��ʼ��
		// sqlite3_bind_int(stmt,1, id);
		sqlite3_bind_int(stmt,2, flag);
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
	pthread_mutex_unlock(&notice_sqlite3_mutex);

	DbgFuncExit();
	
    return ret;
}

//�����ݿ�ָ������ı���Ϣ
int notice_sqlite3_restore_record(void)
{

	int ret;
	sqlite3 * db = 0;
	int flag;	
	sqlite3_stmt * stmt = NULL;			// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	unsigned char *content;
	save_notice_t notice_info = {0};
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&notice_sqlite3_mutex);
		
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{		
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}

	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"select * from %s;",SQL_NOTICE_TABLE);
	DbgPrintf("sql = %s\r\n",sql);
	
	ret = sqlite3_prepare_v2(db,sql, -1, &stmt, 0);
	if (ret == SQLITE_OK)
	{
		notice_info.num = 0;
		
		while(SQLITE_DONE != sqlite3_step(stmt))
		{
			#if 0
			{
				int id;
				id = sqlite3_column_int(stmt, 0);
				DbgPrintf("id = %d\r\n",id);
			}
			#endif

			flag = sqlite3_column_int(stmt, 1);
			notice_info.motice_msg[notice_info.num].flag = flag&0xFF;
			DbgPrintf("notice_flag = 0x%02x\r\n",notice_info.motice_msg[notice_info.num].flag);
			
			content = (unsigned char *)sqlite3_column_text(stmt, 3);
			memcpy(notice_info.motice_msg[notice_info.num].content,content,strlen((char *)content));
			//DbgPrintf("notice_content %s\r\n",notice_info.motice_msg[notice_info.num].content);
			
			notice_info.num++;
			
		}
		//���浽ȫ�ֽṹ��
		notice_set_info(&notice_info);
		notice_info_debug();
		
	}
	else
	{
		DbgError("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);	
		pthread_mutex_unlock(&notice_sqlite3_mutex);

		return ret;
	}

	if (stmt)
		sqlite3_finalize(stmt);
	
	
	//�ر����ݿ�
    sqlite3_close(db);
    db = 0;
	pthread_mutex_unlock(&notice_sqlite3_mutex);

	DbgFuncExit();

	return ret;
}

