/****************************************Copyright (c)**************************************************                         
** File name:			mcu_version.c
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
#include	"module/gb905_ex/mcu/mcu_version.h"

#define		DEBUG_Y
#include	"libs/debug.h"


typedef struct
{
	unsigned char hw_ver[2];
	unsigned char sw_ver[2];
	unsigned int  uuid[3];
}__packed mcu_protocol_version_rep_t;

typedef  struct
{
	unsigned char start_magic_id;
	mcu_protocol_header_t header;
	unsigned char xor;
	unsigned char end_magic_id;
} __packed mcu_protocol_version_t;

void mcu_send_version_req(void)
{
	mcu_protocol_version_t version;

	DbgFuncEntry();
	
	mcu_protocol_build_header(&version.header,SYSTEM2MCU_VERSION_REQ,0);
	mcu_protocol_send_data((unsigned char *)&version,sizeof(mcu_protocol_version_t));
		
	DbgFuncExit();
}

unsigned char mcu_get_version_treat(unsigned char *buf,int len)
{
	mcu_protocol_version_rep_t version;

	DbgFuncEntry();

	version = *(mcu_protocol_version_rep_t *)(&buf[1 + sizeof(mcu_protocol_header_t)]);
	
	DbgPrintf("hw_ver[0] = 0x%x\r\n",version.hw_ver[0]);
	DbgPrintf("hw_ver[1] = 0x%x\r\n",version.hw_ver[1]);
	DbgPrintf("sw_ver[0] = 0x%x\r\n",version.sw_ver[0]);
	DbgPrintf("sw_ver[1] = 0x%x\r\n",version.sw_ver[1]);
	DbgPrintf("uuid[0] = 0x%x\r\n",version.uuid[0]);
	DbgPrintf("uuid[1] = 0x%x\r\n",version.uuid[1]);
	DbgPrintf("uuid[2] = 0x%x\r\n",version.uuid[2]);

	DbgFuncExit();

	return GB905_EX_RESULT_OK;
	
}

