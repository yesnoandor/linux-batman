/****************************************Copyright (c)**************************************************                         
** File name:			fleety_report.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-11-10
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _FLEETY_REPORT_H
#define	_FLEETY_REPORT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef  struct
{
	unsigned int distance;
	unsigned int total_distance;
	unsigned int elapse;
	unsigned int total_elapse;
}fleety_report_params_t;

void fleety_report_init(void);
void fleety_report(void);


#ifdef __cplusplus
}
#endif

#endif



