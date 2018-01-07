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


#include	"module/gb905/gb905_common.h"
#include	"module/gb905/notice/gb905_notice.h"

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

    DbgFuncEntry();

    notice = (gb905_notice_t *)buf;

    DbgPrintf("flag = 0x%0x\r\n",notice->flag.whole);
    DbgPrintf("content = %s\r\n",(unsigned char *)&notice->content);

    DbgFuncExit();
	
    return GB905_RESULT_OK;
}

