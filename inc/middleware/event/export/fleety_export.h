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
	EXPORT_TYPE_PHOTO=0,			// ����ͼƬ
    EXPORT_TYPE_RECORD,             // ����¼��
    EXPORT_TYPE_YY,					// operation
    EXPORT_TYPE_QT,					// log out
    EXPORT_TYPE_JY					// IC card
};

enum{
	EXPORT_SETTING=0,			// ��������
	EXPORT_PARAMS,				// ��������
	EXPORT_RECORD_PHOTO,		// ¼������Ƭ�ɼ�
	EXPORT_OPERATION,			// Ӫ�����ݲɼ�
	EXPORT_UPDATE				// �̼���������
};

typedef struct{
	unsigned char	device_type;			// �豸����
	unsigned char	vendor_id;				// ���̱�ʶ
	unsigned char	hw_version;				// Ӳ���汾��
	unsigned char	sw_version[2];			// ����汾��

	unsigned int	file_len;				// �����ļ����ܳ���
	unsigned int	file_checksum;			// �����ļ����ۼӺ�
	unsigned int	file_name;				// �����ļ�������
}__packed setting_ctrl_t;


typedef struct{
	unsigned char photo_en;					// ��Ƭ�ɼ���=0x00, ���ɼ�;  =0x01, �ɼ�
	unsigned char record_en;				// ¼���ɼ���=0x00, ���ɼ�;  =0x01, �ɼ�
	unsigned char photo_type;				// ��Ƭ�ɼ����ͣ�=0x00, �س�����; =0x01, ������������; =0x02, ��������; =0x03, ������������; =0xFF, ������Ƭ
	unsigned char record_type;				// ¼���ɼ����ͣ�=0x00, ����¼��; =0x01, Ͷ��¼��; =0x02, ����¼��; =0xFF, ȫ��¼��
	gb905_timestamp_t start_timestamp;		// �ɼ���ʼʱ��
	gb905_timestamp_t end_timestamp;		// �ɼ�����ʱ��
	unsigned char ruf[110];
	unsigned short crc;
}__packed record_photo_ctrl_t;

typedef struct{
	gb905_timestamp_t start_timestamp;		// �ɼ���ʼʱ��
	gb905_timestamp_t end_timestamp;		// �ɼ�����ʱ��
	unsigned char export_classify;			// �ɼ����ݷ����ʶ:   =0x00, �ɼ�ȫ��;  =0x01, ���ɼ�δ�ϴ�������
	unsigned char ruf[114];					// 
	unsigned short crc;						// 
}__packed operation_ctrl_t;



//gb905_timestamp_t timestamp;		// ʱ�� (BCD�� YY-MM-DD-hh-mm-ss)


void fleety_export_treat(int type);


#ifdef __cplusplus
}
#endif

#endif

