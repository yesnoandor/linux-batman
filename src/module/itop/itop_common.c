/****************************************Copyright (c)**************************************************                         
** File name:			itop_common.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-10-30
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#include		"common.h"

#include		"middleware/socket/authorize_socket.h"
#include		"middleware/info/device.h"

#include		"module/itop/authorize/itop_authorize.h"
#include		"module/itop/itop_common.h"

#define			DEBUG_Y
#include		"libs/debug.h"


#define 		EXT_TYPE_MSG						0x0F
#define			EXT_HEADER_MAGIC_BYTE				0xF0F0

#define 		GATEWAY_MAIN_MESSAGE_TOTAL_NUM 		15
#define			GATEWAY_EXTEND_MESSAGE_TOTAL_NUM 	256

typedef union 
{

	unsigned char whole;
	
	struct
	{
		// low --> high
		unsigned char ext_header:1;			// =1,有扩展头	;=0,没有扩展头
		unsigned char ext_msg:1;			// =1,扩展消息	;=0,普通消息
    }msg_type;
}itop_msg_flag_t;

typedef struct{
	unsigned char * msg_buf;
	int msg_len;

	unsigned char msg_no;
	
	unsigned int mtd;			
	itop_msg_flag_t msg_flag;
}itop_msg_t,*p_itop_msg_t;

typedef unsigned char (*iTopTreatFunc)(unsigned char *msg_buf);

static iTopTreatFunc itop_protocol_main_treat[GATEWAY_MAIN_MESSAGE_TOTAL_NUM];
static iTopTreatFunc itop_protocol_ext_treat[GATEWAY_EXTEND_MESSAGE_TOTAL_NUM];


/*
* 解析出一条完整的消息
* msg ： 存放整条消息的参数
* header : 解析出协议的头信息
* return ：解析出协议，需要偏移的长度
*/
static unsigned short itop_get_a_full_msg(itop_msg_t *msg)
{
  	unsigned short offset = 0;
  	itop_ext_header_t * ext_header;
	itop_msg_header_t * msg_header;
	itop_ext_msg_header_t * ext_msg_header;

	DbgFuncEntry();

	if(msg->msg_len < 2)
	{
		DbgWarn("not enough chars(msg len = %d)\r\n",msg->msg_len);
		return offset;
	}

  	ext_header = (itop_ext_header_t *)(msg->msg_buf);
  	if(ext_header->head_F0F0 == EXT_HEADER_MAGIC_BYTE)
  	{
  		if(msg->msg_len < sizeof(itop_ext_header_t) + sizeof(itop_ext_msg_header_t))
		{
			DbgWarn("not enough chars(msg len = %d)\r\n",msg->msg_len);
			return offset;
		}

	
  		msg->msg_flag.msg_type.ext_header = 1;
  		offset += sizeof(itop_ext_header_t);
  		
  		msg_header = (itop_msg_header_t *)(msg->msg_buf + sizeof(itop_ext_header_t));
		if(msg_header->msg_type.msg_type.msg_id == TYPE_DN_EXTEND_MSG)
		{
			msg->msg_flag.msg_type.ext_msg = 1;
			ext_msg_header = (itop_ext_msg_header_t *)msg_header;
			
			msg->mtd = (ext_header->mtd_h << 16) | ext_msg_header->mtd_l;
			offset += ext_msg_header->msg_len + sizeof(itop_ext_msg_header_t) - 2;

			msg->msg_no = ext_msg_header->ext_msg_id;
		}
		else
		{
			msg->msg_flag.msg_type.ext_msg = 0;
			msg->mtd = (ext_header->mtd_h << 16) | msg_header->mtd_l;
			offset += msg_header->msg_len + sizeof(itop_msg_header_t) - 2;

			msg->msg_no = msg_header->msg_type.msg_type.msg_id;
		}
  	}
  	else
  	{
  		if(msg->msg_len < sizeof(itop_ext_msg_header_t))
		{
			DbgWarn("not enough chars(msg len = %d)\r\n",msg->msg_len);
			return offset;
		}
		
  		msg->msg_flag.msg_type.ext_header = 0;

  		msg_header = (itop_msg_header_t *)msg->msg_buf;
		if(msg_header->msg_type.msg_type.msg_id == TYPE_DN_EXTEND_MSG)
		{
			msg->msg_flag.msg_type.ext_msg = 1;
			ext_msg_header = (itop_ext_msg_header_t *)msg_header;
			msg->mtd = (ext_header->mtd_h << 16) | ext_msg_header->mtd_l;
			msg->msg_no = ext_msg_header->ext_msg_id;

			offset += ext_msg_header->msg_len + sizeof(itop_ext_msg_header_t) - 2;
		}
		else
		{
			msg->msg_flag.msg_type.ext_msg = 0;
			msg->mtd = (ext_header->mtd_h << 16) | msg_header->mtd_l;
			msg->msg_no = msg_header->msg_type.msg_type.msg_id;

			offset += msg_header->msg_len + sizeof(itop_msg_header_t) - 2;
		}
  	}


	if(offset > msg->msg_len)
	{
		offset = 0;
	}
	
	DbgPrintf("ext_msg = 0x%x\r\n",msg->msg_flag.msg_type.ext_msg);
	DbgPrintf("ext_header = 0x%x\r\n",msg->msg_flag.msg_type.ext_header);

	DbgPrintf("mtd = 0x%x\r\n",msg->mtd);
	DbgPrintf("msg_no = 0x%x\r\n",msg->msg_no);
	DbgPrintf("offset = 0x%x\r\n",offset);
  	
	DbgFuncExit();
	
	return offset;
}


/*
* 解析主消息
* @header ： 解析出协议的头信息
* @msg_buf ： 存放消息的数据buffer
*/
static void itop_parse_msg_main(unsigned char msg_no,unsigned char * msg_buf)
{
	if(msg_no && msg_no < GATEWAY_MAIN_MESSAGE_TOTAL_NUM)
	{
		if(itop_protocol_main_treat[msg_no])
		{
			(*itop_protocol_main_treat[msg_no])(msg_buf);
		}
	}
	
}

/*
* 解析扩展消息
* @header : 解析出协议的头信息
* @msg_buf： 存放消息的数据buffer
* 
*/
static void itop_parse_msg_ext(unsigned char msg_no,unsigned char * msg_buf)
{	
	if(msg_no && msg_no < GATEWAY_EXTEND_MESSAGE_TOTAL_NUM)
	{
		if(itop_protocol_ext_treat[msg_no])
		{
			(*itop_protocol_ext_treat[msg_no])(msg_buf);
		}
	}
}

/*
* 分析一条消息命令
* @msg： 存放完整消息命令的buf
* @header : 解析出协议的头信息
* @return：解析出协议，需要偏移的长度
*/
static void itop_parse_a_full_msg(itop_msg_t *msg)
{
	unsigned char * msg_data;
	unsigned int mtd_id;
	device_info_t info;

	DbgFuncEntry();

	mtd_id = msg->mtd;

	get_device_info(&info);
	
	if(info.mtd_id  && (info.mtd_id != mtd_id))
	{
		DbgWarn("");
        return;
	}

	if(msg->msg_flag.msg_type.ext_header)
	{
		msg_data = msg->msg_buf + sizeof(itop_ext_header_t);
	}
	else
	{
		msg_data = msg->msg_buf;
	}
	
	if(msg->msg_flag.msg_type.ext_msg)
	{
		itop_parse_msg_ext(msg->msg_no,msg_data);
	}
	else if(msg->msg_no == TYPE_DN_AUTH)
	{
		itop_authorize_analyze(mtd_id,msg_data);
	}
	else
	{
		itop_parse_msg_main(msg->msg_no,msg_data);
	}

	DbgFuncExit();
}

//----------
/*
* itop协议的解析
* @buf： 从socket 收到的循环buffer中，抽取当前接受到的数据，放入临时开的接受数据buffer
* @len : 临时开的接受数据buffer长度
* @return：返回当前解析掉的数据长度
*/
int itop_protocol_analyze(unsigned char * buf,int len)
{
    itop_msg_t msg;

	int offset;

	DbgFuncEntry();

	memset(&msg,0x00,sizeof(itop_msg_t));
	
	msg.msg_buf = buf;
	msg.msg_len = len;
	
	do{
		offset = itop_get_a_full_msg(&msg);
		if(offset)
		{
			itop_parse_a_full_msg(&msg);
			msg.msg_buf += offset;
			msg.msg_len -= offset;
		}
	}while(msg.msg_len);

	DbgFuncExit();
	
	return (len - msg.msg_len);
}

/*
* itop 协议的命令请求
* @buf： 
* @len : 
* @return：返回当前解析掉的数据长度
*/
void itop_protocol_request(unsigned char * buf, int len)
{
	itop_ext_header_t * ext_header;
	
	ext_header = (itop_ext_header_t *)buf;
	ext_header->head_F0F0 = 0xF0F0;
	ext_header->series_NO = 0;
	ext_header->mtd_h = 0;
	
	authorize_socket_send(buf,len);
}

