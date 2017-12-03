/****************************************Copyright (c)**************************************************                         
** File name:			ui_imei.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-01
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
#include	"module/gb905_ex/ui/ui_imei.h"

#include	"middleware/info/imei.h"

#define		DEBUG_Y
#include	"libs/debug.h"

typedef  struct
{
    unsigned char start_magic_id;
    ui_protocol_header_t header;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed ui_protocol_imei_req_t;



unsigned char ui_imei_info_treat(unsigned char *buf,unsigned int len)
{
	char *imei_info;

	DbgFuncEntry();
	
	imei_info = (char *)buf;
	
	set_imei(imei_info);
	
	DbgFuncExit();

	return GB905_EX_RESULT_OK;
}


void ui_imei_info_req(void)
{
	ui_protocol_imei_req_t imei;

	DbgFuncEntry();
	
	ui_protocol_build_header(&imei.header,STSTEM2UI_IMEI_REQ,0);

	ui_protocol_send_data((unsigned char *)&imei,sizeof(ui_protocol_imei_req_t));
		
	DbgFuncExit();
}
