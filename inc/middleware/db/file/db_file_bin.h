/****************************************Copyright (c)**************************************************                         
** File name:			db_file_bin.h
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

#ifndef _DB_FILE_BIN_H
#define	_DB_FILE_BIN_H

#ifdef __cplusplus
extern "C" {
#endif

bool file2buf(const char * path,int offset,unsigned char *buff,int len);
bool buf2file(const char * path,int offset,unsigned char *buff,int len);


#ifdef __cplusplus
}
#endif

#endif

