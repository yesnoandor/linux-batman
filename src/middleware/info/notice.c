/****************************************Copyright (c)**************************************************                         
** File name:			notice.c
** Descriptions:		��¼�ı���Ϣ
*
*
*
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-12
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

#include	"middleware/info/notice.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static save_notice_t notice_now;

//�����ı���Ϣ
void notice_set_info(save_notice_t *notice_info)
{
	DbgFuncEntry();
	
	memset(&notice_now,0x00,sizeof(save_notice_t));
	memcpy(&notice_now,notice_info,sizeof(save_notice_t));

	DbgFuncExit();
}

//��ȡ�ı���Ϣ
void notice_get_info(save_notice_t *notice_info)
{
	DbgFuncEntry();
	
	memset(notice_info,0x00,sizeof(save_notice_t));
	memcpy(notice_info,&notice_now,sizeof(save_notice_t));

	DbgFuncExit();
}

//��ӡ�ı���Ϣ
void notice_info_debug(void)
{
	int i;
	save_notice_t notice_info;
	
	DbgFuncEntry();
	
	notice_get_info(&notice_info);

	for(i=0;i<notice_info.num;i++)
	{
		DbgPrintf("flag = 0x%0x\r\n",notice_info.motice_msg[i].flag);
    	DbgPrintf("content = %s\r\n",notice_info.motice_msg[i].content);
	}

	DbgFuncExit();
}
