/****************************************Copyright (c)**************************************************                         
** File name:			gb905_common.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-04
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

#include	"middleware/socket/fleety_socket.h"
#include	"middleware/info/device.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905/report/gb905_trace.h"
#include	"module/gb905/params/gb905_params.h"
#include	"module/gb905/ctrl/gb905_control.h"
#include	"module/gb905/notice/gb905_notice.h"
#include	"module/gb905/event/gb905_event.h"
#include	"module/gb905/question/gb905_question.h"
#include	"module/gb905/phone_book/gb905_phone_book.h"
#include	"module/gb905/heart_beat/gb905_heart_beat.h"
#include	"module/gb905/transparent/gb905_transparent.h"
#include	"module/gb905/callback/gb905_callback.h"
#include	"module/gb905/order/gb905_order.h"




#define		DEBUG_Y
#include	"libs/debug.h"


// 转义规则 ： 0x7D ==> 0x7D,0x01;	0x7E ==> 0x7D,0x02
#define		ESCAPE_CHAR			0x7D
#define		MAGIC_CHAR			0x7E

#define		TEMINAL_ID			0x10


/** 
   * @brief 	转义消息
   * @param dest		存放转义以后的消息缓存
   * @param src		存放转义以前的消息缓存
   * @param len  		转义以前的消息长度
   * 
   * @return			转义以后的消息长度 （长度会增加）
   */
static int gb905_escape(unsigned char *dest, unsigned char *src, int len)
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
   * @param src		存放解析以前的消息缓存
   * @param len  		存放解析以前的消息长度
   * 
   * @return			存放解析以后的消息长度 （长度会减少）
   */
static int gb905_unescape(unsigned char *dest, unsigned char *src, int len)
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
				DbgError("not unescape number error!\r\n");
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
				DbgError("unescape number error!\r\n");
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

/** 
   * @brief 	服务器对终端的通用应答的解析
   * @param buf	收到应答的缓冲地址
   * @param len	收到应答的缓冲长度
   *
   */
static void gb905_common_ack_treat(unsigned char *buf,int len)
{
	ack_body_t * ack_body;

	DbgFuncEntry();
	
	ack_body = (ack_body_t *)buf;
	
	ack_body->seq = EndianReverse16(ack_body->seq);
	ack_body->id = EndianReverse16(ack_body->id);

	if(len != sizeof(ack_body_t) || ack_body->result != GB905_RESULT_OK)
	{
		DbgWarn("common ack (server --> terminal) faile!(id = 0x%x)\r\n",ack_body->id);
	}
	else
	{
		DbgGood("common ack (server --> terminal) ok!(id = 0x%x)\r\n",ack_body->id);

		switch (ack_body->id)
		{
			case MESSAGE_HEART_BEAT:
				gb905_heart_beat_reset();
				break;
		
			default:
				break;
		}

	}

	gb905_debug_ack(ack_body);

	DbgFuncExit();
}


/** 
   * @brief 	查找GB905  的协议魔术字
   * @param buf	收到数据的缓冲地址
   * @param len	收到数据的缓冲长度
   * @param offset  	查找到魔术字的偏移地址
   *
   * @return	是否查找到
   *		=true, 查找到
   *		=false, 没有查找到
   */
static bool gb905_find_magic_char(unsigned char *buf, int len,unsigned int * offset)
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

/** 
   * @brief 	解析出一条完整的消息
   * @param buf	GB905 头的地址
   * @param len	消息命令ID
   * @param msg  	消息体的长度
   *
   * @return	
   */
static int gb905_get_a_full_msg(unsigned char *buf,int len,buff_mgr_t *msg)
{
	bool ret;
	unsigned int head_offset = 0,tail_offset = 0;
	int offset;

	DbgFuncEntry();
	
	ret = gb905_find_magic_char(buf,len,&head_offset);
	if(ret && (len - head_offset >= sizeof(gb905_header_t) + 3))
	{
		ret = gb905_find_magic_char(buf + head_offset + 1,len - head_offset - 1,&tail_offset);
		tail_offset += head_offset + 1;
		if(ret)
		{
			DbgPrintf("head_offset = %d\r\n",head_offset);
			DbgPrintf("tail_offset = %d\r\n",tail_offset);
			
			msg->buf = (buf + head_offset);
			msg->len = tail_offset - head_offset + 1;
			offset = tail_offset + 1;
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
* @brief 	GB905  协议的消息体具体解析
* @param msg		存放消息的缓存数据结构
* 
* @return			解析是否成功
*/
static bool gb905_parse_header(buff_mgr_t * msg)
{
	bool ret = true;
	unsigned int mtd_id;
	gb905_header_t * header;
	device_info_t info;

	DbgFuncEntry();

	header = (gb905_header_t *)(msg->buf + 1);

	gb905_debug_header(header);

	header->msg_id = EndianReverse16(header->msg_id);
	header->msg_len = EndianReverse16(header->msg_len);
	header->msg_serial_number = EndianReverse16(header->msg_serial_number);

	DbgPrintf("msg_id = 0x%x\r\n",header->msg_id);
	DbgPrintf("msg_len = 0x%x\r\n",header->msg_len);
	DbgPrintf("msg_serial_number = 0x%x\r\n",header->msg_serial_number);
	
	if( header->msg_len != msg->len - sizeof(gb905_header_t) - 3)
	{
		ret = false;

		DbgError("msg len is invaild!\r\n");
		return ret;
	}
	
	if( header->msg_vendor_type != TEMINAL_ID )
	{
		ret = false;

		DbgError("ternimal id is error!\r\n");
		return ret;
	}
	
	if( !is_bcd((unsigned char *)(&header->uuid_device_id), sizeof(uuid_device_id_t)) )
	{
		DbgError("bcd uuid is invaild!\r\n");
		
		ret = false;
		return ret;
	}

	mtd_id = bcd2decimal((unsigned char *)(&header->uuid_device_id.mtd_id),sizeof(header->uuid_device_id.mtd_id));
	get_device_info(&info);

	DbgPrintf("mtd_id = %d \r\n",mtd_id);
	DbgPrintf("info.mtd_id = %d \r\n",info.mtd_id);
	
	if(mtd_id != info.mtd_id)
	{
		DbgError("mismatch mtd id!\r\n");

		ret = false;
		return ret;
	}

	DbgFuncExit();
	
	return ret;
}


/** 
   * @brief 			GB905  协议的消息体具体解析
   * @param 	msg		存放消息的缓存数据结构
   * 
   * @return			解析是否成功
   */
static bool gb905_parse_protocol(buff_mgr_t * msg)
{
	gb905_header_t * header;
	unsigned short msg_no;
	unsigned char result;

	DbgFuncEntry();

	header = (gb905_header_t *)(msg->buf + 1);

	msg_no = header->msg_id;

	DbgPrintf("msg_no = 0x%x\r\n",msg_no);

	switch (msg_no)
	{
		case MESSAGE_GENERAL_DOWN_ACK:
			gb905_common_ack_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);
			break;

		case MESSAGE_GET_LOCATION:
			gb905_report_ack_send(EndianReverse16(header->msg_serial_number));
			break;
			
		case MESSAGE_TRACE_LOCATION:
			result = gb905_trace_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);
			gb905_send_ack(header,result);
			break;

		case MESSAGE_SET_PARAMS:
			result = gb905_set_params_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);		
			gb905_send_ack(header,result);
			break;

		case MESSAGE_GET_PARAMS:
			gb905_get_params_treat(msg->buf,msg->len);
			break;

		case MESSAGE_ISU_CTRL:
			result = gb905_control_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);		
			gb905_send_ack(header,result);
			break;

		case MESSAGE_TEXT_NOTICE:
			result = gb905_notice_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);
			gb905_send_ack(header,result);
			break;

		case MESSAGE_EVENT_LIST:
			result = gb905_set_event_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);
			gb905_send_ack(header,result);

            //gb905_report_event_treat(5);		//demo test report_event
			break;

		case MESSAGE_QUESTION:
			result = gb905_question_issue_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);
			gb905_send_ack(header,result);

            //gb905_question_ack_treat(12,3);		//demo test question_ack
            break;

		case MESSAGE_PHONE_BOOK:
			result = gb905_set_phone_book_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);
			gb905_send_ack(header,result);
			break;
		
		case MESSAGE_CALLBACK:
			result = gb905_callback_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);
			gb905_send_ack(header,result);
			break;

		case MESSAGE_PERIPHERY_DOWN:
			gb905_transparent_download_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);
			break;
		
		case MESSAGE_ORDER_INFO:
			result = gb905_order_briefing_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);
			gb905_send_ack(header, result);
			break;
			
		case MESSAGE_ORDER_GRAB_ACK:
			result = gb905_order_detials_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);
			gb905_send_ack(header, result);
			break;
		
		case MESSAGE_ORDER_CANCEL_DOWN:
			result = gb905_order_cancel_treat(msg->buf + 1 + sizeof(gb905_header_t),header->msg_len);
			gb905_send_ack(header, result);
			break;

			
		#if 0
		case MESSAGE_REMOTE_CTRL:
			result = gb905_remote_control_treat(msg->msg_buf + 1 + sizeof(gb905_msg_header_t),header->msg_len);
			gb905_send_ack(header,result);
			gb905_remote_control_ack(EndianReverse16(header->msg_serial_number));
			break;
		
		case MESSAGE_PICTURE_CTRL:
			result = gb905_picture_ctrl_treat(msg->msg_buf + 1 + sizeof(gb905_msg_header_t),header->msg_len,header->msg_serial_number);
			gb905_send_ack(header,result);
			break;


		case MESSAGE_PICTURE_SEARCH:
			gb905_send_ack(header,GB905_RESULT_OK);
			gb905_picture_search_treat(msg->msg_buf + 1 + sizeof(gb905_msg_header_t),header->msg_len,header->msg_serial_number);
			break;

		case MESSAGE_RECORD_SEARCH:
			gb905_send_ack(header,GB905_RESULT_OK);
			gb905_audio_search_treat(msg->msg_buf + 1 + sizeof(gb905_msg_header_t),header->msg_len,header->msg_serial_number);
			break;
			
		case MESSAGE_AV_UPLOAD_REQUEST:
			gb905_send_ack(header,GB905_RESULT_OK);
			gb905_av_upload_request_treat(msg->msg_buf + 1 + sizeof(gb905_msg_header_t),header->msg_len);
			break;
				
		case MESSAGE_GET_LOCATION:
			gb905_send_report_ack(EndianReverse16(header->msg_serial_number));
			break;

		case MESSAGE_TRACE_LOCATION:
			result = gb905_location_trace_treat(msg->msg_buf + 1 + sizeof(gb905_msg_header_t),header->msg_len);
			gb905_send_ack(header,result);
			break;






		case MESSAGE_CONFIRM_ALARM:
			result = gb905_confirm_alarm_treat();
			gb905_send_ack(header, result);
			break;
			
		case MESSAGE_CANCEL_ALARM:
			result = gb905_cancel_alarm_treat();
			gb905_send_ack(header, result);
			break;


		case MESSAGE_DEVICE_INSPECTION:
			result = gb905_device_inspection_treat(msg->msg_buf + 1 + sizeof(gb905_msg_header_t),header->msg_len);
			break;
			
		case MESSAGE_OBD_GET_HBT_REPORT:
			gb905_send_obd_hbt_report();
			break;

		case MESSAGE_OBD_SET_PARAMS:
			result = gb905_set_obd_params_treat(msg->msg_buf + 1 + sizeof(gb905_msg_header_t),header->msg_len);		
			gb905_send_ack(header,result);
			break;

		case MESSAGE_OBD_GET_PARAMS:
			gb905_get_obd_params_treat(msg->msg_buf,msg->msg_len);
			break;

		case MESSAGE_OBD_TRANSPARENT:
			gb905_obd_transparent_treat(msg->msg_buf + 1 + sizeof(gb905_msg_header_t),header->msg_len);
			break;
		#endif
		
		default:
			break;
	}
		
	DbgFuncExit();
	
	return true;
}



/** 
   * @brief 	解析出一条完整的消息
   * @param msg		buff  管理指针
   *
   * @return	解析是否成功
   */
static bool gb905_parse_a_full_msg(buff_mgr_t *msg)
{
	int ret;

	DbgFuncEntry();

	ret = gb905_parse_header(msg);
	if(ret < 0)
	{
		DbgError("\r\n");
		return ret;
	}

	ret = gb905_parse_protocol(msg);

	DbgFuncExit();

	return ret;
}

/** 
* @brief 	构造GB905  的终端ACK  的消息体
* @param ack	 	GB905  ack  消息体的地址
* @param header	GB905  ack  对应的header 数据结构的地址
* @param result  	返回服务器的状态
*
*/
static void gb905_build_ack_body(ack_body_t * ack_body,gb905_header_t * header,unsigned char result)
{
	ack_body->seq = EndianReverse16(header->msg_serial_number);
	ack_body->id = EndianReverse16(header->msg_id);
	ack_body->result = result;
}

//----------
/*
* itop协议的解析
* @buf： 从socket收到的循环buffer中，抽取当前接受到的数据，放入临时开的接受数据buffer
* @len : 临时开的接受数据buffer长度
* @return：返回当前解析掉的数据长度
*/

/** 
   * @brief 	GB905  协议的解析
   * @param buf		存放解析以后的消息缓存
   * @param len  		存放解析以前的消息长度
   * 
   * @return			存放解析以后的消息长度 （长度会减少）
   */
int gb905_protocol_ayalyze(unsigned char * buf,int len)
{
	unsigned char * msg_buf;
	int msg_len;
	int offset;
	double_buff_mgr_t msg;

	DbgFuncEntry();

	msg_buf = buf;
	msg_len = len;
	
	do{
		memset(&msg,0x00,sizeof(msg));
		
		offset = gb905_get_a_full_msg(msg_buf,msg_len,&msg.raw);

		#if 0
		{
			int i;
				
			DbgPrintf("offset = %d\r\n",offset);
			DbgPrintf("raw_msg_lens = %d\r\n",msg.raw_msg_len);
			
			for(i=0;i<msg.raw_msg_len;i++)
			{
				DbgPrintf("msg_raw_buf[%d] = 0x%2x\r\n",i,msg.raw_msg_buf[i]);
			}
		}
		#endif

		if(offset && msg.raw.len)
		{	
			msg.now.buf = malloc(msg.raw.len);
			if(!msg.now.buf)
			{
				DbgError("gb905 Memory Malloc Failed!\r\n");
				return -1;
			}

			msg.now.len = gb905_unescape(msg.now.buf,msg.raw.buf,msg.raw.len);

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
				DbgError("xor verify failed!\r\n");
			}
			else
			{
				gb905_parse_a_full_msg(&msg.now);
			}

			free(msg.now.buf);
		}

		msg_buf += offset;
		msg_len -= offset;
	}while(offset && msg.raw.len && msg_len > 0);
	
	DbgFuncExit();
	
	return (len - msg_len);

}


/** 
* @brief 	构造GB905  的头
* @param header	 	GB905 头的地址
* @param msg_id		消息命令ID
* @param msn_len  	消息体的长度
*
*/
void gb905_build_header(gb905_header_t * header, unsigned short msg_id, unsigned short msg_len)
{
	device_info_t info;
	unsigned int mtd_id;
	static 	unsigned int msg_seq_number = 0;

	DbgFuncEntry();

	get_device_info(&info);
	
	memset(header,0x00,sizeof(gb905_header_t));
	header->msg_id = EndianReverse16(msg_id);
	header->msg_len = EndianReverse16(msg_len);

	header->msg_vendor_type = 0x10;
	header->uuid_device_id.vendor_id = GB905_FLEETY_VENDOR_ID;
	header->uuid_device_id.product_id = 0x00;

	mtd_id = info.mtd_id % 1000000;
	decimal2bcd(mtd_id,header->uuid_device_id.mtd_id,3);

	header->msg_serial_number = EndianReverse16(msg_seq_number);
	msg_seq_number++;

	DbgFuncExit();
}


/** 
* @brief 	构造GB905  的终端整个ACK   数据结构
* @param ack	 	GB905  ack  应答数据结构的地址
* @param header	GB905  ack  对应的header 数据结构的地址
* @param result  	返回服务器的状态
*
*/
void gb905_build_ack(gb905_ack_t * ack,gb905_header_t * header,unsigned char result)
{
	gb905_build_header(&ack->header,MESSAGE_GENERAL_UP_ACK,sizeof(ack_body_t));
	gb905_build_ack_body(&ack->ack_body,header,result);
}


	


/** 
* @brief 	构造GB905  的时间格式(BCD[6], YY-MM-DD-hh-mm-ss)
* @param timestamp	 	GB905  时间格式数据的地址
*
*/
void gb905_build_timestamp(gb905_timestamp_t * timestamp)
{
	time_t timep;
	struct tm *p;

	DbgFuncEntry();
	
	time(&timep);
	p = localtime(&timep);

	decimal2bcd(p->tm_year - 100,&timestamp->bcd[0],1);
	decimal2bcd(p->tm_mon+1,&timestamp->bcd[1],1);
	decimal2bcd(p->tm_mday,&timestamp->bcd[2],1);
	decimal2bcd(p->tm_hour,&timestamp->bcd[3],1);
	decimal2bcd(p->tm_min,&timestamp->bcd[4],1);
	decimal2bcd(p->tm_sec,&timestamp->bcd[5],1);

	DbgFuncExit();
}

/** 
* @brief 	构造GB905  的时间格式(UINT32)
*
* @return		表示时间戳的整型数据
*/
unsigned int gb905_build_timestamp_id(void)
{
	time_t timep;
	struct tm *p;
	gb905_timestamp_id_t timestamp_id;
	
	time(&timep);
	p = localtime(&timep);

	timestamp_id.timestamp.sec = p->tm_sec;
	timestamp_id.timestamp.min = p->tm_min;
	timestamp_id.timestamp.hour = p->tm_hour;
	timestamp_id.timestamp.mday = p->tm_mday;
	timestamp_id.timestamp.mon = p->tm_mon + 1;
	timestamp_id.timestamp.year = p->tm_year - 100;

	return timestamp_id.id; 
}

void gb905_send_data(unsigned char socket_index,unsigned char * buf, int len)
{
	unsigned char * new_buf;
	int new_len;
	unsigned char val;
	
	buf[0] = buf[len-1] = MAGIC_CHAR;

	val = xor8_computer((unsigned char *)&buf[1],len - 3);
	buf[len-2] = val;

	new_buf = (unsigned char *)malloc(len * 2);
	if(!new_buf)
	{
		DbgError("gb905 Memory Malloc Failed!\r\n");
		return;
	}

	new_buf[0] = buf[0];
	new_len = gb905_escape(&new_buf[1],&buf[1],len-2);
	new_len++;
	new_buf[new_len] = buf[len-1];
	new_len++;

	#if 0
	{
		int i;

		DbgPrintf("new_len = 0x%x\r\n",new_len);
		
		for(i=0;i<new_len;i++)
		{
			DbgPrintf("new_buf[%d] = 0x%x\r\n",i,new_buf[i]);
		}
	}
	#endif
	
	fleety_socket_send(socket_index,new_buf,new_len);

	free(new_buf);
}

void gb905_send_ack(gb905_header_t * header,unsigned char result)
{
	gb905_ack_t ack;

	DbgFuncEntry();

	gb905_build_ack(&ack,header,result);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&ack,sizeof(gb905_ack_t));
	
	DbgFuncExit();
}

//-----
void gb905_debug_header(gb905_header_t * header)
{
	DbgFuncEntry();

	DbgPrintf("msg_id = 0x%x\r\n",header->msg_id);
	DbgPrintf("msg_len = 0x%x\r\n",header->msg_len);
	
	DbgPrintf("msg_vendor_type = 0x%02x\r\n",header->msg_vendor_type);
	DbgPrintf("vendor_id = 0x%02x\r\n",header->uuid_device_id.vendor_id);
	DbgPrintf("product_id = 0x%02x\r\n",header->uuid_device_id.product_id);
	DbgPrintf("mtd_id[0] = 0x%02x\r\n",header->uuid_device_id.mtd_id[0]);
	DbgPrintf("mtd_id[1] = 0x%02x\r\n",header->uuid_device_id.mtd_id[1]);
	DbgPrintf("mtd_id[2] = 0x%02x\r\n",header->uuid_device_id.mtd_id[2]);
	DbgPrintf("msg_serial_number = %d\r\n",header->msg_serial_number);
	
	DbgFuncExit();
}

void gb905_debug_ack(ack_body_t * ack_body)
{
	DbgFuncEntry();

	DbgPrintf("seq = 0x%x\r\n",ack_body->seq);
	DbgPrintf("id = 0x%x\r\n",ack_body->id);
	DbgPrintf("result = 0x%x\r\n",ack_body->result);
	
	DbgFuncExit();
}

