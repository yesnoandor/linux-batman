/****************************************Copyright (c)**************************************************                         
** File name:			gb905_trace.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-13
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_TRACE_H
#define	_GB905_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

// 位置跟踪策略
enum{
	TIME_POLICY = 0x00,				// = 0x00,时间间隔，持续时间
	TIME_DISTANCE_POLICY = 0x01,	// = 0x01,时间间隔，持续距离	
	DISTANCE_TIME_POLICY = 0x10,	// = 0x10,距离间隔，持续时间
	DISTANCE_POLICY = 0x11,			// = 0x11,距离间隔，持续距离
	TRACE_STOP_POLICY = 0xFF,		// = 0xFF,停止跟踪
};

// 位置跟踪的消息体信息数据格式
typedef struct 
{
	unsigned char policy;			// 跟踪策略
	unsigned short interval;		// 时间(s) /距离间隔(m)
	unsigned int threhold;			// 持续时间(s) /距离(m)
}__packed trace_body_t;



void gb905_trace_send(void);
unsigned char  gb905_trace_treat(unsigned char *buf,int len);

#ifdef __cplusplus
}
#endif
	
#endif

