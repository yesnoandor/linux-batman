/****************************************Copyright (c)**************************************************                         
** File name:			gb905_meter.c
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
		
#include	"misc/endian.h"
#include	"misc/bcd.h"
#include	"misc/misc.h"
#include	"misc/check.h"



#include	"module/gb905/gb905_common.h"
//#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/tsm/gb905_tsm.h"
#include	"module/gb905_peri/meter/gb905_meter.h"

#include	"middleware/info/status.h"
#include	"middleware/info/setting.h"
#include	"middleware/info/meter.h"

#include	"middleware/uart/fleety_uart.h"

#include	"middleware/event/fleety_event.h"
		
#define		DEBUG_Y
#include	"libs/debug.h"

#define		METER_TYPE_ID		0x02
#define		METER_VENDOR_ID		0x03			// 0x10

#define		METER_RESULT_OK		0x90
#define		METER_RESULT_FAIL	0xFF


#define		METER_COMMAND_QUERY_STATE			0x0000
#define		METER_COMMAND_CALIBRATION_TIME		0x0001
#define		METER_COMMAND_QUERY_PARAMETER		0x0004
#define		METER_COMMAND_SET_PARAMETER			0x0005		
#define		METET_COMMAND_OPERAION_RECORD		0x0006

#define		METER_COMMAND_OPEN					0x00E0
#define		METER_COMMAND_OPEN_SUCCESS			0x00E1
#define		METER_COMMAND_CLOSE					0x00E3
#define		METER_COMMAND_CLOSE_SUCCESS			0x00E4

#define		METER_COMMAND_OPERATION_OCCUPIED	0x00E7
#define		METER_COMMAND_OPERATION_EMPTY		0x00E8
#define		METER_COMMAND_HEART_BEAT			0x00E9

#define		METER_OPERATION_RESEND_WHEN_CLOSE	0X00F1
#define		METER_OPERATION_RESEND				0X00F2

#define		MATER_COMMAND_UPGRADE				0x00FF


static void gb905_meter_build_header(gb905_peri_header_t* header,unsigned short cmd,unsigned short len)
{
	header->type = METER_TYPE_ID;
	header->vendor = METER_VENDOR_ID;
	gb905_peri_build_header(header,cmd,len);
}

static void gb905_meter_build_timestamp(gb905_meter_timestamp_t * timestamp)
{
	time_t timep;
	struct tm *p;
	
	time(&timep);
	p = localtime(&timep);

	decimal2bcd(p->tm_year + 1900,&timestamp->bcd[0],2);
	decimal2bcd(p->tm_mon+1,&timestamp->bcd[2],1);
	decimal2bcd(p->tm_mday,&timestamp->bcd[3],1);
	decimal2bcd(p->tm_hour,&timestamp->bcd[4],1);
	decimal2bcd(p->tm_min,&timestamp->bcd[5],1);
}

static void gb905_meter_build_heart_beat_ack_body(gb905_meter_heart_beat_ack_body_t * body)
{
	setting_params_t *parameter;
	
	DbgFuncEntry();
	
	parameter = get_setting_params();
	
	body->terminal_state = EndianReverse16(0x0000);
	// 5 :: BCD[5]
	memcpy(&body->time_limit,parameter->meter_operation_count_time,sizeof(body->time_limit));

	body->number_of_times_limit = parameter->meter_operation_count_limit;		
	
	body->rfu = EndianReverse16(0x0000);

	DbgFuncExit();
}

static void gb905_meter_build_set_param_body(gb905_meter_parameter_body_t * body)
{
	gb905_meter_parameter_body_t * meter_params;
		
	DbgFuncEntry();

	meter_params = get_meter_params();
	memcpy(body,meter_params,sizeof(gb905_meter_parameter_body_t));

	DbgFuncExit();
}

static void gb905_meter_build_login_ack_body(gb905_meter_loginout_ack_body_t * ack_body)
{
	license_params_t * license_params;
	setting_params_t * setting_params;

	license_params = (license_params_t *)ack_body;
	get_license_params(license_params);
	
	setting_params = get_setting_params();

	// 获取当前刷卡时间
	gb905_meter_build_timestamp(&ack_body->swipe_timestamplit);

	// 获取当前ISU  状态
	ack_body->terminal_status = EndianReverse16(0x0000);
	
	memcpy(ack_body->time_limit,setting_params->meter_operation_count_time,sizeof(setting_params->meter_operation_count_time));
	memcpy(ack_body->number_of_times_limit,&setting_params->meter_operation_count_limit,sizeof(setting_params->meter_operation_count_limit));

	ack_body->result = METER_RESULT_OK;
}




/** 
* @brief 	解析计价器发送过来的状态信息
*
*/
static void gb905_meter_parse_state(unsigned char * buf,unsigned short len)
{
	gb905_meter_state_body_t * state_body;
	//udpate_match_info_t meter_match_info;

	DbgFuncEntry();

	state_body = (gb905_meter_state_body_t *)buf;
	
	DbgPrintf("hw version = 0x%x\r\n",state_body->hw_version);
	DbgPrintf("sw major version = 0x%x\r\n",state_body->sw_major_version);
	DbgPrintf("sw minor version = 0x%x\r\n",state_body->sw_minor_version);
	
	DbgPrintf("device state = 0x%x\r\n",state_body->device_state);
	DbgPrintf("running state = 0x%x\r\n",state_body->running_state);
		

	#if 0
	meter_state = (gb2014_meter_state_t *)buf;

	meter_match_info.vendor_id = meter_state->uuid_device_id.vendor_id;
	meter_match_info.hw_version = meter_state->hw_version;
	meter_match_info.sw_version[0] = meter_state->sw_major_version;
	meter_match_info.sw_version[1] = meter_state->sw_minor_version;
	set_meter_match_info(&meter_match_info);
	
	Set_inspection_data(GB905_PRICE_TYPE,buf,len);
	#endif
	
	DbgFuncExit();
}

static void gb905_meter_parse_query_parameter(unsigned char * buf,unsigned short len)
{
	gb905_meter_parameter_body_t * meter_params;
		
	DbgFuncEntry();

	meter_params = get_meter_params();
	memcpy(meter_params,buf,sizeof(gb905_meter_parameter_body_t));
	
	debug_meter_params();

	DbgFuncExit();
}


static void gb905_meter_set_parameter_ack_treat(unsigned char * buf,unsigned short len)
{
	gb905_meter_set_param_ack_body_t * ack_body;

	DbgFuncEntry();
		
	ack_body = (gb905_meter_set_param_ack_body_t *)buf;
	
	DbgPrintf("ack result is: 0x%x\r\n",ack_body->ack_result);

	DbgFuncExit();
}

/** 
* @brief 	计价器开机确认成功(计价器发出开机数据信息)
*
* @return 是否运行正常
*/
static bool gb905_meter_open_close_treat(unsigned char mark)
{
	bool ret = false;
	
	if(mark == 0x90)	// 0x90 :  开机标识
	{
		gb905_tsm_get_driver_info();
		ret = true;
	}

	return ret;
}

/** 
* @brief 	计价器开机确认成功(计价器发出开机数据信息)
*
* @return 是否运行正常
*/
static unsigned char gb905_meter_open_success_treat(unsigned char *buf,unsigned short len)
{
	gb905_meter_open_success_body_t * body;
	fleety_event_t event;

	DbgFuncEntry();

	body = (gb905_meter_open_success_body_t *)buf;

	DbgPrintf("company license number = %s\r\n",body->company_license_number);

	if(body->result == METER_RESULT_OK)
	{	
		//set_loading_status(true);

		set_meter_open_success_info((char *)body);
		
		event.id = LOGINING_EVENT;
		event.param = true;
		event.priority = DAFAULT_PRIORITY;
		
		fleety_event_push(&event);
	}
	else
	{
		DbgError("meter open failed!\r\n");
	}
	
	DbgFuncExit();

	return body->result;
}





/** 
* @brief 	计价器关机确认成功(计价器发出关机信息)
*
* @return 是否运行正常
*/
static unsigned char gb905_meter_close_success_treat(unsigned char *buf,unsigned short len)
{
	gb905_meter_close_success_body_t * body;
	fleety_event_t event;

	DbgFuncEntry();

	body = (gb905_meter_close_success_body_t *)buf;

	//set_loading_status(false);
	set_meter_close_success_info((char *)body);
		
	event.id = LOGINING_EVENT;
	event.param = false;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);
	
	DbgFuncExit();

	return METER_RESULT_OK;

#if 0
	meter2014_logout_data_t * meter_data;
	meter_data = (meter2014_logout_data_t*)date;
	set2014_meter_logout_info(meter_data);
	
	fleety_event_t event;
	unsigned int logining;
		
	DbgFuncEntry();
		
	logining = 0;
	set_logining_status(logining);
		
	event.event_id = LOGINING_EVENT;
	event.event_param = logining;
		
	fleety_event_push(&event);
		
	DbgFuncExit();	
#endif

	return METER_RESULT_OK;
}


/** 
* @brief 	计价器心跳应答
*
*/
static void gb905_meter_heart_beat_ack(void)
{
	unsigned char *buf;
	unsigned char xor;
	
	gb905_meter_heart_beat_ack_t heart_beat_ack;

	DbgFuncEntry();

	buf = (unsigned char *)&heart_beat_ack;

	gb905_meter_build_header(&heart_beat_ack.header,METER_COMMAND_HEART_BEAT,sizeof(gb905_meter_heart_beat_ack_body_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));
	
	gb905_meter_build_heart_beat_ack_body(&heart_beat_ack.body);

	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_meter_heart_beat_ack_body_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(&heart_beat_ack.tail,xor);

	fleety_uart_send(METER_UART,buf,sizeof(gb905_meter_heart_beat_ack_t));

	DbgFuncExit();	
}

/** 
* @brief 	计价器心跳数据解析(获取计价器当前状态，出租车营运证，驾驶员资格证)
*
*/
static void gb905_meter_parse_heart_beat(unsigned char *buf,unsigned short len)
{
	
	gb905_meter_heart_beat_body_t * heart_beat_body;

	DbgFuncEntry();
	
	heart_beat_body = (gb905_meter_heart_beat_body_t *)buf;


	if(heart_beat_body->meter_state.flag.force_on)
	{	
		DbgPrintf("meter force on\r\n");
	}

	if(heart_beat_body->meter_state.flag.force_off)
	{	
		DbgPrintf("meter force off\r\n");
	}
	
	#if 0
	{
		unsigned char i;
		DbgPrintf("meter state = 0x%x\r\n",heart_beat_body->meter_state.whole);

		for(i = 0;i < ARRAY_SIZE(heart_beat_body->company_license_number);i++)
		{
			DbgPrintf("company license number[%d] = 0x%x \r\n",i,heart_beat_body->company_license_number[i]);
		}

		for(i = 0;i < ARRAY_SIZE(heart_beat_body->driver_license_number);i++)
		{
			DbgPrintf("driver license number[%d] = 0x%x \r\n",i,heart_beat_body->driver_license_number[i]);
		}
	}
	#endif
	
	set_meter_connected();
	
	DbgFuncExit();
}

static int gb905_meter_common_ack(unsigned short command,unsigned char result)
{
	unsigned char *ack_buf;
	unsigned char ack_len;
	
	gb905_peri_header_t* meter_header;
	gb905_peri_tail_t* meter_tail;
	unsigned char * meter_ack_result;

	unsigned char xor;

	DbgFuncEntry();

	ack_len = sizeof(gb905_peri_header_t) + sizeof(gb905_peri_tail_t) + sizeof(unsigned char);
	ack_buf = (unsigned char *)malloc(ack_len);
	if(!ack_buf)
	{
		DbgError("meter common ack memory malloc failed!\r\n");
		return -1;
	}
	
	meter_header = (gb905_peri_header_t*)ack_buf;
	gb905_meter_build_header(meter_header,command,sizeof(unsigned char) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));

	meter_ack_result = (unsigned char *)(ack_buf + sizeof(gb905_peri_header_t));
	*meter_ack_result = result;
	
	meter_tail = (gb905_peri_tail_t*)(ack_buf + sizeof(gb905_peri_header_t) + sizeof(unsigned char));
	xor = xor8_computer(ack_buf + offsetof(gb905_peri_header_t,len),sizeof(unsigned char) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(meter_tail,xor);

#if 0
	{
		int j;

		for(j=0;j<ack_len;j++)
		{
			DbgPrintf("ack_buf[%d] = 0x%x\r\n",j,ack_buf[j]);
		}
	}
#endif

	fleety_uart_send(METER_UART,ack_buf,ack_len);
	
	free(ack_buf);

	DbgFuncExit();
	
	return 0;
}

/** 
* @brief 	GB905  计价器空车变重车的处理
* @param buf		存放计价器传过来消息体的缓存数据地址
* @param buf		存放计价器传过来消息体的缓存数据长度
* 
* @return			解析是否成功
*/
static unsigned char gb905_meter_loading_treat(unsigned char * buf,unsigned short len)
{
	//gb905_peri_bcd_timestamp_t * occupied_timestamp;
	//unsigned int loading;

	fleety_event_t event;

	DbgFuncEntry();

	//occupied_timestamp = (gb905_peri_bcd_timestamp_t *)buf;

	set_loading_status(true);
	
	event.id = LOADING_EVENT;
	event.param = true;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);


	//gb905_take_picture(TAKE_PIC_REASON_OCCUPY);
	//fleety_report(0);
	
	DbgFuncExit();

	return METER_RESULT_OK;
	
	//1.set _KCDstatus
	//2.set location flat status
	//3.set toplight status
	//4.send location report
	//5.take picture	
}

/** 
* @brief 	GB905  计价器重车变空车的处理
* @param buf		存放计价器传过来消息体的缓存数据地址
* @param buf		存放计价器传过来消息体的缓存数据长度
* 
* @return			解析是否成功
*/
static unsigned char gb905_meter_unloading_treat(unsigned char * buf,unsigned short len)
{
	gb905_meter_operation_t * meter_operation;
	//unsigned int loading;

	fleety_event_t event;
		

	DbgFuncEntry();

	meter_operation = (gb905_meter_operation_t *)buf;
	set_meter_operation_info((char *)meter_operation);

	
	set_loading_status(false);
	
	event.id = LOADING_EVENT;
	event.param = false;
	event.priority = DAFAULT_PRIORITY;
	
	fleety_event_push(&event);


	//meter_operation = (gb2014_meter_operation_t *)buf;
	//set2014_meter_operation_info(meter_operation);

	//loading = 0;
	//set_loading_status(loading);
	
	//event.event_id = LOADING_EVENT;
	//event.event_param = loading;

	//fleety_event_push(&event);
	//fleety_report(0);
	
	DbgFuncExit();

	return METER_RESULT_OK;
	
	//clear kcdstatus
	//clear location report kcdstatus 
	//take picture
	//set toplight status
}

/** 
* @brief 	GB905  计价器协议的消息头具体解析
* @param msg		存放消息的缓存数据结构
* 
* @return			解析是否成功
*/
static bool gb905_meter_parse_header(buff_mgr_t * msg)
{
	bool ret = true;
	gb905_peri_header_t * header;

	DbgFuncEntry();

	header = (gb905_peri_header_t *)(msg->buf);

	header->len = EndianReverse16(header->len);
	header->cmd = EndianReverse16(header->cmd);
	
	//DbgPrintf("len = 0x%04x\r\n",header->len);
	//DbgPrintf("cmd = 0x%04x\r\n",header->cmd);

	// len : start from device_info
	if(header->len != msg->len - (int)offsetof(gb905_peri_header_t,type) - sizeof(gb905_peri_tail_t))
	{
		DbgError("msg len is invaild!\r\n");
				
		ret = false;
		return ret;
	}

	//DbgPrintf("type = 0x%04x\r\n",header->type);
	
	if(header->type != METER_TYPE_ID)
	{
		DbgError("meter terminal type error!\r\n");
				
		ret = false;
		return ret;
	}

	// DbgPrintf("vendor = 0x%04x\r\n",header->vendor);
	
	if( header->vendor != METER_VENDOR_ID )
	{
		DbgError("meter ternimal id is error!\r\n");
		
		ret = false;
		return ret;
	}
	
	DbgFuncExit();
	
	return ret;
}

/** 
* @brief 	GB905  计价器协议的消息体具体解析
* @param msg		存放消息的缓存数据结构
* 
* @return			解析是否成功
*/
static int gb905_meter_parse_protocol(buff_mgr_t * msg)
{
	bool ret = 0;
	unsigned char result;
	
	gb905_peri_header_t * header;
	unsigned char * buf;
	unsigned short len;

	DbgFuncEntry();
	
	header = (gb905_peri_header_t *)(msg->buf);
	buf = (unsigned char *)(msg->buf + sizeof(gb905_peri_header_t));
	len = header->len - (sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));
	
#if 1
	{
		int i;
		DbgPrintf("parse meter date is :");
		for(i = 0;i < header->len + 7;i++)
		{
			DbgPrintf("0x%02x ",msg->buf[i]);
		}
		DbgPrintf("\r\n");
	}
#endif

	DbgPrintf("cmd = 0x%04x\r\n",header->cmd);
		
	switch(header->cmd)
	{
		case METER_COMMAND_QUERY_STATE:
			gb905_meter_parse_state(buf,len);
			//inspection_meter_state(meter_data,meter_data_len);
			break;

		case METER_COMMAND_QUERY_PARAMETER:
			gb905_meter_parse_query_parameter(buf,len);
			break;

		case METER_COMMAND_SET_PARAMETER:
			gb905_meter_set_parameter_ack_treat(buf,len);
			break;	

		case METER_COMMAND_HEART_BEAT:
			gb905_meter_parse_heart_beat(buf,len);
			gb905_meter_heart_beat_ack();
			break;

		case METER_COMMAND_OPERATION_OCCUPIED:  // 计价器空车转重车
			result = gb905_meter_loading_treat(buf,len);
			gb905_meter_common_ack(header->cmd,result);
			break;
			
		case METER_COMMAND_OPERATION_EMPTY:  	// 计价器重车转空车
			result = gb905_meter_unloading_treat(buf,len);
			gb905_meter_common_ack(header->cmd,result);
			break;

		case METER_OPERATION_RESEND:			// 计价器补传
			result = METER_RESULT_OK;
			gb905_meter_common_ack(header->cmd,result);
			break;

		case METER_COMMAND_OPEN:  				// 计价器开机
		case METER_COMMAND_CLOSE:				// 计价器关机
			gb905_meter_open_close_treat(*(unsigned char *)buf);		// 给meter   的应答在tsm  功能模块中调用
			break;

		case METER_COMMAND_OPEN_SUCCESS:		// 计价器开机成功
			gb905_meter_open_success_treat(buf,len);
			break;

		case METER_COMMAND_CLOSE_SUCCESS:		// 计价器关机成功
		case METER_OPERATION_RESEND_WHEN_CLOSE:
			result = gb905_meter_close_success_treat(buf,len);
			gb905_meter_common_ack(header->cmd,result);
			break;

		case MATER_COMMAND_UPGRADE:
			//update_peripheral_common_ack(meter_data,meter_data_len);
			break;
	
		default:
			DbgWarn("don't support this meter command!\r\n");
			ret = -1;
			break;
			
	}
	
	return ret;
}


/*
*  @brief 		解析出一条完整的计价器消息
*  @msg： 	存放完整消息命令的buf
* 
*  @return：解析出协议，需要偏移的长度
*/
static int gb905_meter_parse_a_full_msg(buff_mgr_t *msg)
{

	int ret;

	DbgFuncEntry();

	ret = gb905_meter_parse_header(msg);
	if( ret < 0)
	{
		return ret;
	}

	ret = gb905_meter_parse_protocol(msg);

	DbgFuncExit();

	return ret;

}

//-----
/** 
* @brief 	计价器运价设置指令
*
*/
void gb905_meter_set_parameter(void)
{
	unsigned char *buf;

	unsigned char xor;
	gb905_meter_set_param_t set_param;

	DbgFuncEntry();
	
	buf = (unsigned char *)&set_param;
	gb905_meter_build_header(&set_param.header,METER_COMMAND_SET_PARAMETER,sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));

	gb905_meter_build_set_param_body(&set_param.body);
	
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(&set_param.tail,xor);

	fleety_uart_send(METER_UART,buf,sizeof(gb905_meter_set_param_t));

	DbgFuncExit();
}

/** 
* @brief 	计价器运价查询指令
*
*/
void gb905_meter_query_parameter(void)
{
	unsigned char *buf; 
	
	unsigned char xor;
	gb905_meter_query_param_t query_param;


	DbgFuncEntry();
	
	buf = (unsigned char *)&query_param;
	gb905_meter_build_header(&query_param.header,METER_COMMAND_QUERY_PARAMETER,sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));

	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(&query_param.tail,xor);

	fleety_uart_send(METER_UART,buf,sizeof(gb905_meter_query_param_t));

	DbgFuncExit();
}

/** 
* @brief 	计价器状态查询指令
*
*/
void gb905_meter_query_state(void)
{	
	unsigned char *buf; 

	unsigned char xor;
	gb905_meter_query_state_t query_state;

	DbgFuncEntry();

	buf = (unsigned char *)&query_state;
	gb905_meter_build_header(&query_state.header,METER_COMMAND_QUERY_STATE,sizeof(gb905_peri_bcd_timestamp_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));

	gb905_peri_build_timestamp(&query_state.timestamp);

	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_peri_bcd_timestamp_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len));
	gb905_peri_build_tail(&query_state.tail,xor);

	fleety_uart_send(METER_UART,buf,sizeof(gb905_meter_query_state_t));

	DbgFuncExit();
}


/** 
* @brief 	往计价器发送开机应答(    TSM 模块中调用)
*
* @return 是否运行正常
*/
bool gb905_meter_open_ack(void)
{
	unsigned char *buf;
	
	unsigned char xor;
	gb905_meter_loginout_ack_t login_ack;

	DbgFuncEntry();

	buf = (unsigned char *)&login_ack;
	gb905_meter_build_header(&login_ack.header,METER_COMMAND_OPEN,sizeof(gb905_meter_loginout_ack_body_t) + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,type));

	gb905_meter_build_login_ack_body(&login_ack.ack_body);
		
	xor = xor8_computer(buf + offsetof(gb905_peri_header_t,len),sizeof(gb905_meter_loginout_ack_body_t)  + sizeof(gb905_peri_header_t) - offsetof(gb905_peri_header_t,len) );
	gb905_peri_build_tail(&login_ack.tail,xor);

	{
		int i;
		for(i=0;i<sizeof(gb905_meter_loginout_ack_t);i++)
		{
			DbgPrintf("login ack buf[%d] = 0x%x\r\n",i,buf[i]);
		}
	}
	
	fleety_uart_send(METER_UART,buf,sizeof(login_ack));

	DbgFuncExit();
	
	return METER_RESULT_OK;
}


/** 
* @brief 	GB905   的计价器协议解析
* @param buf	 	存放解析以后的消息缓存
* @param len		存放解析以前的消息长度
*
* @return 		解析掉的数据长度(   可以从缓冲区中移除)
*/
int gb905_meter_protocol_ayalyze(unsigned char * buf,int len)
{
	unsigned char * msg_buf;
	int msg_len;
	
	int offset;
	
	double_buff_mgr_t meter_buf;

	DbgFuncEntry();

	msg_buf = buf;
	msg_len = len;
	
	do{
		memset(&meter_buf,0,sizeof(double_buff_mgr_t));
		
		meter_buf.raw.buf = msg_buf;
		meter_buf.raw.len = msg_len;
		
		offset = gb905_peri_get_a_full_msg(&meter_buf);
#if 0
		{
			int i;
				
			DbgPrintf("offset = %d\r\n",offset);
			DbgPrintf("meter_lens = %d\r\n",meter_buf.raw.len);
			
			for(i=0;i<meter_buf.raw.len;i++)
			{
				DbgPrintf("meter_buf[%d] = 0x%2x\r\n",i,meter_buf.raw.buf[i]);
			}
		}
#endif
		if(offset && meter_buf.raw.len)
		{
			// 3:magic_end + xor				5: magic_start+magic_end+xor
			if(meter_buf.raw.buf[meter_buf.raw.len - 3] != xor8_computer(meter_buf.raw.buf + 2,meter_buf.raw.len - 5))
			{
				DbgError("xor verify failed!\r\n");
				return -1;
			}

			gb905_meter_parse_a_full_msg(&meter_buf.raw);
		}

		msg_buf += offset;
		msg_len -= offset;
	}while(offset && meter_buf.raw.len && msg_len > 0);
	
	return (len - msg_len);

	DbgFuncExit();
}

