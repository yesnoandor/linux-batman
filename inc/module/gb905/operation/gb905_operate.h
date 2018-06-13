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

// 营运数据的不同阶段状态
enum{
	OPERATION_BEGIN = 0,
	OPERATION_END,
	OPERATION_EVAL,
};

// 营运数据的数据格式
typedef struct {
	report_body_t	begin_report;			// 空转重时车位置信息
	report_body_t	end_report;				// 重转空时车位置信息
	gb905_timestamp_id_t operation_id;		// 营运ID
	gb905_timestamp_id_t eval_id;			// 评价ID
	unsigned char eval_state;				// 评价选项
	unsigned short eval_ext;				// 评价选项扩展
	unsigned int order_id;					// 电召订单ID
	unsigned int meter_content;				// 计价器营运数据
}__packed operation_body_t;

// 营运数据的整个消息数据格式
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

