/****************************************Copyright (c)**************************************************                         
** File name:			demo_sqlite3_path.c
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
#include	"common.h"

#include	"middleware/db/sqlite3/db_sqlite3_path.h"

//#define		DEBUG_Y
#include	"libs/debug.h"


/** 
* @brief 	构建指定文件名的文件在Sqlite3  数据库的绝对路径
* @param path		绝对路径
* @param name		文件名
* 
*/
void build_sqlite3_path(char * path,const char * name)
{
	DbgFuncEntry();

	sprintf(path,"%s%s",DB_FILE_PATH,name);
	
	DbgFuncExit();
}

