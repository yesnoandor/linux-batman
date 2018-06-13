/****************************************Copyright (c)**************************************************                         
** File name:			notice.h
** Descriptions:		��¼�ı���Ϣ
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

// �ı���Ϣ�ṹ
typedef struct{
	unsigned char flag;								// ��־
	unsigned char content[NOTICE_MAX_LEN];			// �ı���Ϣ(�Ϊ499byte)
}__packed msg_notice_t;

// �ı���Ϣ����ṹ
typedef struct{
	unsigned int num;								//�ı���Ϣ��Ŀ
	msg_notice_t motice_msg[NOTICE_MAX_NUM];		// �ı���Ϣ�ṹ
}__packed save_notice_t;


void notice_set_info(save_notice_t *notice_info);
void notice_get_info(save_notice_t *notice_info);
void notice_info_debug(void);


#ifdef __cplusplus
}
#endif
	
#endif

