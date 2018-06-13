/****************************************Copyright (c)**************************************************                         
** File name:			db_file_xml.h
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

#ifndef _DB_FILE_XML_H
#define	_DB_FILE_XML_H

#ifdef __cplusplus
extern "C" {
#endif

void init_xml(char* path,unsigned char * addr_list[],char * type_list[],int size);
bool xml2data(char* path,unsigned char * addr_list[]);
bool data2xml(char* path,unsigned char * addr_list[]);


#ifdef __cplusplus
}
#endif

#endif

