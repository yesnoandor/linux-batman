/****************************************Copyright (c)**************************************************                         
** File name:			meter.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-14
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _OPERATION_H
#define	_OPERATION_H

#ifdef __cplusplus
extern "C" {
#endif

void get_meter_operation_info(char * buff);
void set_meter_operation_info(char * buff);

void get_meter_open_success_info(char * buf);
void set_meter_open_success_info(char * buf);

void get_meter_close_success_info(char * buf);
void set_meter_close_success_info(char * buf);

#ifdef __cplusplus
}
#endif

#endif

