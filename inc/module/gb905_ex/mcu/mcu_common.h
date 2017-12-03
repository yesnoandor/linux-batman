/****************************************Copyright (c)**************************************************                         
** File name:			mcu_common.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-11-29
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _MCU_COMMON_H
#define	_MCU_COMMON_H

#include	"module/gb905_ex/gb905_ex_common.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#define			MCU2SYSTEM_GENERAL_ACK		0x8001
#define			MCU2SYSTEM_GPIO_IN			0x8003
#define			MCU2SYSTEM_ADC_REP			0x8004
#define			MCU2SYSTEM_EVAL_IN			0x8005
#define			MCU2SYSTEM_VOLUMN_IN		0x8006	
#define			MCU2SYSTEM_GPIO_MAP_REP		0x8007
#define			MCU2SYSTEM_VERSION_REP		0x8040

#define			SYSTEM2MCU_GENERAL_ACK		0x0001
#define			SYSTEM2MCU_HEART_BEAT		0x0002
#define			SYSTEM2MCU_GPIO_OUT			0x0003
#define			SYSTEM2MCU_ADC_REQ			0x0004
#define			SYSTEM2MCU_GPIO_MAP_REQ		0x0007
#define			SYSTEM2MCU_VERSION_REQ		0x0040

#define			SYSTEM2MCU_REBOOT			0x0080


typedef gb905_ex_header_t mcu_protocol_header_t;
typedef gb905_ex_ack_body_t mcu_protocol_ack_body_t;
typedef gb905_ex_ack_t mcu_protocol_ack_t;

void mcu_protocol_build_header(mcu_protocol_header_t * header, unsigned short msg_id, unsigned short msg_len);
void mcu_protocol_build_ack(mcu_protocol_ack_t * ack, mcu_protocol_header_t * header, unsigned char result);
void mcu_protocol_send_data(unsigned char * buf, int len);
int mcu_protocol_ayalyze(unsigned char * buf,int len);
void mcu_protocol_send_ack(mcu_protocol_header_t * header,unsigned char result);


void mcu_protocol_init(void);
void mcu_protocol_deinit(void);

	
#ifdef __cplusplus
}
#endif
	
#endif


