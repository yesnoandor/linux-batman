/****************************************Copyright (c)**************************************************                         
** File name:			avh
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

#ifndef _AV_H
#define	_AV_H


#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	unsigned char resolution;	// �ֱ���
	
	unsigned char quality;		// ͼ������
	unsigned char brightness;	// ����
	unsigned char contrast;		// �Աȶ�
	unsigned char saturation;	// ���Ͷ�
	unsigned char chroma;		// ɫ��
}photo_params_t;

// ����һ��������������
typedef struct{
	unsigned char status;		// =1, ��������
	int total;					// ��Ҫ�ܹ����������
	int index;					// ��ǰ���������
	
	unsigned short interval;	// ���ռ������λ�룬0 ��ʾ����С�������
	unsigned char flag;			// ��־��=1, ���棻=0, ʵʱ�ϴ�

    unsigned char camear_id;    //����ͷID(1~4)
    unsigned char reason;       //����ԭ��
	unsigned short serial_number;	// ���������������ˮ��
	photo_params_t params;		// ���ղ���
}photo_ctrl_info_t;

// ����һ��AV  �ϴ�����
typedef struct{
	int photo_id;
	unsigned short reason;
	int offset;
	int total;
}photo_progress_info_t;


void get_photo_ctrl_info(unsigned char * buf);
void set_photo_ctrl_info(unsigned char *buf);
void debug_photo_ctrl_info(void);

void get_photo_progress_info(unsigned char * buf);
void set_photo_progress_info(unsigned char *buf);
void debug_photo_progress_info(void);

#ifdef __cplusplus
}
#endif



#endif

