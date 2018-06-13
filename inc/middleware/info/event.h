/****************************************Copyright (c)**************************************************                         
** File name:			event.h
** Descriptions:		记录事件信息
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-18
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _EVENT_H
#define _EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

#define	EVENT_MAX_NUM		100
#define	EVENT_MAX_LEN		20

// 文本信息结构
typedef struct{
	unsigned char id;								// 事件ID
	unsigned char content[EVENT_MAX_LEN];			// 事件信息(最长为20byte)
}__packed msg_event_t;

// 文本信息保存结构
typedef struct{
	unsigned int num;								//事件信息数目
	msg_event_t event_msg[EVENT_MAX_NUM];		    //事件信息结构
}__packed save_event_t;


void event_set_info(save_event_t *event_info);
void event_get_info(save_event_t *event_info);
void event_info_debug(void);


#ifdef __cplusplus
}
#endif
	
#endif

