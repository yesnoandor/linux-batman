/****************************************Copyright (c)**************************************************                         
** File name:			ui_notice.h
** Descriptions:		文本信息信息处理(与APK信息交互)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-23
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _UI_NOTICE_H
#define	_UI_NOTICE_H


#ifdef __cplusplus
extern "C" {
#endif

#define		DISPLAY_TYPE_NOTICE			0x00	//文本提示	
#define		DISPLAY_TYPE_TTS			0x01	//语音提示
#define		DISPLAY_TYPE_TTS_NOTICE		0x02	//文本+语音提示

void ui_send_history_notice_list(unsigned char display_type);
void ui_send_current_notice_list(unsigned char *buf,int len,unsigned char type);
void ui_send_terminal_remind(unsigned char *buf,int len);

#ifdef __cplusplus
}
#endif

#endif

