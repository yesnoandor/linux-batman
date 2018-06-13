/****************************************Copyright (c)**************************************************                         
** File name:			operation_sqlite3.h
** Descriptions:		营运数据信息数据库功能
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

#ifndef _OPERATION_SQLITE3_H
#define _OPERATION_SQLITE3_H

#ifdef __cplusplus
extern "C" {
#endif
#include	"libs/sqlite3/sqlite3.h"


int operation_sqlite3_init(sqlite3 * db);
int operation_sqlite3_insert_record(int seq,unsigned char * buf,int len);
int operation_sqlite3_update_record(int seq);
int operation_sqlite3_restore_record(void);


#ifdef __cplusplus
}
#endif
	
#endif

