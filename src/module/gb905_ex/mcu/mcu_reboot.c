/****************************************Copyright (c)**************************************************                         
** File name:			mcu_reboot.c
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

#include	"common.h"

#include	"module/gb905_ex/mcu/mcu_common.h"

#include	"middleware/uart/fleety_uart.h"

#define		DEBUG_Y
#include	"libs/debug.h"


typedef  struct
{
    unsigned char start_magic_id;
    mcu_protocol_header_t header;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed mcu_protocol_reboot_t;

void stm32_send_reboot_req(unsigned int out)
{
	mcu_protocol_reboot_t reboot;

	DbgFuncEntry();
	
	mcu_protocol_build_header(&reboot.header,SYSTEM2MCU_REBOOT,0);
	mcu_protocol_send_data((unsigned char *)&reboot,sizeof(mcu_protocol_reboot_t));
		
	DbgFuncExit();
}

