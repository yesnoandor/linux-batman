/****************************************Copyright (c)**************************************************                         
** File name:			ui_event.h
** Descriptions:		�¼���Ϣ����(��APK��Ϣ����)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-21
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _UI_EVENT_H
#define	_UI_EVENT_H


#ifdef __cplusplus
extern "C" {
#endif


void ui_send_history_event_list(unsigned char display_type);
void ui_send_current_event_list(void);
unsigned char  ui_event_report_treat(unsigned char *buf,int len);

	
#ifdef __cplusplus
}
#endif
	

#endif
