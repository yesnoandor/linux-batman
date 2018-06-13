/****************************************Copyright (c)**************************************************                         
** File name:			fleety_call.h
** Descriptions:		 
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
#ifndef _FLEETY_CALL_H
#define	_FLEETY_CALL_H

#ifdef __cplusplus
extern "C" {
#endif

#define		CALL_TYPE_COMMON		0x00		//普通通话
#define		CALL_TYPE_MONITOR		0x01		//监听

#define		ACTIVE_HANG_UP			0x01		//主动挂断
#define		PASSIVE_HANG_UP			0x02		//被动挂断

bool fleety_call_out(char *phone_num,unsigned char type);
void fleety_call_in(char *phone_num);
void fleety_hand_up(unsigned char hang_up_type);
void fleety_answer_call(void);
void fleety_clcc_state_treat(void);
void fleety_call_function_init(void);

#ifdef __cplusplus
}
#endif

#endif
