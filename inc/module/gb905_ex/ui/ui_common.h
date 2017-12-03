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
#define			STSTEM2UI_IMEI_REQ			0x0720

#define			UI2SYSTEM_GENERAL_ACK		0x8001
#define			UI2SYSTEM_IMEI_REP			0x8720
#define			UI2SYSTEM_GPS_INFO			0x8702


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


