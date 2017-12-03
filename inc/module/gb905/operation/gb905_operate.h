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


typedef struct {
	report_body_t	begin_report;
	report_body_t	end_report;
	gb905_timestamp_id_t operation_id;
	gb905_timestamp_id_t eval_id;
	unsigned char eval_state;
	unsigned short eval_ext;
	unsigned int order_id;
	unsigned int meter_content;
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





#ifdef __cplusplus
}
#endif
	
#endif

