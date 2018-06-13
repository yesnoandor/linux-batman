/****************************************Copyright (c)**************************************************                         
** File name:			gb905_update_common.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-16
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_UPDATE_COMMON_H
#define	_GB905_UPDATE_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define		UPDATE_ISU_FILE						"isu.bin"
#define		UPDATE_METER_FILE					"meter.bin"
#define		UPDATE_TOPLIGHT_FILE				"toplight.bin"
#define		UPDATE_TSM_FILE						"tsm.bin"

/*
* ����GB905  ��������Э���ͷ��Ϣ
*/
typedef struct{
	unsigned short magic_id;						// '$$'
	unsigned short msg_id;							// ��ϢID
	unsigned short msg_len;							// ��Ϣ������ �����ն˱�ʶ��һ�ֽ�Ϊ��0x10��ʱ����Ϣ������Ŀǰ�洢��Ϊ��Ϣ���Ĵ�С��
	unsigned short msg_serial_number;				// ��Ϣ����ˮ�� ��������˳���0 ��ʼѭ���ۼӣ�
	unsigned char  protocol_version;
	uuid_device_id_t uuid_device_id;				// �ն˱�ʶ֮�豸���
}__packed gb905_update_header_t,*p_gb905_update_header_t;


/*
*  ������Ϣ�����ݽṹ
*/
typedef struct{
	unsigned char device_type;			// �豸���� =0x00,���ܷ����ն�
	unsigned char vendor_id;			// ���̱�� =0x02,���﹫˾
}__packed gb905_base_info_t;

/*
* ������Ϣ�������Ϣ��
*/
typedef gb905_base_info_t	gb905_update_info_req_body_t;


/*
* ������Ϣ�����������Ϣ���ݽṹ
*/
typedef struct
{
    gb905_update_header_t header;
    gb905_update_info_req_body_t body;
    unsigned short crc;
}__packed gb905_update_info_req_t;


/*
* ����������Ϣ��Ӧ��Ϣ������ݽṹ
*/
typedef struct{
	gb905_base_info_t base_info;		// �豸���ͺͳ��̱��
	unsigned char hw_version;			// Ӳ���汾
	unsigned char sw_version[2];		// ����汾
	unsigned int  total_length;			// �����ܳ���(��λ���ֽ�)
	unsigned int  check_sum;			// ��У��
	//unsigned int  download_len;							// spec ��û��
	//unsigned short crc;								// spec��û��
}__packed gb905_update_info_rep_body_t;

typedef gb905_update_info_rep_body_t gb905_update_info_t;

/*
* ���������������Ϣ��
*/
typedef struct{
	gb905_base_info_t base_info;		// �豸���ͺͳ��̱��
	unsigned int address;				// �������ص�ƫ�Ƶ�ַ
}__packed gb905_update_code_req_body_t;


/*
* �������������������Ϣ���ݽṹ
*/
typedef struct
{
    gb905_update_header_t header;
    gb905_update_code_req_body_t body;
    unsigned short crc;
}__packed gb905_update_code_req_t;


/*
* ��������������Ӧ�����ݽṹ
*/
typedef struct{
	gb905_update_info_req_body_t base_info;
	unsigned int address;							// ��ʼ��ַ
	unsigned short size;							// ���ݰ���С
	unsigned char content[512];						// ��������
}__packed gb905_update_code_rep_body_t;


typedef gb905_update_info_req_t	gb905_update_complete_req_t;
typedef gb905_base_info_t	gb905_update_complete_req_ack_body_t;


typedef gb905_base_info_t	gb905_update_cancel_req_body_t;


void gb905_update_info_req(void);
void gb905_update_code_req(int offset);
void gb905_update_complete_req(void);


int gb905_update_protocol_ayalyze(unsigned char * buf,int len);
void gb905_update_send_data(unsigned char * buf, int len);

void gb905_update_debug_header(gb905_update_header_t * header);

#ifdef __cplusplus
}
#endif

#endif

