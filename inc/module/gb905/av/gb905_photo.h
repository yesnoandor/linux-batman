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


// ����ԭ�� 0:�����س����� 1:������������ 2���������� 3��������������
enum{
	PHOTO_REASON_OCCUPY = 0x00,
	PHOTO_REASON_EVALUTION,
	PHOTO_REASON_ALARM,
 	PHOTO_REASON_PLATFORM
};

// ������ѯ��Ƭ����Ϣ
typedef struct{
	unsigned char camera_id;
	unsigned char reason;
	gb905_timestamp_t start_timestamp;
	gb905_timestamp_t end_timestamp;
}__packed gb905_search_pic_t;


// ����������Ƭ����Ϣ
typedef struct
{
	unsigned char reason;			// ����ԭ�� 0:�����س����� 1:������������ 2���������� 3��������������
	unsigned char encode_mode;		// ����ģʽ(=0, jpeg)
	unsigned int latitude;			// ����ʱ��γ��
	unsigned int longitude;			// ����ʱ�̾���
	gb905_timestamp_id_t photo_id;	// ͼƬID   ��
	unsigned char camera_id;		// ��������ͷ��id 
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
