/****************************************Copyright (c)**************************************************                         
** File name:			gb905_heart_beat.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-04
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

#include	"middleware/socket/fleety_socket.h"

#include	"module/gb905/gb905_common.h"


#define		DEBUG_Y
#include	"libs/debug.h"

#define		GB905_HEART_BEAT_THREHOLD		40

typedef  struct
{
    unsigned char start_magic_id;
    gb905_header_t header;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed gb905_heart_beat_t;


static int gb905_heart_beat_count = 0;


void gb905_heart_beat_reset(void)
{
	gb905_heart_beat_count = 0;
}

/** 
* @brief 	向国标服务器发送心跳命令
*
*/
void gb905_heart_beat_send(void)
{
	int len;
	gb905_heart_beat_t heart_beat;

	DbgFuncEntry();
	
	len = 0;
	
	gb905_build_header(&heart_beat.header,MESSAGE_HEART_BEAT,len);
	
#ifdef DEBUG_Y
	gb905_debug_header(&heart_beat.header);
#endif

	gb905_send_data(MAIN_SOCKET,(unsigned char *)&heart_beat,sizeof(gb905_heart_beat_t));

	gb905_heart_beat_count++;

	DbgFuncExit();
}

void gb905_heart_beat_treat(void)
{
	if(gb905_heart_beat_count > GB905_HEART_BEAT_THREHOLD)
	{
		DbgError("GB905 Heart Beat Timeout!\r\n");
	}
}
