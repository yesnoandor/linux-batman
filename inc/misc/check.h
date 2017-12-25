/****************************************Copyright (c)**************************************************                         
** File name:			check.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-04
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _CHECK_H
#define	_CHECK_H


#ifdef __cplusplus
extern "C" {
#endif

unsigned short crc_computer(const unsigned char * ptr,int count);
unsigned short crc16_compute(const unsigned char * p_data, unsigned int size, const unsigned short * p_crc);
unsigned char xor8_computer(unsigned char * p_data, unsigned int len);
unsigned char sum8_compute(unsigned char * p_data, unsigned int len);
unsigned char sum16_compute(unsigned char * p_data, unsigned int len);


bool is_folder_exist(const char* path);
bool is_file_exist(const char* path);


#ifdef __cplusplus
}
#endif

#endif
