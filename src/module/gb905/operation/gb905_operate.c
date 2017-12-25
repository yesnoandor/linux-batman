/****************************************Copyright (c)**************************************************                         
** File name:			gb905_operate.c
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
#include	"common.h"

#include	"misc/endian.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"

#include	"module/gb905/operation/gb905_operate.h"

#include	"middleware/info/eval.h"
#include	"middleware/socket/fleety_socket.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static operation_body_t pre_operation;


/**
* @brief 	填充营运数据上车时的信息
*
*/
static void gb905_build_operation_start(void)
{
	DbgFuncEntry();

	gb905_build_report_body(&pre_operation.begin_report);
	
	DbgFuncExit();
}

/**
* @brief 	填充营运数据下车时的信息
*
*/
static void gb905_build_operation_end(void)
{
	DbgFuncEntry();

	gb905_build_report_body(&pre_operation.end_report);
	pre_operation.operation_id.id = EndianReverse32(gb905_build_timestamp_id());
	
	DbgFuncExit();
}

/**
* @brief 	填充营运数据受到服务评价时的信息
*
*/
static void gb905_build_operation_eval(void)
{	
	DbgFuncEntry();

	pre_operation.eval_id.id = EndianReverse32(gb905_build_timestamp_id());
	pre_operation.eval_state = get_eval_state();
	pre_operation.eval_ext = 0x00;
	
	DbgFuncExit();
}


void gb905_build_operation(unsigned char * buf,int len,unsigned char * meter_buf,int meter_len)
{
	gb905_header_t * header;
	unsigned short msg_id;
	unsigned short msg_len;
		
	int offset;

	DbgFuncEntry();

	offset = 1;
	header = (gb905_header_t *)&buf[offset];

	msg_id = MESSAGE_OPERATION_REPORT;
	msg_len = len - 3 - sizeof(gb905_header_t);

	gb905_build_header(header,msg_id,msg_len);
	gb905_debug_header(header);

	offset += sizeof(gb905_header_t);
	DbgPrintf("offset = %d\r\n",offset);

	memcpy(&buf[offset],&pre_operation,sizeof(operation_body_t) - 4);
	offset += sizeof(operation_body_t) - 4;
	DbgPrintf("offset = %d\r\n",offset);
	
	memcpy(&buf[offset],meter_buf,meter_len);

	if(len != offset + meter_len + 2)
	{
		DbgError("operate len err!\r\n");
		DbgError("len = %d,offset = %d,meter_len = %d\r\n",len,offset,meter_len);
	}

	DbgFuncExit();
}


/**
* @brief 	不同阶段，填充营运数据的不同部分
*
*/
void gb905_build_pre_operation(int phase)
{
	DbgFuncEntry();

	DbgPrintf("phase = %d\r\n",phase);

	switch(phase)
	{
		case OPERATION_BEGIN:
			gb905_build_operation_start();
			break;

		case OPERATION_END:
			gb905_build_operation_end();
			break;

		case OPERATION_EVAL:
			gb905_build_operation_eval();
			break;

		default:
			DbgError("don't support this phase!\r\n");
			break;
	}

	DbgFuncExit();
}


/**
* @brief 	发送营运数据
*
*/
void gb905_send_operate(unsigned char * meter_buf,int meter_len)
{
	gb905_header_t * header;
	unsigned char * operation_buf;
	int operate_len;

	DbgFuncEntry();
	
	operate_len = sizeof(gb905_header_t) + 3;		// 3 : start magic + xor + end maigc
	operate_len += sizeof(operation_body_t) - 4;	// 4 : meter_content
	operate_len += meter_len;
	
	operation_buf = (unsigned char *)malloc(operate_len);
	if(!operation_buf)
	{
		DbgError("Get Params Memory Malloc Failed!\r\n");
		return;
	}
	memset((void*)operation_buf, 0, operate_len);

	gb905_build_operation(operation_buf,operate_len,meter_buf,meter_len);

	gb905_send_data(MAIN_SOCKET,operation_buf,operate_len);

	header = (gb905_header_t *)&operation_buf[1];
	//gb905_record_msg(MAIN_SOCKET,header,operation_buf,operate_len);
	
	header->msg_serial_number = EndianReverse16(header->msg_serial_number);
	//operation_insert_record((int)(header->msg_serial_number),operation_buf,operate_len);

	free(operation_buf);

	DbgFuncExit();
}



