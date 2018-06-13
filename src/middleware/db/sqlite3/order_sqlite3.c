/****************************************Copyright (c)**************************************************                         
** File name:			order_sqlite3.c
** Descriptions:		��ʷ�������ݿ���Ϣ
*				��ʷ������Ϣ���ݿ���ֶ�����:
*
* 				@id				����ID(���ݿ��Զ�����)
* 				@order_id			����ID
* 				@order_type			��������
* 				@order_state			����״̬
*				@start_lon			�˿;���
*				@start_lat			�˿�γ��
*				@end_lon			Ŀ�ĵؾ���
*				@end_lat				Ŀ�ĵ�γ��
*				@phone_number		�˿͵绰
*				@descriptor			��������
**
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-24
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

#include	"module/gb905/gb905_common.h"

#include	"middleware/db/sqlite3/db_sqlite3_path.h"
#include	"middleware/db/sqlite3/db_sqlite3.h"
#include	"middleware/db/sqlite3/order_sqlite3.h"
#include	"middleware/info/order.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define		SQL_ORDER_TABLE				"bookorder_table"

static 		pthread_mutex_t				order_sqlite3_mutex;

int order_sqlite3_init(sqlite3 * db)
{
	int  ret = 0;
	char * err_msg = NULL;
	char sql[SQL_STR_MAX_LEN];

	DbgFuncEntry();

	
	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"create table if not exists %s(id INTEGER PRIMARY KEY AUTOINCREMENT,order_id int,order_type int,order_state int,\
		start_lon int,start_lat int,end_lon int,end_lat int,phone_number char(20),descriptor blob);",SQL_ORDER_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	// ִ�н���
	ret = sqlite3_exec(db,sql,0,0,&err_msg);
    if(ret != SQLITE_OK)
    {
        DbgError("SQL create table error:%s\n",err_msg);
        sqlite3_free(err_msg);
    }

	pthread_mutex_init(&order_sqlite3_mutex, NULL);

	DbgFuncExit();
	
    return ret;
}


int order_sqlite3_insert_record(void)
{
	int ret = 0;
	sqlite3 *db = 0;
	sqlite3_stmt *stmt;			// ��¼Ԥ����֮���SQL�����
	int bolb_len=0;
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];
	order_info_detail_t detail;
	unsigned char state;
	
	DbgFuncEntry();

	get_order_detail_info((unsigned char *)&detail);
	state = get_order_status_info();

	
	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&order_sqlite3_mutex);	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&order_sqlite3_mutex);
		
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}


	// �������ݿ��
	memset(sql,0x00,SQL_STR_MAX_LEN);
	snprintf(sql,SQL_STR_MAX_LEN,"insert into %s (id,order_id,order_type,order_state,start_lon,start_lat,end_lon,end_lat,phone_number,descriptor) values (?,?,?,?,?,?,?,?,?,?);",SQL_ORDER_TABLE);
	DbgPrintf("sql = %s\r\n",sql);

	DbgGood("insert order id = %d\r\n",detail.order_id);
	DbgGood("insert order state = 0x%02x\r\n",state);
	// ׼��SQL Ԥ�������
	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret == SQLITE_OK)
	{
		// ��Ԥ��������еĵ�1,2,3����������sqlite3_bind_xxx ��1��ʼ��
		sqlite3_bind_int(stmt,2, detail.order_id);
		sqlite3_bind_int(stmt,3, detail.order_type);
		sqlite3_bind_int(stmt,4, state);
		sqlite3_bind_int(stmt,5, detail.passenger_lon);
		sqlite3_bind_int(stmt,6, detail.passenger_lat);
		sqlite3_bind_int(stmt,7, detail.destination_lon);
		sqlite3_bind_int(stmt,8, detail.destination_lat);
		
		sqlite3_bind_text(stmt,9, detail.phone,strlen(detail.phone),NULL);
		//sqlite3_bind_text(stmt,10, order_info->descriptor,-1,NULL);
		bolb_len = strlen(detail.description);
		if(bolb_len>255)//
		{
			bolb_len =255;
		}
		sqlite3_bind_blob(stmt,10,detail.description,bolb_len,NULL);
	
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

	pthread_mutex_unlock(&order_sqlite3_mutex);

	DbgFuncExit();
	
	return ret;
}

int order_sqlite3_update_state(void)
{
	int ret = 0;
	sqlite3 *db = 0;
	sqlite3_stmt *stmt = NULL; 		// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];
	order_info_detail_t detail;
    unsigned char order_state;
    
	DbgFuncEntry();

    get_order_detail_info((unsigned char*)&detail);
    order_state = get_order_status_info();
	
	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&order_sqlite3_mutex);	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);
	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&order_sqlite3_mutex);
		
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}
	
	// ���ݶ���ID������Ӧ�Ķ���״̬
	memset(sql,0x00,SQL_STR_MAX_LEN);//update bookorder_table set order_state=3  where order_id=15
	snprintf(sql,SQL_STR_MAX_LEN,"update %s set order_state=%d where order_id=%d;",SQL_ORDER_TABLE,order_state,detail.order_id);
	DbgPrintf("sql = %s\r\n",sql);

	ret = sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if(ret != SQLITE_OK)
	{
		DbgError("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);

		pthread_mutex_unlock(&order_sqlite3_mutex);
		return ret;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_DONE)
	{
		DbgError("sqlite3_step failed!(%d)\r\n",ret);
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(db);

		pthread_mutex_unlock(&order_sqlite3_mutex);
		return ret;
	}

	if (stmt)
		sqlite3_finalize(stmt);

	// �ر����ݿ�
	sqlite3_close(db);
	
	pthread_mutex_unlock(&order_sqlite3_mutex);
	
	DbgFuncExit();

	return ret;

}


int order_sqlite3_restore_record(void)
{

	int ret = 0;
	sqlite3 *db = 0;
	sqlite3_stmt *stmt = NULL; 		// ��¼Ԥ����֮���SQL�����
	char sql[SQL_STR_MAX_LEN];
	char db_sqlite3_path[MAX_PATH_CHARS_SIZE];
	save_order_t save_order;

	int id,order_id,order_type,order_state;
	int start_lon,start_lat,end_lon,end_lat;
	unsigned char *	blob_buf = NULL;
	unsigned int	blob_len = 0;
	char *phone_num;
	
	DbgFuncEntry();
	
	//���������ļ�·��
	build_sqlite3_path(db_sqlite3_path,db_sqlite_name);
	DbgPrintf("path = %s\r\n",db_sqlite3_path);

	pthread_mutex_lock(&order_sqlite3_mutex);	
	// ��һ�����ݿ�
	ret = sqlite3_open(db_sqlite3_path,&db);

	if(ret != SQLITE_OK)
	{
		pthread_mutex_unlock(&order_sqlite3_mutex);
		
		DbgError("can't open sqlite db:%s",sqlite3_errmsg(db));
		return ret;
	}

	memset(sql,0x00,SQL_STR_MAX_LEN);
	
	snprintf(sql,SQL_STR_MAX_LEN,"select * from %s;",SQL_ORDER_TABLE);
	DbgPrintf("sql = %s\r\n",sql);
	
	ret = sqlite3_prepare_v2(db,sql, -1, &stmt, 0);
	if (ret == SQLITE_OK)
	{
		save_order.num = 0;
		while(SQLITE_DONE != sqlite3_step(stmt))
		{
			
			id = sqlite3_column_int(stmt, 0);
			DbgPrintf("id = %d\r\n",id);
			
			order_id = sqlite3_column_int(stmt, 1);
			save_order.order_msg[save_order.num].order_id = order_id;
			DbgPrintf("order_id = %d\r\n",save_order.order_msg[save_order.num].order_id);

			order_type = sqlite3_column_int(stmt, 2);
			save_order.order_msg[save_order.num].order_type = (unsigned char)order_type;
			DbgPrintf("order_type = 0x%02x\r\n",save_order.order_msg[save_order.num].order_type);

			order_state = sqlite3_column_int(stmt, 3);
			save_order.order_msg[save_order.num].state = (unsigned char)order_state;
			DbgGood(" order state = 0x%02x\r\n",save_order.order_msg[save_order.num].state);
			
			start_lon = sqlite3_column_int(stmt, 4);
			save_order.order_msg[save_order.num].passenger_lon = start_lon;
			DbgGood(" start lon = %d\r\n",save_order.order_msg[save_order.num].passenger_lon);
			
			start_lat= sqlite3_column_int(stmt, 5);
			save_order.order_msg[save_order.num].passenger_lat = start_lat;
			DbgGood(" start lat = %d\r\n",save_order.order_msg[save_order.num].passenger_lat);
			
			end_lon = sqlite3_column_int(stmt, 6);
			save_order.order_msg[save_order.num].destination_lon = end_lon;
			DbgGood(" end lon = %d\r\n",save_order.order_msg[save_order.num].destination_lon);
			
			end_lat= sqlite3_column_int(stmt, 7);
			save_order.order_msg[save_order.num].destination_lat = end_lat;
			DbgGood(" end lat = %d\r\n",save_order.order_msg[save_order.num].destination_lat);

			phone_num = (char *)sqlite3_column_text(stmt, 8);
			memset(save_order.order_msg[save_order.num].phone,0x00,MAX_ORDER_PHONE_CHARS_SIZE);
			memcpy(save_order.order_msg[save_order.num].phone,phone_num,strlen(phone_num));
			DbgGood(" phone num = %s\r\n",save_order.order_msg[save_order.num].phone);
			
			blob_buf = (unsigned char *)sqlite3_column_blob(stmt,9);
			blob_len = sqlite3_column_bytes(stmt, 9);

			memset(save_order.order_msg[save_order.num].description,0x00,MAX_ORDER_DESC_CHARS_SIZE);
			if(blob_len>MAX_ORDER_DESC_CHARS_SIZE)
			{
				memcpy(save_order.order_msg[save_order.num].description,blob_buf,MAX_ORDER_DESC_CHARS_SIZE-1);
			}
			else
			{
				memcpy(save_order.order_msg[save_order.num].description,blob_buf,blob_len);
			}
			
			DbgGood(" description = %s\r\n",save_order.order_msg[save_order.num].description);

			save_order.num++;
		}

		order_save_info(&save_order);

	}
	else
	{
		DbgPrintf("sqlite3_prepare_v2 failed!(%d)\r\n",ret);
		if (stmt)
			sqlite3_finalize(stmt);
			
		sqlite3_close(db);

		pthread_mutex_unlock(&order_sqlite3_mutex);
	
		return ret;
	}

	if (stmt)
		sqlite3_finalize(stmt);
	
	
	//�ر����ݿ�
	sqlite3_close(db);
	db = 0;
	pthread_mutex_unlock(&order_sqlite3_mutex);

	DbgFuncExit();

	return ret;
}

