/****************************************Copyright (c)**************************************************                         
** File name:			ui_question.h
** Descriptions:		
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-24
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _UI_QUESTION_H
#define	_UI_QUESTION_H
	
	
	
#ifdef __cplusplus
	extern "C" {
#endif
	
void ui_send_question_info(unsigned char *buf,int len);
unsigned char ui_question_ack_treat(unsigned char *buf,int len);
	
#ifdef __cplusplus
	}
#endif
	
#endif

