/****************************************Copyright (c)**************************************************                         
** File name:			gb905_tsm.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-21
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_TSM_H
#define _GB905_TSM_H

#ifdef __cplusplus
extern "C" {
#endif

#include	"module/gb905_peri/gb905_peri_common.h"

enum{
	TSM_BPS2400 = 0,
	TSM_BPS4800,
	TSM_BPS9600,		// default
	TSM_BPS14400,
	TSM_BPS19200,
	TSM_BPS38400,
	TSM_BPS57600,
	TSM_BPS115200,
	TSM_BPS128000,
};

typedef struct{
	unsigned char device_num[5];			// �豸���к�
	unsigned char hard_version;				// Ӳ���汾��
	unsigned char soft_version[2];			// ����汾��
	unsigned char administrative_area[3];	// ������������
	unsigned char device_status;			// �豸״̬(0x00,����; 0x01,ϵͳ�洢�쳣)
	unsigned char esam_type;				// ESAM���ͺ�
	//unsigned char pos_num[5];					// SAM ��״̬
	//unsigned char psam_card_num[5];
	//unsigned int trading_num; 
	//unsigned char rfu[5];
}__packed gb905_tsm_status_t;


typedef struct{
	unsigned char device_status;			// 
	unsigned char SAM_status;
	unsigned char loginout_status;
	unsigned char driver_code[18];
	unsigned char car_num[6];
	unsigned char terminal_func;
	unsigned char one_card_POS_status;
	unsigned char unionpay_POS_status;
	unsigned char extend;
}__packed gb905_tsm_heart_beat_t;


typedef struct{
	gb905_peri_bcd_timestamp_t now_time;		// MCU  ��ǰʱ��
	unsigned char internet_content_status;		// ֧����������״̬
	unsigned char working_status;				// ����״̬
	unsigned char car_num[6];					// �������ƺ���
	unsigned char certification_num[18];		// ����˾����ҵ�ʸ�֤��(���֤����)
}__packed gb905_tsm_heart_beat_ack_t;

typedef struct{
	unsigned char vendor_id;					// ���̱�ʶ
	unsigned char hard_version;					// Ӳ���汾��
	unsigned char soft_major_version;			// ������汾��
	unsigned char soft_minor_version;			// ������汾��
}__packed gb905_tsm_upgrade_t;


typedef struct{
	unsigned char ic_card_type;					// ��������(=0x01 ��·����֤; =0x02,��ҵ�ʸ�֤)
	unsigned char country_version;				// ȫ���汾
	unsigned char place_version;				// �ط��汾
	unsigned char driver_name[12];				// ˾������			(ASCII  ����)
	unsigned char driver_address[48];			// ˾��סַ			(ASCII  ����)
	unsigned char quailficantion_number[18];	// ��ҵ�ʸ�֤��	(ASCII  ����)
	unsigned char quailficantion_type[48];		// ��ҵ�ʸ����	(ASCII  ����)
	unsigned char license_organ[48];			// ��֤����			(ASCII  ����)
	unsigned char useful_life[4];				// ��Ч��ֹ����	(BCD ����YYYYMMDD)
	unsigned char card_number[6];				// ����				(HEX ����)
	unsigned char return_result[2];				// �������	(0x90 0x00 = �ɹ���0xFF 0x0E:��֧�ֵĿ���)
}__packed gb905_tsm_driver_info_t;

typedef struct{
	unsigned char vendor_id;					// ���̱�ʶ
	unsigned short cmd;							// Ӧ�������
	unsigned short result;						// Ӧ���״̬(�ɹ�,  ʧ�ܣ���֧�ֵ�)
}__packed gb905_tsm_common_ack_t;

bool gb905_tsm_query_state(void);
bool gb905_tsm_reset(void);
bool gb905_tsm_upgrade(gb905_tsm_upgrade_t* tsm_date);
bool gb905_tsm_get_driver_info(void);
bool gb905_tsm_decryption(unsigned char* data,unsigned short data_len);


int gb905_tsm_protocol_ayalyze(unsigned char * buf,int len);

#ifdef __cplusplus
}
#endif
	
#endif

