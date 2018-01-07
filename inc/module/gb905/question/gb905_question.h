/****************************************Copyright (c)**************************************************                         
** File name:			gb905_question.h
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
#ifndef _GB905_QUESTION_H
#define _GB905_QUESTION_H
	
#ifdef __cplusplus
extern "C" {
#endif



unsigned char gb905_question_issue_treat(unsigned char *buf,int len);
void gb905_question_ack_treat(unsigned int question_id,unsigned char answer_id);

#ifdef __cplusplus
}
#endif
	
#endif

