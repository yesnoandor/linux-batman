/****************************************Copyright (c)**************************************************                         
** File name:			photo_sqlite3.c
** Descriptions:		��Ƭ���ݿ⹦��
*
*				��Ƭ���ݿ���ֶ�����:
*
* 				@id				����ID(���ݿ��Զ�����)
* 				@reason			����ԭ��
* 				@encode_mode	�����ʽ
* 				@lat			γ��
*               @lon            ����
*               @photo_id       ��ƬID
*               @camera_id      ����ͷID
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:	2018-02-05
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/


#ifndef _PHOTO_SQLITE3_H
#define _PHOTO_SQLITE3_H

#ifdef __cplusplus
extern "C" {
#endif


int photo_sqlite3_init(sqlite3 * db);
int photo_sqlite3_insert_record(gb905_photo_info_t *photo_info);
int photo_sqlite3_get_record_count(gb905_search_pic_t *search_pic);
int photo_sqlite3_get_record(gb905_search_pic_t *search_pic,int *content);
int photo_sqlite3_get_record_id(record_photo_ctrl_t *record_photo_ctrl);

#ifdef __cplusplus
}
#endif
	
#endif
