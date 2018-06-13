/****************************************Copyright (c)**************************************************                         
** File name:			gb905_notice.c
** Descriptions:		国标905    中的文本信息下发
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2017-12-29
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

#include	"middleware/db/sqlite3/notice_sqlite3.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/notice/gb905_notice.h"
#include	"module/gb905_ex/ui/ui_notice.h"

#include	"middleware/info/notice.h"

#define		DEBUG_Y
#include	"libs/debug.h"

// 文本信息下发标志
typedef union 
{
	unsigned char whole;
	
	struct
	{
		// low --> high
		unsigned char urgent:1;		        // bit0 : 紧急
		unsigned char reserved:1;			// bit1 : 预留
		unsigned char lcd:1;                // bit2 : 显示装置显示
		unsigned char tts:1;                // bit3 : 语音合成播读
		unsigned char adv:1;                // bit4 : 广告屏显示
		unsigned char :0;                   // bit5~bit7:预留
    }flag;
}gb905_notice_flag_t;


// 文本信息下发数据结构
typedef struct{
	gb905_notice_flag_t flag;				// 标志
	int content;							// 文本信息(最长为499byte)
}__packed gb905_notice_t;


/*
* @brief 	平台下发文本信息的处理
* @param buf	  下发文本消息体的参数地址
* @param len	  下发文本消息体的参数长度
*
* @return  返回终端通用应答的状态
*/
unsigned char gb905_notice_treat(unsigned char *buf,int len)
{
    gb905_notice_t *notice;
    int data_len;
    DbgFuncEntry();


    notice = (gb905_notice_t *)buf;
    data_len = strlen((char *)&notice->content);
    DbgPrintf("flag = 0x%0x len %d\r\n",notice->flag.whole,data_len);

    //最大文本消息长度为449byte
    if(data_len>NOTICE_MAX_LEN -1)// skip '\0'
    {
        return GB905_RESULT_FAIL;
    }
    
    //DbgPrintf("content = %s\r\n",(unsigned char *)&notice->content);
	
	if(notice->flag.flag.urgent)
	{
		//发送当前文本信息到UI(紧急报文:文本+语音提示)
		ui_send_current_notice_list(buf,len,DISPLAY_TYPE_TTS_NOTICE);
	}
	else
	{
		if((notice->flag.flag.lcd||notice->flag.flag.adv)&&notice->flag.flag.tts)
		{
			//发送当前文本信息到UI(文本+语音提示)
			ui_send_current_notice_list(buf,len,DISPLAY_TYPE_TTS_NOTICE);			
		}
		else if((notice->flag.flag.lcd||notice->flag.flag.adv))
		{
			//发送当前文本信息到UI(仅文本提示)
			ui_send_current_notice_list(buf,len,DISPLAY_TYPE_NOTICE);
		}
		else if(notice->flag.flag.tts)
		{
			//发送当前文本信息到UI(仅语音提示)
			ui_send_current_notice_list(buf,len,DISPLAY_TYPE_TTS);			
		}
		else
		{
			//发送当前文本信息到UI(其他情况:仅文本提示)
			ui_send_current_notice_list(buf,len,DISPLAY_TYPE_NOTICE);
		}
	}
	
	//保存文本信息到数据库
	notice_sqlite3_insert_record((int)notice->flag.whole,(unsigned char *)&notice->content,strlen((char *)&notice->content));

	//notice_sqlite3_restore_record();//for test
	
    DbgFuncExit();
	
    return GB905_RESULT_OK;
}

