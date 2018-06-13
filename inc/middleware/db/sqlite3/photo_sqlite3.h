/****************************************Copyright (c)**************************************************                         
** File name:			photo_sqlite3.c
** Descriptions:		照片数据库功能
*
*				照片数据库表字段如下:
*
* 				@id				序列ID(数据库自动生成)
* 				@reason			拍照原因
* 				@encode_mode	编码格式
* 				@lat			纬度
*               @lon            经度
*               @photo_id       照片ID
*               @camera_id      摄像头ID
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
