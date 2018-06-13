/****************************************Copyright (c)**************************************************                         
** File name:			phone_book_sqlite3.h
** Descriptions:		电话薄信息数据库功能
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-09
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _PHONE_BOOK_SQLITE3_H
#define _PHONE_BOOK_SQLITE3_H

#ifdef __cplusplus
extern "C" {
#endif
#include	"libs/sqlite3/sqlite3.h"


int phone_book_sqlite3_init(sqlite3 * db);
int phone_book_sqlite3_insert_record(int flag,char * phone_num,char *name);
int phone_book_sqlite3_restore_record(void);
unsigned char phone_book_sqlite3_get_falg_by_number(unsigned char * phone_num);
int phone_book_sqlite3_delete_record(void);

#ifdef __cplusplus
}
#endif
	
#endif

