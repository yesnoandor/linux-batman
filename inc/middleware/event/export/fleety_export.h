/****************************************Copyright (c)**************************************************                         
** File name:			fleety_export.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-27
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _FLEETY_EXPORT_H
#define	_FLEETY_EXPORT_H

#ifdef __cplusplus
extern "C" {
#endif


#define	EXPORT_PARAMS_CRL_FILE			"CONFIG.CRL"
#define	EXPORT_RECORD_PHOTO_CRL_FILE	"RPEXPORT.CRL"
#define	EXPORT_OPERATION_CRL_FILE		"YYEXPORT.CRL"
#define	PERIPHERAL_UPDATE_CRL_FILE		"UPDATE.CRL"

enum{
	EXPORT_TYPE_PHOTO=0,			// 导出图片
    EXPORT_TYPE_RECORD,             // 导出录音
    EXPORT_TYPE_YY,					// operation
    EXPORT_TYPE_QT,					// log out
    EXPORT_TYPE_JY					// IC card
};

enum{
	EXPORT_SETTING=0,			// 参数设置
	EXPORT_PARAMS,				// 参数导出
	EXPORT_RECORD_PHOTO,		// 录音，照片采集
	EXPORT_OPERATION,			// 营运数据采集
	EXPORT_UPDATE				// 固件本地升级
};

typedef struct{
	unsigned char	device_type;			// 设备类型
	unsigned char	vendor_id;				// 厂商标识
	unsigned char	hw_version;				// 硬件版本号
	unsigned char	sw_version[2];			// 软件版本号

	unsigned int	file_len;				// 配置文件的总长度
	unsigned int	file_checksum;			// 配置文件的累加和
	unsigned int	file_name;				// 配置文件的名称
}__packed setting_ctrl_t;


typedef struct{
	unsigned char photo_en;					// 照片采集：=0x00, 不采集;  =0x01, 采集
	unsigned char record_en;				// 录音采集：=0x00, 不采集;  =0x01, 采集
	unsigned char photo_type;				// 照片采集类型：=0x00, 重车拍照; =0x01, 服务评价拍照; =0x02, 报警拍照; =0x03, 中心主动拍照; =0xFF, 所有照片
	unsigned char record_type;				// 录音采集类型：=0x00, 正常录音; =0x01, 投诉录音; =0x02, 报警录音; =0xFF, 全部录音
	gb905_timestamp_t start_timestamp;		// 采集开始时间
	gb905_timestamp_t end_timestamp;		// 采集结束时间
	unsigned char ruf[110];
	unsigned short crc;
}__packed record_photo_ctrl_t;

typedef struct{
	gb905_timestamp_t start_timestamp;		// 采集开始时间
	gb905_timestamp_t end_timestamp;		// 采集结束时间
	unsigned char export_classify;			// 采集数据分类标识:   =0x00, 采集全部;  =0x01, 仅采集未上传的数据
	unsigned char ruf[114];					// 
	unsigned short crc;						// 
}__packed operation_ctrl_t;



//gb905_timestamp_t timestamp;		// 时间 (BCD码 YY-MM-DD-hh-mm-ss)


void fleety_export_treat(int type);


#ifdef __cplusplus
}
#endif

#endif

