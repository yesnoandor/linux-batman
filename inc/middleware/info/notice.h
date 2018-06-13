/****************************************Copyright (c)**************************************************                         
** File name:			notice.h
** Descriptions:		记录文本信息
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

#ifndef _NOTICE_H
#define _NOTICE_H

#ifdef __cplusplus
extern "C" {
#endif

#define	NOTICE_MAX_NUM		100
#define	NOTICE_MAX_LEN		450

// 文本信息结构
typedef struct{
	unsigned char flag;								// 标志
	unsigned char content[NOTICE_MAX_LEN];			// 文本信息(最长为499byte)
}__packed msg_notice_t;

// 文本信息保存结构
typedef struct{
	unsigned int num;								//文本信息数目
	msg_notice_t motice_msg[NOTICE_MAX_NUM];		// 文本信息结构
}__packed save_notice_t;


void notice_set_info(save_notice_t *notice_info);
void notice_get_info(save_notice_t *notice_info);
void notice_info_debug(void);


#ifdef __cplusplus
}
#endif
	
#endif

