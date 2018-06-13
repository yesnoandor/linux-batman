/****************************************Copyright (c)**************************************************                         
** File name:			ui_call.h
** Descriptions:		UI与终端的通话信息交互处理
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
#ifndef _UI_CALL_H
#define	_UI_CALL_H


#ifdef __cplusplus
extern "C" {
#endif

unsigned char ui_call_out_treat(unsigned char *buf,int len);
unsigned char ui_call_control_treat(unsigned char *buf,int len);
void ui_send_call_in(char *phone_num);
void ui_send_call_state(unsigned char state);


#ifdef __cplusplus
}
#endif
	
#endif

