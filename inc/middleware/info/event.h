/****************************************Copyright (c)**************************************************                         
** File name:			event.h
** Descriptions:		��¼�¼���Ϣ
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

// �ı���Ϣ�ṹ
typedef struct{
	unsigned char id;								// �¼�ID
	unsigned char content[EVENT_MAX_LEN];			// �¼���Ϣ(�Ϊ20byte)
}__packed msg_event_t;

// �ı���Ϣ����ṹ
typedef struct{
	unsigned int num;								//�¼���Ϣ��Ŀ
	msg_event_t event_msg[EVENT_MAX_NUM];		    //�¼���Ϣ�ṹ
}__packed save_event_t;


void event_set_info(save_event_t *event_info);
void event_get_info(save_event_t *event_info);
void event_info_debug(void);


#ifdef __cplusplus
}
#endif
	
#endif

