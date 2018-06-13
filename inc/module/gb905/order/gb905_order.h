/****************************************Copyright (c)**************************************************                         
** File name:			gb905_order.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-05
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_ORDER_H
#define _GB905_ORDER_H
	
#ifdef __cplusplus
extern "C" {
#endif

#include	"module/gb905/gb905_common.h"

enum{
	ORDER_CANCEL_REASON_ACCIDENT = 0,
	ORDER_CANCEL_REASON_BLOCKING,
	ORDER_CANCEL_REASON_OTHERS,
};

enum{
    ORDER_TYPE_REAL_TIME = 0,   //ʵʱ����
    ORDER_TYPE_RESERVED,        //ԤԼ����
    ORDER_TYPE_APPOINT,         //ָ�ɶ���
};

// �·�����������Ϣ�ṹ
typedef struct{
	unsigned int id;						// ����id
	unsigned char type;						// ��������(0:��ʱ�ٳ�;1:ԤԼ�ٳ�;2:����ָ��)
	gb905_timestamp_t ride_time;			// Ҫ��ʱ��(YYMMDDhhmmss)
	char description;				// ҵ���������ֽ�(�Գ˿�Ҫ����ŵص������)
}__packed gb905_order_briefing_t;


//�·�����������Ϣ�ṹ
typedef struct{
	unsigned int order_id;						//����id(���޺����������ʾ�ö���δ�б�)
	unsigned char order_type;					//��������(0:��ʱ�ٳ�;1:ԤԼ�ٳ�;2:����ָ��)
	gb905_timestamp_t ride_time;				//Ҫ��ʱ��(YYMMDDhhmmss ȫ0��ʾ������)
	unsigned int passenger_lon;					//�˿�λ�þ���(0.0001'  ��0��ʾ������)
	unsigned int passenger_lat;					//�˿�λ��γ��(0.0001'  ��0��ʾ������)
	unsigned int destination_lon;				//Ŀ�ĵ�λ�þ���(0.0001'  ��0��ʾ������)
	unsigned int destination_lat;				//Ŀ�ĵ�λ��γ��(0.0001'  ��0��ʾ������)
	unsigned char service_fee[2];				//���ٷ����(��ʽΪXXX_X  ,ȫ0��ʾ�޷����)
	char description;							//�����������ֽ�(���������绰�����ҵ����������)
}__packed gb905_order_details_t;


//�ϴ���ʻԱ����������ṹ
typedef struct{
	unsigned char start_magic_id;
	gb905_header_t header;
	unsigned int order_id;						// ����id(��Ӧ�����еĶ���ID)
	unsigned char xor;
	unsigned char end_magic_id;
}__packed gb905_grab_order_t;

//��ʻԱȡ����Ϣ�ṹ
typedef struct{
	unsigned int id;							// ����id
	unsigned char reason;						// ȡ��ԭ��(0:·��;1:�¹�;2:����)
}__packed isu_cancel_order_body_t;

//�ϴ���ʻԱȡ������������ṹ
typedef struct{
	unsigned char start_magic_id;
	gb905_header_t header;
	isu_cancel_order_body_t cancel_order;
	unsigned char xor;
	unsigned char end_magic_id;
}__packed gb905_isu_cancel_order_t;

//�ϴ���ʻԱ��ɶ���������ṹ
typedef struct{
	unsigned char start_magic_id;
	gb905_header_t header;
	unsigned int order_id;				//  ����id(��Ӧ�����еĶ���ID)
	unsigned char xor;
	unsigned char end_magic_id;
}__packed gb905_finish_order_t;


unsigned char gb905_order_briefing_treat(unsigned char *buf,int len);
unsigned char gb905_order_detials_treat(unsigned char *buf,int len);


void gb905_order_grab_treat(void);
void gb905_order_finish_treat(unsigned int order_id);

void gb905_order_isu_cancel_treat(unsigned int id,unsigned char reason);
unsigned char gb905_order_server_cancel_treat(unsigned char *buf,int len);



#ifdef __cplusplus
}
#endif
	
#endif

