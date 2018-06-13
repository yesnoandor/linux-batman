/****************************************Copyright (c)**************************************************                         
** File name:			report_sqlite3.h
** Descriptions:		位置汇报数据库功能
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-08
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _REOPRT_SQLITE3_H
#define _REOPRT_SQLITE3_H

#ifdef __cplusplus
extern "C" {
#endif
#include	"libs/sqlite3/sqlite3.h"


int report_sqlite3_init(sqlite3 * db);
int report_sqlite3_insert_record(int seq,unsigned char * buf,int len);
int report_sqlite3_update_state(int seq);
int report_sqlite3_update_seq(sqlite3 * db,int new_seq,int old_seq);
int report_sqlite3_restore_record(void);


#ifdef __cplusplus
}
#endif
	
#endif

