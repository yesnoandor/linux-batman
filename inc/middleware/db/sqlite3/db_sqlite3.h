/****************************************Copyright (c)**************************************************                         
** File name:			db_sqlite3.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
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

#ifndef _DB_SQLITE3_H
#define	_DB_SQLITE3_H

#ifdef __cplusplus
extern "C" {
#endif

#define	SQL_STR_MAX_LEN		256
#define		db_sqlite_name			"FleetyDb"

int db_sqlite3_init(void);

#ifdef __cplusplus
}
#endif

#endif

