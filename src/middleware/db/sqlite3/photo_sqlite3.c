/****************************************Copyright (c)**************************************************                         
** File name:			photo_sqlite3.c
** Descriptions:		��Ƭ���ݿ⹦��
*
*				��Ƭ���ݿ���ֶ�����:
*
* 				@id				    ����ID(���ݿ��Զ�����)
* 				@reason			    ����ԭ��
* 				@encode_mode	    �����ʽ
* 				@lat			    γ��
*               @lon                ����
*               @photo_id           ��ƬID
*               @camera_id          ����ͷID
*               @photograph_time    ����ʱ��
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:	2018-02-05
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

#include	"misc/bcd.h"
#include	"misc/endian.h"

#include	"libs/sqlite3/sqlite3.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/av/gb905_photo.h"

#include	"middleware/event/export/fleety_export.h"

#include	"middleware/db/sqlite3/db_sqlite3_path.h"
#include	"middleware/db/sqlite3/db_sqlite3.h"
#include	"middleware/db/sqlite3/photo_sqlite3.h"

#define		DEBUG_Y
#include	"libs/debug.h"


#define		SQL_PHOTO_TABLE			    "photo_table"
static 		pthread_mutex_t				photo_sqlite3_mutex;


//��Ƭ���ݿ��ʼ��
int photo_sqlite3_init(sqlite3 * db)
{
	int  ret = 0;
	char * err_msg = NULL;
	char sql[SQL_STR_MAX_LEN];

	DbgFuncEntry();
 	
	// �������ݿ��
	snprintf(sql,SQL_STR_MAX_LEN,"create table if not exists %s(id INTEGER PRIMARY KEY AUTOINCREMENT,reason int,encode_mode int,lat int,lon int,photo_id int,camera_id int,photograph_time datetime);",SQL_PHOTO_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	// ִ�н���
	ret = sqlite3_exec(db,sql,0,0,&err_msg);
    if(ret != SQLITE_OK)
    {
        DbgError("SQL create table error:%s\n",err_msg);
        sqlite3_free(err_msg);
    }
    pthread_mutex_init(&photo_sqlite3_mutex, NULL);
	
	DbgFuncExit();
	
    return ret;
}

int photo_sqlite3_insert_record(gb905_photo_info_t *photo_info)
{
 	int	ret = 0;
	sqlite3 *db = 0;
	sqlite3_stmt *stmt;			// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&photo_sqlite3_mutex);	
	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&photo_sqlite3_mutex);
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}

	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"insert into %s (id,reason,encode_mode,lat,lon,photo_id,camera_id,photograph_time) values (?,?,?,?,?,?,?,datetime(CURRENT_TIMESTAMP,'localtime'));",SQL_PHOTO_TABLE);
	DbgPrintf("sql = %s\r\n",sql);


 	// ׼��SQL Ԥ�������
 	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret == SQLITE_OK)
	{
		// ��Ԥ��������еĵ�1,2,3����������sqlite3_bind_xxx ��1��ʼ��
		// sqlite3_bind_int(stmt,1, id);
		sqlite3_bind_int(stmt,2, photo_info->reason);
		sqlite3_bind_int(stmt,3, photo_info->encode_mode);
		sqlite3_bind_int(stmt,4, photo_info->latitude);
        sqlite3_bind_int(stmt,5, photo_info->longitude);
		sqlite3_bind_int(stmt,6, photo_info->photo_id.id);
		sqlite3_bind_int(stmt,7, photo_info->camera_id);

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

    pthread_mutex_unlock(&photo_sqlite3_mutex);

	DbgFuncExit();
	
    return ret;   
}

int photo_sqlite3_get_record_count(gb905_search_pic_t *search_pic)
{
	int ret;
	sqlite3 * db = 0;
	sqlite3_stmt * stmt = NULL;			// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];

	unsigned char start_time[32];
	unsigned char end_time[32];
	int records =0;
	

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

    bcdtime_to_char(search_pic->start_timestamp.bcd,start_time);
	bcdtime_to_char(search_pic->end_timestamp.bcd,end_time);
    
	pthread_mutex_lock(&photo_sqlite3_mutex);	
	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&photo_sqlite3_mutex);
		
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}
	
	memset(sql,0x00,SQL_STR_MAX_LEN);
    
	if(search_pic->camera_id ==0)
	{
        //��ѯ��������ͷ����Ƭ��Ŀ
		snprintf(sql,SQL_STR_MAX_LEN,"select count(*) from %s where photograph_time>'%s' and photograph_time<'%s' and reason=%d;",SQL_PHOTO_TABLE,start_time,end_time,search_pic->reason);
		DbgPrintf("sql = %s\r\n",sql);
	}
	else
	{
        //��ѯ��Ӧ����ͷ����Ƭ��Ŀ
		snprintf(sql,SQL_STR_MAX_LEN,"select count(*) from %s where photograph_time>'%s' and photograph_time<'%s' and camera_id=%d and reason=%d;",SQL_PHOTO_TABLE,start_time,end_time,search_pic->camera_id,search_pic->reason);
		DbgPrintf("sql = %s\r\n",sql);
	}

	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret != SQLITE_OK)
	{
		DbgError("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
		
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);

		pthread_mutex_unlock(&photo_sqlite3_mutex);
		return ret;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW)
	{
		DbgError("sqlite3_step failed!(%d)\r\n",ret);

		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);

		pthread_mutex_unlock(&photo_sqlite3_mutex);
		return ret;
    }

    records = sqlite3_column_int(stmt,0);
    DbgPrintf("records = %d\r\n",records);

    if (stmt)
		sqlite3_finalize(stmt);
	sqlite3_close(db);
    db = 0;
	
	pthread_mutex_unlock(&photo_sqlite3_mutex);
	DbgFuncExit();

	return records;
}

// ���ݼ�������(����ͷid, ����ԭ��,  ��ʼʱ��ͽ���ʱ��)
// ���ÿ����ƬID  
int photo_sqlite3_get_record(gb905_search_pic_t *search_pic,int *content)
{
	int ret;
	sqlite3 * db = 0;
	int offset = 0;
    int	pic_id;
	sqlite3_stmt * stmt = NULL;			// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];
	unsigned char start_time[32];
	unsigned char end_time[32];	

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

    bcdtime_to_char(search_pic->start_timestamp.bcd,start_time);
	bcdtime_to_char(search_pic->end_timestamp.bcd,end_time);
    
	pthread_mutex_lock(&photo_sqlite3_mutex);	
	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&photo_sqlite3_mutex);
		
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}

	memset(sql,0x00,SQL_STR_MAX_LEN);
	if(search_pic->camera_id ==0)
	{
		//�ָ���������ͷ����Ƭ
		snprintf(sql,SQL_STR_MAX_LEN,"select * from %s where photograph_time>'%s' and photograph_time<'%s' and reason=%d;",SQL_PHOTO_TABLE,start_time,end_time,search_pic->reason);
		DbgPrintf("sql = %s\r\n",sql);
	}
	
	else
	{
        //�ָ�ָ������ͷ����Ƭ
		snprintf(sql,SQL_STR_MAX_LEN,"select * from %s where photograph_time>'%s' and photograph_time<'%s' and camera_id=%d and reason=%d;",SQL_PHOTO_TABLE,start_time,end_time,search_pic->camera_id,search_pic->reason);
		DbgPrintf("sql = %s\r\n",sql);
	}
	ret = sqlite3_prepare_v2(db,sql, -1, &stmt, 0);
	if (ret == SQLITE_OK)
	{
		while(SQLITE_DONE != sqlite3_step(stmt))
		{
			
			pic_id = sqlite3_column_int(stmt, 5);
			DbgPrintf("pic_id = %d\r\n",pic_id);
			
			pic_id = EndianReverse32(pic_id);
			memcpy(content+offset,&pic_id,sizeof(int));
			offset++;
		}
	}
	else
	{
		DbgPrintf("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
		if (stmt)
			sqlite3_finalize(stmt);
			
		sqlite3_close(db);
		pthread_mutex_unlock(&photo_sqlite3_mutex);
		return ret;
	}

	if (stmt)
		sqlite3_finalize(stmt);
	
	//�ر����ݿ�
    sqlite3_close(db);
    db = 0;

	pthread_mutex_unlock(&photo_sqlite3_mutex);
	DbgFuncExit();

	return 0;
}


int photo_sqlite3_get_record_id(record_photo_ctrl_t *record_photo_ctrl)
{
	int ret;
    int pic_id;
	sqlite3 * db = 0;
	sqlite3_stmt * stmt = NULL;			// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];
	unsigned char start_time[32];
	unsigned char end_time[32];	

	DbgFuncEntry();

	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

    bcdtime_to_char(record_photo_ctrl->start_timestamp.bcd,start_time);
	bcdtime_to_char(record_photo_ctrl->end_timestamp.bcd,end_time);
    
	pthread_mutex_lock(&photo_sqlite3_mutex);
	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&photo_sqlite3_mutex);
		
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}

    memset(sql,0x00,SQL_STR_MAX_LEN);
    if(record_photo_ctrl->start_timestamp.bcd[0]==0&&
        record_photo_ctrl->start_timestamp.bcd[1]==0&&
        record_photo_ctrl->start_timestamp.bcd[2]==0&&
        record_photo_ctrl->start_timestamp.bcd[3]==0&&
        record_photo_ctrl->start_timestamp.bcd[4]==0&&
        record_photo_ctrl->start_timestamp.bcd[5]==0&&
        record_photo_ctrl->end_timestamp.bcd[0]==0&&
        record_photo_ctrl->end_timestamp.bcd[1]==0&&
        record_photo_ctrl->end_timestamp.bcd[2]==0&&
        record_photo_ctrl->end_timestamp.bcd[3]==0&&
        record_photo_ctrl->end_timestamp.bcd[4]==0&&
        record_photo_ctrl->end_timestamp.bcd[5]==0)
    {
        if(record_photo_ctrl->photo_type ==0xFF)
        {
            //��ѯ����ʱ��,��������ԭ�����Ƭ
            snprintf(sql,SQL_STR_MAX_LEN,"select * from %s",SQL_PHOTO_TABLE);
		    DbgPrintf("sql = %s\r\n",sql);
        }
        else
        {
            //��ѯ����ʱ��,��Ӧ����ԭ�����Ƭ
            snprintf(sql,SQL_STR_MAX_LEN,"select * from %s where reason=%d;",SQL_PHOTO_TABLE,record_photo_ctrl->photo_type);
		    DbgPrintf("sql = %s\r\n",sql);
        }
    }
    else if(record_photo_ctrl->start_timestamp.bcd[0]==0&&
        record_photo_ctrl->start_timestamp.bcd[1]==0&&
        record_photo_ctrl->start_timestamp.bcd[2]==0&&
        record_photo_ctrl->start_timestamp.bcd[3]==0&&
        record_photo_ctrl->start_timestamp.bcd[4]==0&&
        record_photo_ctrl->start_timestamp.bcd[5]==0)
    {
        if(record_photo_ctrl->photo_type ==0xFF)
        {
            //��ѯ���н���ʱ��֮ǰ�ģ���������ԭ�����Ƭ
            snprintf(sql,SQL_STR_MAX_LEN,"select * from %s where photograph_time<'%s';",SQL_PHOTO_TABLE,end_time);
		    DbgPrintf("sql = %s\r\n",sql);
        }
        else
        {
            //��ѯ���н���ʱ��֮ǰ�ģ���Ӧ����ԭ�����Ƭ
            snprintf(sql,SQL_STR_MAX_LEN,"select * from %s where photograph_time<'%s' and reason=%d;",SQL_PHOTO_TABLE,end_time,record_photo_ctrl->photo_type);
		    DbgPrintf("sql = %s\r\n",sql);
        }
    }
    else if(record_photo_ctrl->end_timestamp.bcd[0]==0&&
        record_photo_ctrl->end_timestamp.bcd[1]==0&&
        record_photo_ctrl->end_timestamp.bcd[2]==0&&
        record_photo_ctrl->end_timestamp.bcd[3]==0&&
        record_photo_ctrl->end_timestamp.bcd[4]==0&&
        record_photo_ctrl->end_timestamp.bcd[5]==0)
    {
        if(record_photo_ctrl->photo_type ==0xFF)
        {
            //��ѯ���п�ʼʱ��֮��ģ���������ԭ�����Ƭ
            snprintf(sql,SQL_STR_MAX_LEN,"select * from %s where photograph_time>'%s';",SQL_PHOTO_TABLE,start_time);
		    DbgPrintf("sql = %s\r\n",sql);
        }
        else
        {
            //��ѯ���п�ʼʱ��֮��ģ���Ӧ����ԭ�����Ƭ
            snprintf(sql,SQL_STR_MAX_LEN,"select * from %s where photograph_time>'%s' and reason=%d;",SQL_PHOTO_TABLE,start_time,record_photo_ctrl->photo_type);
		    DbgPrintf("sql = %s\r\n",sql);
        }
    }
    else
    {
       	if(record_photo_ctrl->photo_type ==0xFF)
    	{
            //��ѯ����ʱ�䷶Χ�ڣ���������ԭ�����Ƭ
    		snprintf(sql,SQL_STR_MAX_LEN,"select * from %s where photograph_time>'%s' and photograph_time<'%s';",SQL_PHOTO_TABLE,start_time,end_time);
    		DbgPrintf("sql = %s\r\n",sql);
    	}
    	
    	else
    	{
            //��ѯ����ʱ�䷶Χ�ڣ���������ԭ�����Ƭ
    		snprintf(sql,SQL_STR_MAX_LEN,"select * from %s where photograph_time>'%s' and photograph_time<'%s' and reason=%d;",SQL_PHOTO_TABLE,start_time,end_time,record_photo_ctrl->photo_type);
    		DbgPrintf("sql = %s\r\n",sql);
    	}
	 
    }

    ret = sqlite3_prepare_v2(db,sql, -1, &stmt, 0);
	if (ret == SQLITE_OK)
	{
		while(SQLITE_DONE != sqlite3_step(stmt))
		{
			
		    pic_id = sqlite3_column_int(stmt, 5);
            DbgPrintf("pic_id = %d\r\n",pic_id);

		}
	}
	else
	{
		DbgPrintf("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
		if (stmt)
			sqlite3_finalize(stmt);
			
		sqlite3_close(db);
		pthread_mutex_unlock(&photo_sqlite3_mutex);
	
		return ret;
	}

	if (stmt)
		sqlite3_finalize(stmt);
	
	
	//�ر����ݿ�
    sqlite3_close(db);
    db = 0;
	pthread_mutex_unlock(&photo_sqlite3_mutex);
    
	DbgFuncExit();

	return 0;
}
