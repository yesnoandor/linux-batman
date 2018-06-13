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
	unsigned char resolution;	// 分辨率
	
	unsigned char quality;		// 图像质量
	unsigned char brightness;	// 亮度
	unsigned char contrast;		// 对比度
	unsigned char saturation;	// 饱和度
	unsigned char chroma;		// 色度
}photo_params_t;

// 描述一次启动拍照命令
typedef struct{
	unsigned char status;		// =1, 启动拍照
	int total;					// 需要总共拍摄的数量
	int index;					// 当前拍摄的数量
	
	unsigned short interval;	// 拍照间隔：单位秒，0 表示按最小间隔拍照
	unsigned char flag;			// 标志：=1, 保存；=0, 实时上传

    unsigned char camear_id;    //摄像头ID(1~4)
    unsigned char reason;       //拍照原因
	unsigned short serial_number;	// 立即拍照命令的流水号
	photo_params_t params;		// 拍照参数
}photo_ctrl_info_t;

// 描述一次AV  上传进度
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

