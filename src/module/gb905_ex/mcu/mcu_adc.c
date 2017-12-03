/****************************************Copyright (c)**************************************************                         
** File name:			mcu_adc.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-24
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

#define		DEBUG_Y
#include	"libs/debug.h"




typedef  struct
{
    unsigned char start_magic_id;
    mcu_protocol_header_t header;
	unsigned char channel;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed mcu_protocol_adc_t;


void mcu_send_adc_req(unsigned char channel)
{
	mcu_protocol_adc_t adc;

	DbgFuncEntry();
	
	mcu_protocol_build_header(&adc.header,SYSTEM2MCU_ADC_REQ,sizeof(channel));

	adc.channel = channel;

	mcu_protocol_send_data((unsigned char *)&adc,sizeof(mcu_protocol_adc_t));
		
	DbgFuncExit();
}

void mcu_get_adc_treat(unsigned char *buf,int len)
{
	unsigned short adc_in;

	DbgFuncEntry();

	adc_in = *(unsigned short *)(&buf[1 + sizeof(mcu_protocol_header_t)]);

	DbgPrintf("adc_in = %d\r\n",adc_in);


	DbgFuncExit();
	
}


