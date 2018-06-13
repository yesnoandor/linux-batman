/****************************************Copyright (c)**************************************************                         
** File name:			db_file_path.c
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

#include	"middleware/info/device.h"
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

	sprintf(path,"%s%s",path,name);
	
	DbgFuncExit();
}


void build_db_path(char * path,const char * name)
{
	DbgFuncEntry();

	sprintf(path,"%s%s",DB_FILE_PATH,name);
	
	DbgFuncExit();
}

void build_export_path(char * path,const char * name)
{
	DbgFuncEntry();

	sprintf(path,"%s%s",EXPORT_PATH,name);
	DbgPrintf("export path %s \r\n",path);
    
	DbgFuncExit();
}

void build_photo_name(char * name,unsigned int id)
{
	DbgFuncEntry();

	sprintf(name,"photo%d.jpg",id);
	DbgPrintf("build photo name %s \r\n",name);

    DbgFuncExit();
}

void build_photo_path(char * path,const char * name)
{
	DbgFuncEntry();

	sprintf(path,"%s%s",PHOTO_PATH,name);
    DbgPrintf("build photo path %s \r\n",path);
	
	DbgFuncExit();
}

void build_status_path(char * path,const char * name)
{
	DbgFuncEntry();

	sprintf(path,"%s%s",STATUS_PATH,name);
    DbgPrintf("build status path %s \r\n",path);
	
	DbgFuncExit();
}

void build_export_record_photo_path(char *export_path,char *file_name)
{
	device_info_t info;
    
	DbgFuncEntry();

	get_device_info(&info);

	if(!export_path)
	{
		return;
	}

	sprintf(export_path,"%s%d%s",EXPORT_PATH,info.mtd_id,file_name);
    
    DbgFuncExit();
}

void build_operate_export_path(char *export_path,unsigned char export_type)
{
	device_info_t info;
	DbgFuncEntry();
	
	get_device_info(&info);
	
	DbgPrintf("info mtd id = %d \r\n",info.mtd_id);
	if(!export_path)
	{
		return;
	}

	DbgPrintf("export type = 0x%02x \r\n",export_type);
	switch(export_type)
	{
		case 0:
			
			sprintf(export_path,"%s%dYY.DAT",EXPORT_PATH,info.mtd_id);
			break;

		case 1:
			sprintf(export_path,"%s%dQT.DAT",EXPORT_PATH,info.mtd_id);
			break;

		case 2:
			sprintf(export_path,"%s%dJY.DAT",EXPORT_PATH,info.mtd_id);
			break;

		default:
			break;
	}
	
	DbgFuncExit();
}
