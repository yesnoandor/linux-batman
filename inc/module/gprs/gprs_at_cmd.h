/****************************************Copyright (c)**************************************************                         
** File name:			gprs.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-06
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _GPRS_AT_CMD_H
#define _GPRS_AT_CMD_H

bool gprs_get_imei(void);


int gprs_at_protocol_ayalyze(unsigned char * buf,int len);
void gprs_at_protocol_init(void);
bool gprs_get_call_state(void);
bool gprs_dail_out(char *phone_num);
bool gprs_set_clip(void);
int gprs_answer_call(void);
int gprs_hand_up(void);
int gprs_send_clcc(void);


#endif

