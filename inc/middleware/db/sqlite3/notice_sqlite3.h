/****************************************Copyright (c)**************************************************                         
** File name:			notice_sqlite3.h
** Descriptions:		文本信息数据库功能
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

#ifndef _NOTICE_SQLITE3_H
#define _NOTICE_SQLITE3_H

#ifdef __cplusplus
extern "C" {
#endif
#include	"libs/sqlite3/sqlite3.h"


int notice_sqlite3_init(sqlite3 * db);
int notice_sqlite3_insert_record(int flag,unsigned char *buf,int len);
int notice_sqlite3_restore_record(void);


#ifdef __cplusplus
}
#endif
	
#endif

