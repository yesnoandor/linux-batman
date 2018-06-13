/****************************************Copyright (c)**************************************************                         
** File name:			event_sqlite3.h
** Descriptions:		事件信息数据库功能
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

#ifndef _EVENT_SQLITE3_H
#define _EVENT_SQLITE3_H

#ifdef __cplusplus
extern "C" {
#endif
#include	"libs/sqlite3/sqlite3.h"


int event_sqlite3_init(sqlite3 * db);
int event_sqlite3_insert_record(int id,char *buf,int len);
bool event_sqlite3_judge_event_exist(int event_id);
int event_sqlite3_update_by_id(int event_id,char *descriptor);
int event_sqlite3_restore_record(void);
int event_sqlite3_delete_record(void);

#ifdef __cplusplus
}
#endif
	
#endif


