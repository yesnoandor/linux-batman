/****************************************Copyright (c)**************************************************                         
** File name:			db_file_path.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-03
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _DB_FILE_PATH_H
#define	_DB_FILE_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

void build_file_path(char * path,const char * name);

void build_db_path(char * path,const char * name);
void build_export_path(char * path,const char * name);


void build_photo_name(char * name,unsigned int id);
void build_photo_path(char * path,const char * name);

void build_status_path(char * path,const char * name);
#ifdef __cplusplus
}
#endif

#endif

