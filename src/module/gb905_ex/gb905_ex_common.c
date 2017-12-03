/****************************************Copyright (c)**************************************************                         
** File name:			gb905_ex_common.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-28
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

#include	"misc/bcd.h"
#include	"misc/check.h"
#include	"misc/endian.h"

#include	"module/gb905_ex/gb905_ex_common.h"

#include	"middleware/socket/fleety_socket.h"
#include	"middleware/uart/fleety_uart.h"

#define		DEBUG_Y
#include	"libs/debug.h"



//#include	"fleety_misc.h"
//#include	"fleety_hal.h"

// 转义规则 ： 0x7D ==> 0x7D,0x01;	0x7E ==> 0x7D,0x02
#define		ESCAPE_CHAR					0x7D
#define		MAGIC_CHAR					0x7E

#define		MAX_PROTOCOL_BUFF_SIZE		512


/** 
* @brief 	转义消息
* @param dest		存放转义以后的消息缓存
* @param src 	存放转义以前的消息缓存
* @param len 		转义以前的消息长度
* 
* @return			转义以后的消息长度 （长度会增加）
*/
static int gb905_ex_escape(unsigned char *dest, unsigned char *src, int len)
{
	int i,j;
	
	if(!dest || !src || !len)
	{
		DbgError("params error!\r\n");
		return -1;
	}
	
	for(i=0,j=0; i<len; i++)
	{
		if(src[i] == ESCAPE_CHAR)
		{
			dest[j++] = ESCAPE_CHAR;
			dest[j++] = 0x01;
		}
		else if(src[i] == MAGIC_CHAR)
		{
			dest[j++] = ESCAPE_CHAR;
			dest[j++] = 0x02;
		}
		else
		{
			dest[j++]=src[i];
		}
	}
	
	return j;
}



/** 
* @brief 	解析转义消息
* @param dest		存放解析以后的消息缓存
* @param src 	存放解析以前的消息缓存
* @param len 		存放解析以前的消息长度
* 
* @return			存放解析以后的消息长度 （长度会减少）
*/
static int gb905_ex_unescape(unsigned char *dest, unsigned char *src, int len)
{
	int i,j;

	if( !dest || !src || !len )
	{
		DbgError("params error!\r\n");
		return -1;
	}
	
	for(i=0,j=0; i<len; i++)
	{
		if(src[i]== ESCAPE_CHAR)
		{
			i++;
			if(i == len)
			{
				DbgPrintf("not unescape number error!\r\n");
				return -1;
			}
			else if(src[i] == 0x01)
			{
				dest[j++] = ESCAPE_CHAR;
			}
			else if(src[i]==0x02)
			{
				dest[j++] = MAGIC_CHAR;
			}
			else
			{
				DbgPrintf("unescape number error!\r\n");
				return -1;
			}
		}
		else
		{
			dest[j++]=src[i];
		}
	}
	
	return j;
}


#if 0

/** 
* @brief 	GB905 EX  协议服务器端对客户端的通用应答的解析
* @param buf	收到应答的缓冲地址
* @param len	收到应答的缓冲长度
*
*/
static void gb905_ex_common_ack_treat(unsigned char *buf,int len)
{
	gb905_ex_ack_body_t * ack_body;

	ack_body = (gb905_ex_ack_body_t *)buf;
	
	gb905_ex_debug_ack(ack_body);

	if(len != sizeof(gb905_ex_ack_body_t) || ack_body->result != GB905_EX_RESULT_OK)
	{
		DbgWarn("gb905 ex common ack (server --> terminal) faile!(id = 0x%x)\r\n",ack_body->id);
	}
	else
	{
		DbgGood("gb905 ex common ack (server --> terminal) ok!(id = 0x%x)\r\n",ack_body->id);
	}
	
	gb905_ex_debug_ack(ack_body);

}
#endif



/** 
* @brief 	查找GB905  ex  的协议魔术字
* @param buf 收到数据的缓冲地址
* @param len 收到数据的缓冲长度
* @param offset		查找到魔术字的偏移地址
*
* @return	是否查找到
*		=true, 查找到
*		=false, 没有查找到
*/

static bool gb905_ex_find_magic_char(unsigned char *buf, int len,unsigned int * offset)
{
	int index=0;
	bool ret = false;

	*offset = 0;
	
	while(index < len)
	{
		if(buf[index] == MAGIC_CHAR)
		{
			ret = true;
			*offset = index;
			break;
		}
		
		index++;
	}

	return ret;
}

/*
 * 解析出一条完整的消息
 * msg ： 存放整条消息的参数
 * header : 解析出协议的头信息
 * return ：解析出协议，需要偏移的长度
 */
static int gb905_ex_get_a_full_msg(unsigned char *buf,int len,buff_mgr_t *msg)
{
	bool ret;
	unsigned int head_offset = 0,tail_offset = 0;
	int offset;

	DbgFuncEntry();

  	ret = gb905_ex_find_magic_char(buf,len,&head_offset);
	if(ret && (len - head_offset >= sizeof(gb905_ex_header_t) + 3))		// 3 : start magic + xor  + end maigc
	{
		ret = gb905_ex_find_magic_char(buf + head_offset + 1,len - head_offset - 1,&tail_offset);
		tail_offset += head_offset + 1;
		if(ret)
		{
			DbgPrintf("head_offset = %d\r\n",head_offset);
			DbgPrintf("tail_offset = %d\r\n",tail_offset);
			if((tail_offset - head_offset + 1)>= sizeof(gb905_ex_header_t) + 3)
			{
				msg->buf = (buf + head_offset);
				msg->len = tail_offset - head_offset + 1;
				offset = tail_offset + 1;
			}
			else
			{
				msg->len = 0;
			 	offset = tail_offset;
			}
		}
		else
		{
			offset = head_offset;
		}
	}
	else
	{
		 offset = head_offset;
	}
 
	DbgPrintf("offset = %d\r\n",offset);
	 
	DbgFuncExit();
	 
	return offset;  
}
 
/** 
 * @brief	 GB905	协议的消息体具体解析
 * @param msg		 存放消息的缓存数据结构
 * 
 * @return			 解析是否成功
 */
static bool gb905_ex_parse_header(buff_mgr_t * msg)
{
	bool ret = true;
	gb905_ex_header_t * header;

	DbgFuncEntry();

	header = (gb905_ex_header_t *)(msg->buf + 1);

	DbgPrintf("msg_id = 0x%x\r\n",header->msg_id);
	DbgPrintf("msg_len = 0x%x\r\n",header->msg_len);
	DbgPrintf("msg_serial_number = 0x%x\r\n",header->msg_serial_number);
	
	if(header->msg_len != msg->len - sizeof(gb905_ex_header_t) - 3)
	{
		ret = false;

		DbgError("msg len is invaild!\r\n");
		return ret;
	}
	
	DbgFuncExit();
	
	return ret;
}

static bool gb905_ex_parse_protocol(gb905_ex_funcs_t * funcs,buff_mgr_t * msg)
{
	bool ret;
	
	DbgFuncEntry();

	if(funcs->p_parse_protocol != NULL)
		ret = funcs->p_parse_protocol(msg);

	DbgFuncExit();
	
	return ret;
}

/** 
* @brief 	解析出一条完整的消息
* @param msg 	buff  管理指针
*
* @return	解析是否成功
*/
static int gb905_ex_parse_a_full_msg(gb905_ex_funcs_t * funcs,buff_mgr_t * msg)
{
	int ret;

	DbgFuncEntry();

	ret = gb905_ex_parse_header(msg);
	if( ret < 0)
	{
		return ret;
	}

	ret = gb905_ex_parse_protocol(funcs,msg);

	
	DbgFuncExit();

	return ret;
}

/** 
* @brief 	构造GB905  ex  的终端ACK  的消息体
* @param ack	 	GB905  ack  消息体的地址
* @param header	GB905  ack  对应的header 数据结构的地址
* @param result  	返回服务器的状态
*
*/ 
static void gb905_ex_build_ack_body(gb905_ex_ack_body_t * ack_body,gb905_ex_header_t * header,unsigned char result)
{
	ack_body->seq = header->msg_serial_number;
	ack_body->id = header->msg_id;
	ack_body->result = result;
}


//----- 

/** 
* @brief 	GB905  ex  协议的解析
* @param buf		存放解析以后的消息缓存
* @param len  		存放解析以前的消息长度
* 
* @return			存放解析以后的消息长度 （长度会减少）
*/
int gb905_ex_protocol_ayalyze(gb905_ex_funcs_t * funcs,unsigned char * buf,int len)
{
	unsigned char * msg_buf;
	int msg_len;
	 
	int offset;
	double_buff_mgr_t msg;
  
	msg_buf = buf;
	msg_len = len;
	  
	do{
		memset(&msg,0x00,sizeof(msg));
 
		offset = gb905_ex_get_a_full_msg(msg_buf,msg_len,&msg.raw);
 
 
		#if 0
		{
			 int i;
				  
			 debug_printf("offset = %d\r\n",offset);
			 debug_printf("raw_msg_lens = %d\r\n",msg.raw.len);
			 
			 for(i=0;i<protocol_msg.raw.len;i++)
			 {
				 debug_printf("msg_raw_buf[%d] = 0x%x\r\n",i,raw.buf[i]);
			 }
		}
		#endif
  
		if(offset && (msg.raw.len > sizeof(gb905_ex_header_t) + 3))
		{
			msg.now.buf = (unsigned char *)malloc(msg.raw.len);
			if(!msg.now.buf)
			{
				DbgError("protocol memory malloc failed!\r\n");
				return -1;
			}
 
			msg.now.len = gb905_ex_unescape(msg.now.buf,msg.raw.buf,msg.raw.len);
  
			#if 0
			{
				int i;
				  
				DbgPrintf("msg_len = %d\r\n",msg.msg_len);
				for(i=0;i<msg.msg_len;i++)
				{
					DbgPrintf("msg_buf[%d] = 0x%2x\r\n",i,msg.msg_buf[i]);
				}
			}
			#endif
			  
			if(msg.now.buf[msg.now.len - 2] != xor8_computer(msg.now.buf + 1,msg.now.len - 3))
			{
				  DbgPrintf("xor verify failed!\r\n");
			}
			else
			{
				gb905_ex_parse_a_full_msg(funcs,&msg.now);
			}
 
			free(msg.now.buf);
		}
 
		 
		msg_buf += offset;
		msg_len -= offset;
 
		DbgPrintf("offset = %d\r\n",offset);
		DbgPrintf("raw.len = %d\r\n",msg.raw.len);
		DbgPrintf("msg_len = %d\r\n",msg_len); 
	}while(offset && msg.raw.len && msg_len > 0);
 
	DbgFuncExit();
	  
	return (len - msg_len);
}
  
/** 
* @brief	  构造GB905  ex  的头
* @param header	  GB905 头的地址
* @param msg_id	  消息命令ID
* @param msn_len	  消息体的长度
*
*/
void gb905_ex_build_header(gb905_ex_header_t * header, unsigned short msg_id, unsigned short msg_len)
{
	static  unsigned int msg_seq_number = 0;
 
	DbgFuncEntry();
 	 
	memset(header,0x00,sizeof(gb905_ex_header_t));
	header->msg_id = msg_id;
	header->msg_len = msg_len;

	header->msg_serial_number = msg_seq_number++;
 
	DbgFuncExit();
}
 


/** 
* @brief 	构造GB905  ex  的终端整个ACK   数据结构
* @param ack	 	GB905  ex ack  应答数据结构的地址
* @param header	GB905  ex ack  对应的header 数据结构的地址
* @param result  	返回服务器的状态
*
*/
void gb905_ex_build_ack(gb905_ex_ack_t * ack,unsigned short msg_id,gb905_ex_header_t * header,unsigned char result)
{
	gb905_ex_build_header(&ack->header,msg_id,sizeof(gb905_ex_ack_body_t));
	gb905_ex_build_ack_body(&ack->ack_body,header,result);
}


/** 
* @brief 	客户端往服务器端发送数据(增加转义，魔术字和校验的处理)
* @param header	GB905  ex ack  应答数据结构的地址
* @param result  	返回服务器的状态
*
*/
void gb905_ex_send_data(unsigned char domain,unsigned char index,unsigned char * buf, int len)
{
	int i;
	unsigned char * new_buf;
	int new_len;
	unsigned char val;
	
	buf[0] = buf[len-1] = MAGIC_CHAR;

	val = xor8_computer((unsigned char *)&buf[1],len - 3);
	buf[len-2] = val;

	new_buf = (unsigned char *)malloc(len * 2);
	if(!new_buf)
	{
		DbgError("protocol memory malloc failed!\r\n");
		return;
	}

	new_buf[0] = buf[0];
	new_len = gb905_ex_escape(&new_buf[1],&buf[1],len-2);
	new_len++;
	new_buf[new_len] = buf[len-1];
	new_len++;

	DbgPrintf("new_len = 0x%x\r\n",new_len);
		
	for(i=0;i<new_len;i++)
	{
		DbgPrintf("new_buf[%d] = 0x%x\r\n",i,new_buf[i]);
	}

	switch (domain)
	{
		case SOCKET_DOMAIN:
			fleety_socket_send(index,new_buf,new_len);
			break;

		case UART_DOMAIN:
			fleety_uart_send(index,new_buf,new_len);
			break;

		default:
			DbgError("domain is err(%d)\r\n",domain);
			break;
	}
	
	free(new_buf);
}

#if 0
/** 
* @brief 	客户端发送往服务器端的应答
* @param header	GB905  ex ack  应答数据结构的地址
* @param result  	返回服务器的状态
*
*/
void gb905_ex_send_ack(gb905_ex_header_t * header,unsigned char result)
{
	gb905_ex_ack_t ack;

	DbgFuncEntry();

	gb905_ex_build_ack(&ack,header,result);
	
	//gb905_ex_build_header(&ack.header,SYSTEM2MCU_GENERAL_ACK,sizeof(gb905_ex_ack_body_t));
	//gb905_ex_build_ack_body(&ack.ack_body,header,result);
	gb905_ex_send_data(MCU_UART,(unsigned char *)&ack,sizeof(gb905_ex_ack_t));

	DbgFuncExit();
}
#endif

//-----------------------------------------------------------
void gb905_ex_debug_header(gb905_ex_header_t * header)
{
	DbgFuncEntry();
 
	DbgPrintf("msg_id = 0x%x\r\n",header->msg_id);
	DbgPrintf("msg_len = 0x%x\r\n",header->msg_len);
	DbgPrintf("msg_serial_number = 0x%x\r\n",header->msg_serial_number);
	 
	DbgFuncExit();
}
 
void gb905_ex_debug_ack(gb905_ex_ack_body_t * ack_body)
{
	DbgFuncEntry();
 
	DbgPrintf("seq = 0x%x\r\n",ack_body->seq);
	DbgPrintf("id = 0x%x\r\n",ack_body->id);
	DbgPrintf("result = 0x%x\r\n",ack_body->result);
	 
	DbgFuncExit();
}

//-----
gb905_ex_funcs_t * gb905_ex_base_create(unsigned char domain,unsigned char index,p_parse_protocol_t callback)
{
	gb905_ex_funcs_t * funcs = malloc(sizeof(gb905_ex_funcs_t));

	DbgFuncEntry();
	
	funcs->p_analyze_func = gb905_ex_protocol_ayalyze;
	funcs->p_build_header_func =  gb905_ex_build_header;
	funcs->p_build_ack_func = gb905_ex_build_ack;
	funcs->p_send_data_func = gb905_ex_send_data;

	funcs->p_parse_protocol = callback;

	funcs->domain = domain;
	funcs->index = index;

	DbgFuncExit();
	
	return funcs;
}

void gb905_ex_base_release(gb905_ex_funcs_t *funcs)
{
	DbgFuncEntry();

	funcs->p_analyze_func = NULL;
	funcs->p_build_header_func = NULL;
	funcs->p_build_ack_func = NULL;
	funcs->p_send_data_func = NULL; 

	funcs->p_parse_protocol = NULL;

	funcs->domain = 0;
	free(funcs);

	DbgFuncExit();
}

