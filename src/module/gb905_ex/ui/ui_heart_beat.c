/****************************************Copyright (c)**************************************************                         
** File name:			ui_heart_beat.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-12-20
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
	
#include	"module/gb905_ex/ui/ui_common.h"
	

	
#define		DEBUG_Y
#include	"libs/debug.h"

#define		UI_HEART_BEAT_THREHOLD		40

typedef  struct
{
    unsigned char start_magic_id;
    ui_protocol_header_t header;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed ui_protocol_heart_beat_t;


static int ui_heart_beat_count = 0;

void ui_heart_beat_reset(void)
{
	ui_heart_beat_count = 0;
}


void ui_heart_beat_send(void)
{
	int len;
	ui_protocol_heart_beat_t heart_beat;

	DbgFuncEntry();
	
	len = 0;
	
	ui_protocol_build_header(&heart_beat.header,SYSTEM2UI_HEART_BEAT,len);

	ui_protocol_send_data((unsigned char *)&heart_beat,sizeof(ui_protocol_heart_beat_t));
		
	DbgFuncExit();
}


void ui_heart_beat_treat(void)
{
	if(ui_heart_beat_count > UI_HEART_BEAT_THREHOLD)
	{
		DbgError("UI Heart Beat Timeout!\r\n");
	}
}


