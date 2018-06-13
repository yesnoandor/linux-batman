/****************************************Copyright (c)**************************************************                         
** File name:			gb905_photo.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-02-03
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_PHOTO_H
#define _GB905_PHOTO_H
		
#ifdef __cplusplus
extern "C" {
#endif

#define		MAX_TAKE_PICTURE_INTERVAL			5
#define		MIN_TAKE_PICTURE_INTERVAL			2


// 拍照原因 0:进入重车拍照 1:服务评价拍照 2：报警拍照 3：中心主动拍照
enum{
	PHOTO_REASON_OCCUPY = 0x00,
	PHOTO_REASON_EVALUTION,
	PHOTO_REASON_ALARM,
 	PHOTO_REASON_PLATFORM
};

// 描述查询照片的信息
typedef struct{
	unsigned char camera_id;
	unsigned char reason;
	gb905_timestamp_t start_timestamp;
	gb905_timestamp_t end_timestamp;
}__packed gb905_search_pic_t;


// 描述拍照照片的信息
typedef struct
{
	unsigned char reason;			// 拍照原因 0:进入重车拍照 1:服务评价拍照 2：报警拍照 3：中心主动拍照
	unsigned char encode_mode;		// 编码模式(=0, jpeg)
	unsigned int latitude;			// 拍照时刻纬度
	unsigned int longitude;			// 拍照时刻经度
	gb905_timestamp_id_t photo_id;	// 图片ID   号
	unsigned char camera_id;		// 拍照摄像头的id 
}__packed gb905_photo_info_t;

	
unsigned char gb905_photo_take_picture_treat(unsigned char *buf,int len);	
void gb905_photo_upload_picture_treat(void);
void gb905_photo_upload_start(int photo_id,int offset,unsigned short reason);
void gb905_photo_search_picture_treat(unsigned char* buf,int len);
void gb905_photo_upload_av_request_treat(unsigned char* buf,int len);


#ifdef __cplusplus
}
#endif

#endif
