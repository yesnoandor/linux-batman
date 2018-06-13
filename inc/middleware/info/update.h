/****************************************Copyright (c)**************************************************                         
** File name:			update.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-29
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _UPDATE_H
#define	_UPDATE_H

#ifdef __cplusplus
extern "C" {
#endif


void get_update_param(unsigned char * buf);
void set_update_param(unsigned char * buf);

void get_update_info(unsigned char * buf);
void set_update_info(unsigned char * buf);

unsigned char get_update_result(void);
void set_update_result(unsigned char result);

#ifdef __cplusplus
}
#endif

#endif

