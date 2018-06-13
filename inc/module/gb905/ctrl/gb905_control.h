/****************************************Copyright (c)**************************************************                         
** File name:			gb905_control.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-27
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _GB905_CONTROL_H
#define _GB905_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

enum{
	GB905_UPDATE_NONE=0,						// �汾��һ�£���������
	GB905_UPDATE_OK,							// �����ɹ�
	GB905_UPDATE_FAIL,							// ����ʧ��
	GB905_UPDATE_VENDOR_MISMATCH,				// ���̱�ʶ��һ��
	GB905_UPDATE_HW_MISMATCH,					// Ӳ���汾�Ų�һ��
	GB905_UPDATE_DOWNLOAD_FAIL,					// ���������ļ�ʧ��
	GB905_UPDATE_SERVER_CANCEL,					// ����������ȡ��
	GB905_UPDATE_ISU_CANCEL						// ISU �豸����ȡ��
};



typedef struct{
	unsigned char cmd;							// ������
	unsigned int content;						// �������(����������ʹ��)
}__packed gb905_control_body_t;


typedef  struct
{
	unsigned char	device_type;				// �豸����
	unsigned char	vendor_id;					// ���̱�ʶ
	unsigned char	hw_version;					// Ӳ���汾��
	unsigned char	sw_version[2];				// ����汾��
} __packed gb905_update_device_info_t;

typedef  struct
{   
	gb905_update_device_info_t device_info;
 	unsigned char result;
} __packed gb905_update_result_body_t;

typedef  struct
{   
    unsigned char start_magic_id;
    gb905_header_t header;
    
	gb905_update_result_body_t body;
 	
    unsigned char xor;
    unsigned char end_magic_id;
} __packed gb905_update_result_t;

// ������������
typedef  struct
{
	gb905_update_device_info_t	device_info;

	char 	update_server_apn[32];				// ����������APN������ͨ�Ų��ŷ��ʵ�
	char 	update_server_username[32];			// ��������������ͨ�Ų����û���
	char 	update_server_password[32];			// ��������������ͨ�Ų�������
	char 	update_server_ipaddr[32];			// ������������ַ,IP ������

	unsigned short	update_server_tcp_port;		// ����������TCP �˿�
}gb905_update_param_t;


unsigned char gb905_control_treat(unsigned char *buf,int len);

void gb905_send_update_result(unsigned char result);


#ifdef __cplusplus
}
#endif



#endif




