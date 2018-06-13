/****************************************Copyright (c)**************************************************                         
** File name:			order.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-09
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _ORDER_H
#define _ORDER_H

#ifdef __cplusplus
extern "C" {
#endif

#define		MAX_ORDER_PHONE_CHARS_SIZE		12
#define		MAX_ORDER_DESC_CHARS_SIZE		64
#define		MAX_ORDER_NUM					100		

enum{
	ORDER_STATUS_IDLE = 0,
	ORDER_STATUS_BRIEF,
	ORDER_STATUS_BRIEF_ACK,
	ORDER_STATUS_CRAB,
	ORDER_STATUS_CRAB_ACK,
	ORDER_STATUS_DETAIL,
	ORDER_STATUS_DETAIL_ACK,
	ORDER_STATUS_COMPLETE,
	ORDER_STATUS_COMPLETE_ACK,
	ORDER_STATUS_ISU_CANCEL,
	ORDER_STATUS_ISU_CANCEL_ACK,
	ORDER_STATUS_SERVER_CANCEL,
	ORDER_STATUS_SERVER_CANCEL_ACK,
};


// �·�����������Ϣ�ṹ
typedef struct{
	unsigned int id;								// ����id
	unsigned char type;								// ��������(0:��ʱ�ٳ�;1:ԤԼ�ٳ�;2:����ָ��)
	gb905_timestamp_t ride_time;					// Ҫ��ʱ��(YYMMDDhhmmss)
	char description[MAX_ORDER_DESC_CHARS_SIZE];	// ҵ���������ֽ�(�Գ˿�Ҫ����ŵص������)
}__packed order_info_brief_t;


// �·�����������Ϣ�ṹ
typedef struct{
	unsigned int order_id;							// ����id(���޺����������ʾ�ö���δ�б�)
	unsigned char order_type;						// ��������(0:��ʱ�ٳ�;1:ԤԼ�ٳ�;2:����ָ��)
	gb905_timestamp_t ride_time;					// Ҫ��ʱ��(YYMMDDhhmmss ȫ0��ʾ������)
	unsigned int passenger_lon;						// �˿�λ�þ���(0.0001'  ��0��ʾ������)
	unsigned int passenger_lat;						// �˿�λ��γ��(0.0001'  ��0��ʾ������)
	unsigned int destination_lon;					// Ŀ�ĵ�λ�þ���(0.0001'  ��0��ʾ������)
	unsigned int destination_lat;					// Ŀ�ĵ�λ��γ��(0.0001'  ��0��ʾ������)
	unsigned char service_fee[2];					// ���ٷ����(��ʽΪXXX_X  ,ȫ0��ʾ�޷����)
	char phone[MAX_ORDER_PHONE_CHARS_SIZE];			// �˿͵绰����
	char description[MAX_ORDER_DESC_CHARS_SIZE];	// ������������
}__packed order_info_detail_t;

// 
typedef struct {
	unsigned char status;
	order_info_brief_t brief;
	order_info_detail_t detail;
}order_info_t;


// ��ʷ����������Ϣ�ṹ
typedef struct{
	unsigned char state;								//����״̬
	unsigned int order_id;							// ����id(���޺����������ʾ�ö���δ�б�)
	unsigned char order_type;						// ��������(0:��ʱ�ٳ�;1:ԤԼ�ٳ�;2:����ָ��)
	unsigned int passenger_lon;						// �˿�λ�þ���(0.0001'  ��0��ʾ������)
	unsigned int passenger_lat;						// �˿�λ��γ��(0.0001'  ��0��ʾ������)
	unsigned int destination_lon;					// Ŀ�ĵ�λ�þ���(0.0001'  ��0��ʾ������)
	unsigned int destination_lat;					// Ŀ�ĵ�λ��γ��(0.0001'  ��0��ʾ������)
	char phone[MAX_ORDER_PHONE_CHARS_SIZE];			// �˿͵绰����
	char description[MAX_ORDER_DESC_CHARS_SIZE];	// ������������
}__packed history_order_info_t;


typedef struct{
	unsigned int num;									//��󱣴涩��������
	history_order_info_t order_msg[MAX_ORDER_NUM];		//�������ݽṹ
}__packed save_order_t;

void get_order_brief_info(unsigned char * buf);
void set_order_brief_info(unsigned char * buf);
void debug_order_brief_info(void);


void get_order_detail_info(unsigned char * buf);
void set_order_detail_info(unsigned char * buf);
void debug_order_detail_info(void);

void set_order_status_info(unsigned char state);
unsigned char get_order_status_info(void);

void order_save_info(save_order_t *info);
void order_get_info(save_order_t *info);

#ifdef __cplusplus
}
#endif

#endif


