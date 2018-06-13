/****************************************Copyright (c)**************************************************                         
** File name:			toplight.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-13
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _TOPLIGHT_H
#define _TOPLIGHT_H


#ifdef __cplusplus
extern "C" {
#endif

// ¶¥µÆ×´Ì¬ÏÔÊ¾
enum{
	TOPLIGHT_STATUS_EMPTY=0,
	TOPLIGHT_STATUS_LOADING,
	TOPLIGHT_STATUS_STOP,
	TOPLIGHT_STATUS_ORDER,
	TOPLIGHT_STATUS_ALARM,
	TOPLIGHT_STATUS_SECURITY,
};

void set_toplight_operation_status(unsigned char status);
void set_toplight_alarm_status(void);
void reset_toplight_alarm_status(void);


#ifdef __cplusplus
}
#endif

#endif

