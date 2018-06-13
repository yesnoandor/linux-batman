/****************************************Copyright (c)**************************************************                         
** File name:			gb905_operate.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-26
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_OPERATE_H
#define _GB905_OPERATE_H
	
#ifdef __cplusplus
extern "C" {
#endif

// Ӫ�����ݵĲ�ͬ�׶�״̬
enum{
	OPERATION_BEGIN = 0,
	OPERATION_END,
	OPERATION_EVAL,
};

// Ӫ�����ݵ����ݸ�ʽ
typedef struct {
	report_body_t	begin_report;			// ��ת��ʱ��λ����Ϣ
	report_body_t	end_report;				// ��ת��ʱ��λ����Ϣ
	gb905_timestamp_id_t operation_id;		// Ӫ��ID
	gb905_timestamp_id_t eval_id;			// ����ID
	unsigned char eval_state;				// ����ѡ��
	unsigned short eval_ext;				// ����ѡ����չ
	unsigned int order_id;					// ���ٶ���ID
	unsigned int meter_content;				// �Ƽ���Ӫ������
}__packed operation_body_t;

// Ӫ�����ݵ�������Ϣ���ݸ�ʽ
typedef  struct
{
	unsigned char start_magic_id;
	gb905_header_t header;
	operation_body_t operation_body;
	unsigned char xor;
	unsigned char end_magic_id;
} __packed gb905_operation_t;

void gb905_build_pre_operation(int phase);
void gb905_send_operate(unsigned char * meter_buf,int meter_len);


#ifdef __cplusplus
}
#endif
	
#endif

