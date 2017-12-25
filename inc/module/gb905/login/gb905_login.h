/****************************************Copyright (c)**************************************************                         
** File name:			gb905_login.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-15
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _GB905_LOGIN_H
#define _GB905_LOGIN_H

#ifdef __cplusplus
extern "C" {
#endif

// �ϰ��½��Ϣ������ݸ�ʽ
typedef struct{
	report_body_t report;							// λ����Ϣ

	unsigned char company_license_number[16];		// ��˾��Ӫ���֤��(ASCII��,  ����16  �ֽڣ��Ҳ�0)
	unsigned char driver_license_number[19];		// ��ʻԱ��ҵ�ʸ�֤��(ASCII��,  ����19  �ֽڣ��Ҳ�0)
	unsigned char plate_number[6];					// ���ƺ�(ASCII ��)
	
	gb905_meter_timestamp_t login_time;				// ��½ʱ��
}__packed gb905_login_body_t;

//  �ϰ��½��������Ϣ���ݸ�ʽ
typedef  struct
{
	unsigned char start_magic_id;
	gb905_header_t header;
	gb905_login_body_t login_body;
	unsigned char xor;
	unsigned char end_magic_id;
} __packed gb905_login_t;

// �°�ǩ����Ϣ������ݸ�ʽ
typedef struct{
	report_body_t report;							// λ����Ϣ
	gb905_meter_close_success_body_t meter_close_info;	// �Ƽ����ػ���Ϣ
	unsigned char logout_mode;						// ǩ�˷�ʽ(0x00,����ǩ��;   0x01, ǿ��ǩ��)
}__packed gb905_logout_body_t;

//  �°�ǩ�˵�������Ϣ���ݸ�ʽ
typedef  struct
{
	unsigned char start_magic_id;
	gb905_header_t header;
	gb905_logout_body_t logout_body;
	unsigned char xor;
	unsigned char end_magic_id;
} __packed gb905_logout_t;


void gb905_login(void);
void gb905_logout(void);


#ifdef __cplusplus
}
#endif



#endif




