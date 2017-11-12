/****************************************Copyright (c)**************************************************                         
** File name:			itop_authorize.h
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

#ifndef _INFO2FILE_H
#define	_INFO2FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#define		MAX_PATH_CHARS_SIZE			256
#define		COMMON_PATH					""

int file2info(const char * file_name,unsigned char *buff,int len);
int info2file(const char * file_name,unsigned char *buff,int len);


#ifdef __cplusplus
}
#endif

#endif

