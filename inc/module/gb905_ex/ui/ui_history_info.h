/****************************************Copyright (c)**************************************************                         
** File name:			ui_histoory_info.h
** Descriptions:		UI(与APK信息交互)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-22
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _UI_HISTORY_INFO_H
#define	_UI_HISTORY_INFO_H



#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	unsigned char type;
	unsigned char num;
	unsigned int start_id;
	unsigned int end_id;
}__packed ui_history_info_t;

void ui_history_info_display_treat(unsigned char *buf,int len);
void ui_send_history_info(unsigned char display_type);

#ifdef __cplusplus
}
#endif

#endif
