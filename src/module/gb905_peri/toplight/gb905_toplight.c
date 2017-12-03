/****************************************Copyright (c)**************************************************                         
** File name:			gb905_toplight.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-18
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
#include	"misc/misc.h"
#include	"misc/check.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"

#include	"middleware/info/status.h"
#include	"middleware/uart/fleety_uart.h"

#define		DEBUG_Y
#include	"libs/debug.h"


#define		TOPLIGHT_TYPE_ID						0x05		//debug test 0x00
#define		TOPLIGHT_VENDOR_ID						0x00		// 0x10		//debug test 0x02

#define		TOPLIGHT_COMMAND_QUERY_STATE			0x0000
#define		TOPLIGHT_COMMAND_RESET					0x0001
#define		TOPLIGHT_COMMAND_SET_BAUDRATE			0x0003

#define		TOPLIGHT_COMMAND_SET_OPERATION_STATUS	0x0010
#define		TOPLIGHT_COMMAND_SET_STAR_STATUS		0x0011
#define		TOPLIGHT_COMMAND_SET_NIGHT_MODE			0x0012

#define		TOPLIGHT_COMMAND_SET_SECURITY			0x0013
#define		TOPLIGHT_COMMAND_CLR_SECURITY			0x0014

#define		TOPLIGHT_COMMAND_SET_NIGHT_PARAMS		0x0020

#define		TOPLIGHT_COMMAND_UPGRADE				0x00FF



static void gb905_toplight_build_header(gb905_peri_header_t* header,unsigned short cmd,unsigned short len)
{
	header->type = TOPLIGHT_TYPE_ID;
	header->vendor = TOPLIGHT_VENDOR_ID;
	gb905_peri_build_header(header,cmd,len);
}

static void gb905_toplight_common_ack(unsigned short cmd,unsigned char result)
{
	DbgFuncEntry();

	DbgPrintf("cmd(0x%x) ack is 0x%x\r\n",cmd,result);

	if(result != GB905_RESULT_OK)
	{
		DbgWarn("cmd(0x%x) ack is 0x%x\r\n",cmd,result);
	}
	else
	{
		DbgGood("common ack (server --> toplight) ok!(cmd = 0x%x)\r\n",cmd);
	}
 
	DbgFuncExit();	
}

static void gb905_toplight_query_state_ack(unsigned char * buf,unsigned short len)
{
	gb905_toplight_state_t * toplight_state;
	DbgFuncEntry();
	
	toplight_state = (gb905_toplight_state_t *)buf;

	DbgPrintf("hw version = 0x%x\r\n",toplight_state->hw_version);

	DbgPrintf("hw version = 0x%x\r\n",toplight_state->sw_major_version);
	DbgPrintf("hw version = 0x%x\r\n",toplight_state->sw_minor_version);

	DbgPrintf("device state = 0x%x\r\n",toplight_state->device_state);
	DbgPrintf("running state = 0x%x\r\n",toplight_state->running_state);
	DbgPrintf("night mode = 0x%x\r\n",toplight_state->night_mode);
	
	//Set_inspection_data(GB905_TLT_TYPE,buf,len);
	set_toplight_connected();

	DbgFuncExit();
}

static bool gb905_toplight_parse_header(buff_mgr_t * msg)
{
	bool ret = true;
	gb905_peri_header_t * header;

	DbgFuncEntry();

	header = (gb905_peri_header_t *)(msg->buf);

	header->len = EndianReverse16(header->len);
	header->cmd = EndianReverse16(header->cmd);
	
	DbgPrintf("len = 0x%04x\r\n",header->len);
	DbgPrintf("cmd = 0x%04x\r\n",header->cmd);

	// len : start from device_info
	if(header->len != msg->len - (int)offsetof(gb905_peri_header_t,type) - sizeof(gb905_peri_tail_t))
	{
		DbgError("msg len is invaild!\r\n");
				
		ret = false;
		return ret;
	}

	DbgPrintf("type = 0x%04x\r\n",header->type);
	
	if(header->type != TOPLIGHT_TYPE_ID)
	{
		DbgError("toplight terminal type error!\r\n");
				
		ret = false;
		return ret;
	}

	DbgPrintf("vendor = 0x%04x\r\n",header->vendor);
	
	if( header->vendor != TOPLIGHT_VENDOR_ID )
	{
		DbgError("toplight ternimal id is error!\r\n");
		
		ret = false;
		return ret;
	}
	
	DbgFuncExit();
	
	return ret;
}

/**
* @brief 	顶灯一条协议的具体分析
* @param buf 		存放顶灯消息的buf
* @param len  		存放顶灯消息的长度
*
* @return：解析完协议的程度(  需要偏移的长度)
*/
static int gb905_toplight_parse_protocol(buff_mgr_t * msg)
{
	
	bool ret = 0;
	
	gb905_peri_header_t * header;
	unsigned char * toplight_body_buff;
	unsigned short toplight_body_len;

	DbgFuncEntry();
	
	header = (gb905_peri_header_t *)(msg->buf);
	toplight_body_buff = (unsigned char *)(msg->buf + sizeof(gb905_peri_header_t));
	toplight_body_len = header->len - 4;
	
#if 0
	{
		int i;
		for(i = 0;i < toplight_data_len;i++)
		{
			DbgPrintf("-------toplight_date[%d] = 0x%x\r\n",i,toplight_data[i]);
		}
	}
#endif

	DbgPrintf("cmd = 0x%04x\r\n",header->cmd);
		
	switch(header->cmd)
	{

		case TOPLIGHT_COMMAND_QUERY_STATE:
			gb905_toplight_query_state_ack(toplight_body_buff,toplight_body_len);
			//inspection_toplight_state(toplight_data,toplight_data_len);
			break;

		case TOPLIGHT_COMMAND_RESET:
		case TOPLIGHT_COMMAND_SET_BAUDRATE:
		case TOPLIGHT_COMMAND_SET_OPERATION_STATUS:
		case TOPLIGHT_COMMAND_SET_STAR_STATUS:
		case TOPLIGHT_COMMAND_SET_NIGHT_MODE:
		case TOPLIGHT_COMMAND_SET_NIGHT_PARAMS:		
			gb905_toplight_common_ack(header->cmd,*toplight_body_buff);
			break;
#if 0


		case TOPLIGHT_COMMAND_UPGRADE:
			break;
	#endif		
		default:
			DbgWarn("don't support this toplight command!\r\n");
			ret = -1;
			break;
	}
	
	return ret;
}



/*
* 分析一条消息命令
* @msg： 存放完整消息命令的buf
* @header : 解析出协议的头信息
* @return：解析出协议，需要偏移的长度
*/
static int gb905_toplight_parse_a_full_msg(buff_mgr_t *msg)
{

	int ret;

	DbgFuncEntry();

	ret = gb905_toplight_parse_header(msg);
	if( ret < 0){
		return ret;
	}

	ret = gb905_toplight_parse_protocol(msg);

	DbgFuncExit();

	return ret;

}

/**
* @brief 	顶灯的协议分析
* @param buf 		存放顶灯消息的buf
* @param len  		存放顶灯消息的长度
*
* @return：解析完协议的程度(  需要偏移的长度)
*/
int gb905_toplight_protocol_ayalyze(unsigned char * buf,int len)
{
	unsigned char * msg_buf;
	int msg_len;
	
	int offset;
	
	double_buff_mgr_t toplight_buf;

	DbgFuncEntry();

	msg_buf = buf;
	msg_len = len;
	
	do{
		memset(&toplight_buf,0,sizeof(double_buff_mgr_t));
		
		toplight_buf.raw.buf = msg_buf;
		toplight_buf.raw.len = msg_len;
		
		offset = gb905_peri_get_a_full_msg(&toplight_buf);

		{
			int i;
				
			DbgPrintf("offset = %d\r\n",offset);
			DbgPrintf("toplight_lens = %d\r\n",toplight_buf.raw.len);
			
			for(i=0;i<toplight_buf.raw.len;i++)
			{
				DbgPrintf("toplight_buf[%d] = 0x%2x\r\n",i,toplight_buf.raw.buf[i]);
			}
		}

		if(offset && toplight_buf.raw.len)
		{
			// 3:magic_end + xor				5: magic_start+magic_end+xor
			if(toplight_buf.raw.buf[toplight_buf.raw.len - 3] != xor8_computer(toplight_buf.raw.buf + 2,toplight_buf.raw.len - 5))
			{
				DbgError("xor verify failed!\r\n");
				return -1;
			}
			
			gb905_toplight_parse_a_full_msg(&toplight_buf.raw);
		}

		msg_buf += offset;
		msg_len -= offset;
	}while(offset && toplight_buf.raw.len && msg_len > 0);

	DbgFuncExit();
	
	return (len - msg_len);
}


//-----
/** 
* @brief 	查询顶灯状态
* 
* @return		查询是否成功
*/
bool gb905_toplight_query_state(void)
{
	unsigned char *buf;
	unsigned short len;
	unsigned char xor;
	
	gb905_peri_header_t* toplight_header;
	gb905_peri_tail_t* toplight_tail;
	gb905_peri_bcd_timestamp_t * toplight_timestamp;

	
	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(gb905_peri_bcd_timestamp_t) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("toplight state query memory malloc failed!\r\n");
		return false;
	}
	memset(buf,0x00,len);
	
	
	toplight_header = (gb905_peri_header_t*)buf;
	gb905_toplight_build_header(toplight_header,TOPLIGHT_COMMAND_QUERY_STATE,sizeof(gb905_peri_header_t) + sizeof(gb905_peri_bcd_timestamp_t) - offsetof(gb905_peri_header_t,type));

	toplight_timestamp = (gb905_peri_bcd_timestamp_t *)(buf + sizeof(gb905_peri_header_t));
	gb905_peri_build_timestamp(toplight_timestamp);
	
	toplight_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + sizeof(gb905_peri_bcd_timestamp_t));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_bcd_timestamp_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(toplight_tail,xor);
	
	fleety_uart_send(TOPLIGHT_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;
}

/** 
* @brief 	设置顶灯通讯波特率
* @param band 	波特率
* 
* @return			设置是否成功
*/
bool gb905_toplight_set_baudrate(unsigned char band)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* toplight_header;
	gb905_peri_tail_t* toplight_tail;

	unsigned char xor;
	unsigned char * toplight_band;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(band) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("toplight set band memory malloc failed!\r\n");
		return false;
	}

	memset(buf,0x00,len);
	
	toplight_header = (gb905_peri_header_t*)buf;
	gb905_toplight_build_header(toplight_header,TOPLIGHT_COMMAND_SET_BAUDRATE,sizeof(gb905_peri_header_t) + sizeof(band) - offsetof(gb905_peri_header_t,type));

	toplight_band = (unsigned char *)(buf + sizeof(gb905_peri_header_t));
	*toplight_band = band;

	toplight_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + sizeof(band));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_header_t) + sizeof(band) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(toplight_tail,xor);

	fleety_uart_send(TOPLIGHT_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;
	
}


/** 
* @brief 	设置顶灯夜间工作模式
* @param mode 	=0x00,关闭夜间模式;   =0x01,开启夜间模式
* 
* @return			设置是否成功
*/
bool gb905_toplight_set_night_mode(unsigned char mode)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* toplight_header;
	gb905_peri_tail_t* toplight_tail;

	unsigned char xor;
	unsigned char * toplight_mode;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(mode) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("toplight set star status memory malloc failed!\r\n");
		return false;
	}

	memset(buf,0x00,len);
	
	toplight_header = (gb905_peri_header_t*)buf;
	gb905_toplight_build_header(toplight_header,TOPLIGHT_COMMAND_SET_NIGHT_MODE,sizeof(gb905_peri_header_t) + sizeof(mode) - offsetof(gb905_peri_header_t,type));

	toplight_mode = (unsigned char *)(buf + sizeof(gb905_peri_header_t));
	*toplight_mode = mode;

	toplight_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + sizeof(mode));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_header_t) + sizeof(mode) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(toplight_tail,xor);

	fleety_uart_send(TOPLIGHT_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;
}

/** 
* @brief	设置顶灯夜间工作模式参数
* @param 起夜间模式的始时间和结束时间
* 
* @return		设置是否成功
*/
int gb905_toplight_set_night_params(gb905_toplight_night_parems_t * night_params)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* toplight_header;
	gb905_peri_tail_t* toplight_tail;

	unsigned char xor;
	unsigned char * toplight_night_params;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(gb905_toplight_night_parems_t) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("toplight set night params memory malloc failed!\r\n");
		return false;
	}

	memset(buf,0x00,len);

	toplight_header = (gb905_peri_header_t*)buf;
	gb905_toplight_build_header(toplight_header,TOPLIGHT_COMMAND_SET_NIGHT_PARAMS,sizeof(gb905_peri_header_t) + sizeof(gb905_toplight_night_parems_t) - offsetof(gb905_peri_header_t,type));

	toplight_night_params = (unsigned char *)(buf + sizeof(gb905_peri_header_t));
	memcpy(toplight_night_params,night_params,sizeof(gb905_toplight_night_parems_t));

	toplight_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + sizeof(gb905_toplight_night_parems_t));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_toplight_night_parems_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(toplight_tail,xor);

	fleety_uart_send(TOPLIGHT_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;	
}


/*设置顶灯营运状态
	参数：status
	0x00：空车
	0x01：重车
	0x02：停运
	0x03：电召
	0x04：报警
	0x05：显示防伪密标
*/
bool gb905_toplight_set_operation_status(unsigned char status)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* toplight_header;
	gb905_peri_tail_t* toplight_tail;

	unsigned char xor;
	unsigned char * toplight_status;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(status) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("toplight set operation status memory malloc failed!\r\n");
		return false;
	}

	memset(buf,0x00,len);
	
	toplight_header = (gb905_peri_header_t*)buf;
	gb905_toplight_build_header(toplight_header,TOPLIGHT_COMMAND_SET_OPERATION_STATUS,sizeof(gb905_peri_header_t) + sizeof(status) - offsetof(gb905_peri_header_t,type));

	toplight_status = (unsigned char *)(buf + sizeof(gb905_peri_header_t));
	*toplight_status = status;

	toplight_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + sizeof(status));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_header_t) + sizeof(status) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(toplight_tail,xor);

	fleety_uart_send(TOPLIGHT_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;	
}


/* 设置顶灯星级显示
	参数：star
	0x00：未评定
	0x01：一星
	0x02：二星
	0x03：三星
	0x04：四星
	0x05：五星
*/
bool gb905_toplight_set_star_status(unsigned char star)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* toplight_header;
	gb905_peri_tail_t* toplight_tail;

	unsigned char xor;
	unsigned char * toplight_status;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(star) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("toplight set star status memory malloc failed!\r\n");
		return false;
	}

	memset(buf,0x00,len);
	
	toplight_header = (gb905_peri_header_t*)buf;
	gb905_toplight_build_header(toplight_header,TOPLIGHT_COMMAND_SET_STAR_STATUS,sizeof(gb905_peri_header_t) + sizeof(star) - offsetof(gb905_peri_header_t,type));

	toplight_status = (unsigned char *)(buf + sizeof(gb905_peri_header_t));
	*toplight_status = star;

	toplight_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + sizeof(star));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_header_t) + sizeof(star) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(toplight_tail,xor);

	fleety_uart_send(TOPLIGHT_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;	
	
}


bool gb905_toplight_reset(void)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* toplight_header;
	gb905_peri_tail_t* toplight_tail;

	unsigned char xor;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("toplight reset memory malloc failed!\r\n");
		return -1;
	}

	memset(buf,0x00,len);
	
	toplight_header = (gb905_peri_header_t*)buf;
	gb905_toplight_build_header(toplight_header,TOPLIGHT_COMMAND_RESET,sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));


	toplight_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(toplight_tail,xor);

	fleety_uart_send(TOPLIGHT_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;
}

// 防伪密标显示
bool gb905_toplight_set_security(unsigned char* data,unsigned short data_len)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* toplight_header;
	gb905_peri_tail_t* toplight_tail;

	unsigned char xor;
	unsigned char * security_data;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + data_len + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("toplight set star status Memory Malloc Failed!\r\n");
		return false;
	}

	memset(buf,0x00,len);
	
	toplight_header = (gb905_peri_header_t*)buf;
	gb905_toplight_build_header(toplight_header,TOPLIGHT_COMMAND_SET_SECURITY,data_len + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));

	security_data = (unsigned char *)(buf + sizeof(gb905_peri_header_t));
	memcpy(security_data,data,data_len);
	
	toplight_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t) + data_len);
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),data_len + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(toplight_tail,xor);

	fleety_uart_send(TOPLIGHT_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;
}

// 取消防伪密标显示
bool gb905_toplight_clear_security(void)
{
	unsigned char *buf;
	unsigned short len;
	
	gb905_peri_header_t* toplight_header;
	gb905_peri_tail_t* toplight_tail;

	unsigned char xor;

	DbgFuncEntry();
	
	len = sizeof(gb905_peri_header_t) + sizeof(gb905_peri_tail_t);
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("toplight set star status memory malloc failed!\r\n");
		return false;
	}

	memset(buf,0x00,len);
	
	toplight_header = (gb905_peri_header_t*)buf;
	gb905_toplight_build_header(toplight_header,TOPLIGHT_COMMAND_CLR_SECURITY,sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));

	
	toplight_tail = (gb905_peri_tail_t *)(buf + sizeof(gb905_peri_header_t));
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(toplight_tail,xor);

	fleety_uart_send(TOPLIGHT_UART,buf,len);

	free(buf);

	DbgFuncExit();

	return true;
}


