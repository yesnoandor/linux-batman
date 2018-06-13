/****************************************Copyright (c)**************************************************                         
** File name:			mcu_common.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-11-30
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _UI_COMMON_H
#define	_UI_COMMON_H

#include	"module/gb905_ex/gb905_ex_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define			SYSTEM2UI_GENERAL_ACK		0x0001
#define			SYSTEM2UI_HEART_BEAT		0x0002	

#define			SYSTEM2UI_CURRENT_NOTICE	0x0602
#define			SYSTEM2UI_HISTORY_INFO		0x0603
#define			SYSTEM2UI_CURRENT_QUESTION	0x0605
#define			SYSTEM2UI_CURRENT_EVENT		0x0606
#define			SYSTEM2UI_ISU_STATE_ACK		0x0607
#define			SYSTEM2UI_ISU_INFO_ACK		0x0608


#define			SYSTEM2UI_LOGIN_INFO_ACK	0x0609
#define			SYSTEM2UI_CALL_STATE		0x0611
#define			SYSTEM2UI_CALL_IN			0x0613
#define			SYSTEM2UI_TERMINAL_REMIND	0x0615


#define			STSTEM2UI_IMEI_REQ			0x0720
#define			SYSTEM2UI_GPS_REQ			0x0721

#define			SYSTEM2UI_ORDER_BRIEF		0x0801
#define			SYSTEM2UI_ORDER_DETIALS		0x0802


#define			UI2SYSTEM_GENERAL_ACK		0x8001
#define			UI2SYSTEM_IMEI_REP			0x8720
#define			UI2SYSTEM_GPS_INFO			0x8721

#define			UI2SYSTEM_HISTORY_INFO		0x8603
#define			UI2SYSTEM_QUESTION_ACK		0x8605
#define			UI2SYSTEM_EVENT_REPORT		0x8606
#define			UI2SYSTEM_ISU_STATE_REQ		0x8607
#define			SYSTEM2UI_ISU_INFO_REQ		0x8608

#define			UI2SYSTEM_LOGIN_INFO_REQ	0x8609
#define			UI2SYSTEM_CALL_OUT			0x8610
#define			UI2SYSTEM_CALL_CONTROL		0x8614
#define			UI2SYSTEM_TERMINAL_CONTROL	0x8616


#define			UI2SYSTEM_ORDER_CONTROL		0x8803

enum{
	UI_RESULT_OK = 0,
	UI_RESULT_FAIL,
	UI_RESULT_UNKNOWN,
};

typedef gb905_ex_header_t ui_protocol_header_t;
typedef gb905_ex_ack_body_t ui_protocol_ack_body_t;
typedef gb905_ex_ack_t ui_protocol_ack_t;

void ui_protocol_build_header(ui_protocol_header_t * header, unsigned short msg_id, unsigned short msg_len);
void ui_protocol_build_ack(ui_protocol_ack_t * ack, ui_protocol_header_t * header, unsigned char result);
void ui_protocol_send_data(unsigned char * buf, int len);
int ui_protocol_ayalyze(unsigned char * buf,int len);
void ui_protocol_send_ack(ui_protocol_header_t * header,unsigned char result);

void ui_protocol_init(void);
void ui_protocol_deinit(void);

	
#ifdef __cplusplus
}
#endif
	
#endif


