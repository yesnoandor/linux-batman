/****************************************Copyright (c)**************************************************                         
** File name:			gb905_event.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
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
#ifndef _GB905_EVENT_H
#define _GB905_EVENT_H
	
#ifdef __cplusplus
extern "C" {
#endif





unsigned char gb905_set_event_treat(unsigned char *buf,int len);
void gb905_report_event_treat(unsigned char evnet_id);

#ifdef __cplusplus
}
#endif
	
#endif

