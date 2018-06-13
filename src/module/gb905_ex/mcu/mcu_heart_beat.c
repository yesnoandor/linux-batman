/****************************************Copyright (c)**************************************************                         
** File name:			mcu_heart_beat.c
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
	

	
#define		DEBUG_Y
#include	"libs/debug.h"


#define		MCU_HEART_BEAT_THREHOLD		40


static int mcu_heart_beat_count = 0;

typedef  struct
{
    unsigned char start_magic_id;
    mcu_protocol_header_t header;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed mcu_protocol_heart_beat_t;

void mcu_heart_beat_reset(void)
{
	mcu_heart_beat_count = 0;
}

void mcu_heart_beat_send(void)
{
	int len;
	mcu_protocol_heart_beat_t heart_beat;

	DbgFuncEntry();
	
	len = 0;
	
	mcu_protocol_build_header(&heart_beat.header,SYSTEM2MCU_HEART_BEAT,len);

	mcu_protocol_send_data((unsigned char *)&heart_beat,sizeof(mcu_protocol_heart_beat_t));
		
	DbgFuncExit();
}


void mcu_heart_beat_treat(void)
{
	if(mcu_heart_beat_count > MCU_HEART_BEAT_THREHOLD)
	{
		DbgError("MCU Heart Beat Timeout!\r\n");
	}
}

