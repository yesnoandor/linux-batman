/****************************************Copyright (c)**************************************************                         
** File name:			mcu_volumn.c
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


typedef  struct
{
    unsigned char start_magic_id;
    mcu_protocol_header_t header;
	unsigned char volumn;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed mcu_protocol_volumn_t;


unsigned char mcu_volumn_treat(unsigned char *buf,int len)
{
	unsigned char volumn;

	DbgFuncEntry();

	volumn = *(unsigned char *)(&buf[1 + sizeof(mcu_protocol_header_t)]);

	DbgGood("volumn = 0x%x\r\n",volumn);

	DbgFuncExit();

	return GB905_EX_RESULT_OK;
}


