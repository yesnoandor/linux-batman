/****************************************Copyright (c)**************************************************                         
** File name:			gb905_common.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-01-19
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_COMMON_H
#define	_GB905_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

//#define		PROTOCOL_2011	0
//#define		PROTOCOL_2014	1

#define		MESSAGE_GENERAL_UP_ACK				0x0001
#define		MESSAGE_HEART_BEAT					0x0002
#define		MESSAGE_GET_PARAMS_ACK				0x0104
#define		MESSAGE_UPDATE_RESULT				0x0105
#define		MESSAGE_LOCATION_REPORT				0x0200
#define		MESSAGE_GET_LOCATION_ACK			0x0201
#define		MESSAGE_LOCATION_TRACE				0x0202
#define		MESSAGE_LOCATION_SUPPLEMENT_REPORT	0x0203
#define		MESSAGE_EVENT_LIST_ACK				0x0301
#define		MESSAGE_QUESTION_ACK				0x0302
#define 	MESSAGE_REMOTE_CTRL_ACK				0x0500
#define		MESSAGE_PICTURE_UPLOAD				0x0800
#define		MESSAGE_PICTURE_SEARCH_ACK			0x0802
#define		MESSAGE_RECORD_SEARCH_ACK			0x0805
#define		MESSAGE_AUDIO_UPLOAD				0x0806
#define		MESSAGE_ORDER_GRAB					0x0B01

#define		MESSAGE_LOGIN_MSG					0x0B03
#define		MESSAGE_LOGOUT_MSG					0x0B04
#define		MESSAGE_OPERATION_REPORT			0x0B05

#define		MESSAGE_ORDER_FINISH				0x0B07
#define		MESSAGE_ORDER_CANCEL_UP				0x0B08
#define		MESSAGE_PERIPHERY_UP				0x0B10

#define		MESSAGE_OBD_RT_REPORT				0x0F01
#define		MESSAGE_OBD_HBT_REPORT				0x0F02
#define		MESSAGE_OBD_TT_REPORT				0x0F03
#define		MESSAGE_OBD_GET_PARAMS_ACK			0x0F05
#define		MESSAGE_OBD_TRANSPARENT_ACK			0x0F06

#define		MESSAGE_QUERY_TERMINAL_VERSION_ACK	0X0F07
#define		MESSAGE_SEND_TERMIANL_VERSION		0X0F08

#define		MESSAGE_GENERAL_DOWN_ACK			0x8001
#define		MESSAGE_SET_PARAMS					0x8103
#define		MESSAGE_GET_PARAMS					0x8104
#define		MESSAGE_CTRL_TERMINAL				0x8105
#define		MESSAGE_GET_LOCATION				0x8201
#define		MESSAGE_TRACE_LOCATION				0x8202
#define		MESSAGE_TEXT_NOTICE					0x8300
#define		MESSAGE_EVENT_LIST					0x8301
#define		MESSAGE_QUESTION					0x8302
#define		MESSAGE_CALLBACK					0x8400
#define		MESSAGE_PHONE_BOOK					0x8401
#define		MESSAGE_REMOTE_CTRL					0x8500
#define		MESSAGE_PICTURE_CTRL				0x8801
#define		MESSAGE_PICTURE_SEARCH				0x8802
#define		MESSAGE_AV_UPLOAD_REQUEST			0x8803
#define		MESSAGE_RECORD_SEARCH				0x8805
#define		MESSAGE_ORDER_INFO					0x8B00 
#define		MESSAGE_ORDER_GRAB_ACK				0x8B01
#define		MESSAGE_ORDER_CANCEL_DOWN			0x8B09

#define		MESSAGE_CONFIRM_ALARM				0x8B0A
#define		MESSAGE_CANCEL_ALARM				0x8B0B
#define		MESSAGE_PERIPHERY_DOWN				0x8B10

#define		MESSAGE_DEVICE_INSPECTION			0x8B11
#define		MESSAGE_INSPECTION_ACK				0x0B11

#define		MESSAGE_OBD_GET_HBT_REPORT			0x8F02
#define		MESSAGE_OBD_SET_PARAMS				0x8F04
#define		MESSAGE_OBD_GET_PARAMS				0x8F05
#define		MESSAGE_OBD_TRANSPARENT				0x8F06

#define	  	MESSAGE_QUERY_TERMINAL_VERSION		0X8F07

#define		MESSAGE_DOWN_UPGRADE_CMD			0X8F09
#define		MESSAGE_SEND_UPGRADE_DATE			0X8F0A

//----------------����Ϊ�����еر�Э��-----------------------------------------------------------------
#define		BB_ID_CP_REAL_AV_REQ				0x9501 //[sb.AV]ʵʱ����Ƶ��������(A.2.7.39)
#define		BB_ID_CP_REAL_AV_CTL		 		0x9102 //[sb.AV]ʵʱ����Ƶ�������(A.2.7.40)
#define		BB_ID_TE_REAL_AV_REG		 		0x1209 //[sb.AV]ʵʱ����Ƶý��ͨ��ע��(A.2.7.41)
#define		BB_ID_CP_FILE_SEARCH		 		0x9505 //[sb.FILE]�洢����Ƶ����(A.2.7.42)
#define		BB_ID_TE_FILE_LIST			 		0x1505 //[sb.FILE]�ն��ϴ�����Ƶ��Դ�б�(A.2.7.43)
#define		BB_ID_CP_FILE_MONTH_SEARCH	 		0x9508 //[sb.FILE]�ն�����������Ϣ�б�(A.2.7.44)
#define		BB_ID_TE_FILE_MONTH_LIST	 		0x1508 //[sb.FILE]������ѯӦ��(A.2.7.45)
#define		BB_ID_CP_FILE_UPLOAD_REQ	 		0x950b //[sb.FILE]�ļ��ϴ�ָ��(A.2.7.46)
#define		BB_ID_TE_FILE_UPLOAD_REG	 		0x120A //[sb.FILE]�ļ�����ע��(A.2.7.46.1)
#define		BB_ID_CP_RECORD_PLAY_REQ	 		0x9509 //[sb.RECORD]ƽ̨�·�Զ��¼��ط�����(A.2.7.47)
#define		BB_ID_CP_RECORD_PLAY_CTL	 		0x950A //[sb.RECORD]ƽ̨�·�Զ��¼��طſ���(A.2.7.48)


enum{
	GB905_RESULT_OK = 0,
	GB905_RESULT_FAIL,
	GB905_RESULT_UNKNOWN,
};

/*
* �����豸�����Ϣ
*/
typedef struct
{
	unsigned char vendor_id;						// ���̱�� =0x02,���﹫˾
	unsigned char product_id;						// �豸���� =0x00,���ܷ����ն�
	unsigned char mtd_id[3];						// ���к�
}__packed uuid_device_id_t;

/*
* ����GB905  Э���ͷ��Ϣ
*/
typedef struct{
	unsigned short msg_id;							// ��ϢID
	unsigned short msg_len;							// ��Ϣ������ �����ն˱�ʶ��һ�ֽ�Ϊ��0x10��ʱ����Ϣ������Ŀǰ�洢��Ϊ��Ϣ���Ĵ�С��
	unsigned char msg_vendor_type;					// �ն˱�ʶ��һ�ֽ�Ϊ��0x10��
	uuid_device_id_t uuid_device_id;				// �ն˱�ʶ֮�豸���
	unsigned short msg_serial_number;				// ��Ϣ����ˮ�� ��������˳���0 ��ʼѭ���ۼӣ�
}__packed gb905_header_t ,*p_gb905_header_t;


// ͨ��Ӧ�����Ϣ����Ϣ���ݸ�ʽ
typedef struct{
	unsigned short seq;
	unsigned short id;
	unsigned char result;
}__packed ack_body_t;

//ͨ��Ӧ���������Ϣ���ݸ�ʽ 
typedef  struct
{
    unsigned char start_magic_id;
    gb905_header_t header;
    ack_body_t ack_body;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed gb905_ack_t;

// ��BCD  ���ʾʱ��
typedef struct {
	unsigned char bcd[6];				// YY-MM-DD-hh-mm-ss
}__packed gb905_bcd_timestamp_t;

// ��BCD  ���ʾʱ��
typedef struct {
	unsigned char bcd[6];				// YYYY-MM-DD-hh-mm
}__packed gb905_bcd_login_time_t;

typedef union 
{
	unsigned id;
	
	struct
	{
		// low --> high
		unsigned sec:6;			// �� ��0��59��
		unsigned min:6;			// �� ��0��59��
		unsigned hour:5;		// ʱ ��0��23��
		unsigned mday:5;		// �� ��1��31��
		unsigned mon:4;			// �� ��1��12��
		unsigned year:6;		// �� ��2000��2064��
    }timestamp;
}gb905_timestamp_id_t;


void gb905_build_header(gb905_header_t * header, unsigned short msg_id, unsigned short msg_len);
void gb905_build_timestamp(gb905_bcd_timestamp_t * timestamp);
unsigned int gb905_build_timestamp_id(void);

void gb905_send_data(unsigned char socket_index,unsigned char * buf, int len);
void gb905_send_ack(gb905_header_t * header,unsigned char result);

int gb905_protocol_ayalyze(unsigned char * buf,int len);

void gb905_debug_header(gb905_header_t * header);
void gb905_debug_ack(ack_body_t * ack);

#ifdef __cplusplus
}
#endif

#endif

