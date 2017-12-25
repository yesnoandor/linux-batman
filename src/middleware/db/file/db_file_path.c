/****************************************Copyright (c)**************************************************                         
** File name:			demo_gprs.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-09
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

#include	"middleware/db/file/db_file_path.h"

//#define		DEBUG_Y
#include	"libs/debug.h"


/** 
* @brief 	构建指定文件名的文件在文件数据库的绝对路径
* @param path		绝对路径
* @param name		文件名
* 
*/
void build_file_path(char * path,const char * name)
{
	DbgFuncEntry();

	sprintf(path,"%s%s",DB_FILE_PATH,name);
	
	DbgFuncExit();
}

