/****************************************Copyright (c)**************************************************                         
** File name:			gb905_callback.c
** Descriptions:		 电话回拨(0x8400)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-03
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
#include	"module/gb905/callback/gb905_callback.h"

#define		DEBUG_Y
#include	"libs/debug.h"


#define		GB905_PHONE_MAX_CHAR			20		// 最多20   个字符
	

// 电话回拨下发数据结构
typedef struct{
	unsigned char flag;								// 标志0:普通通话;1:监听
	unsigned char phone_num[GB905_PHONE_MAX_CHAR];	//电话号码(最长为20byte)
}__packed gb905_callback_t;


/*
* @brief 	下行透传数据的处理
* @param buf	 电话回拨消息体的数据地址
* @param len	 电话回拨消息体的数据长度
* 
* @return  返回终端通用应答的状态
*/

unsigned char gb905_callback_treat(unsigned char *buf,int len)
{
	gb905_callback_t *callback;
	
	DbgFuncEntry();

	callback = (gb905_callback_t *)buf;

	DbgPrintf("callback flag is 0x%02x\r\n",callback->flag);
	DbgPrintf("callback phone num is %s \r\n",callback->phone_num);

	//判断是否为监听(falg=0:普通通话;falg=1:监听)
	if(callback->flag)
	{
		//关闭扬声器
	}
	else
	{
		//打开扬声器	
	}
	
	//拨打电话
	
	DbgFuncExit();

	return GB905_RESULT_OK;
}

