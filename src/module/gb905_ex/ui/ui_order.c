/****************************************Copyright (c)**************************************************                         
** File name:			ui_order.c
** Descriptions:		订单业务处理(与APK信息交互)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-19
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

#include	"module/gb905/order/gb905_order.h"
#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_history_info.h"
#include	"module/gb905_ex/ui/ui_order.h"


#include	"middleware/info/order.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define	UI_ORDER_FINISH			0x01
#define UI_ORDER_CANCEL			0x02
#define UI_ORDER_GRAP			0x03


enum{
	UI_GRAP_RESULT_FAIL = 0,
	UI_GRAP_RESULT_OK,
};

enum{
	UI_CANCEL_REASON_ACCIDENT = 0,
	UI_CANCEL_REASON_BLOCKING,
	UI_CANCEL_REASON_OTHER,
};

typedef  struct
{
    unsigned char start_magic_id;
    ui_protocol_header_t header;
	order_info_brief_t brief;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed ui_send_order_briefing_t;


typedef  struct
{
    unsigned char start_magic_id;
    ui_protocol_header_t header;
	order_info_detail_t detials;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed ui_send_order_detials_t;


typedef  struct
{
	unsigned int id;				//对应订单ID
	unsigned char result;			//抢单结果(=0:拒绝;=1:抢单)
	
} __packed ui_order_grap_t;


typedef  struct
{
	unsigned int id;					//对应订单ID
	unsigned char type;					//控制类型(=1:完成订单;=2:取消订单)
	unsigned char param;				//控制参数(若为完成订单则填0;若为取消订单则填取消原因)
} __packed ui_order_control_t;

// 历史订单发送到UI信息结构(不包括电话号码和详情)
typedef struct{
	unsigned char state;
	unsigned int id;								// 订单id(若无后续内容则表示该订单未中标)
	unsigned char type;								// 订单类型(0:即时召车;1:预约召车;2:车辆指派)
	unsigned int passenger_lon;						// 乘客位置经度(0.0001'  填0表示不启用)
	unsigned int passenger_lat;						// 乘客位置纬度(0.0001'  填0表示不启用)
	unsigned int destination_lon;					// 目的地位置经度(0.0001'  填0表示不启用)
	unsigned int destination_lat;					// 目的地位置纬度(0.0001'  填0表示不启用)
}__packed ui_order_info_t;


// 订单详情发送到UI信息结构(不包括电话号码和详情)
typedef struct{
	unsigned int id;								// 订单id(若无后续内容则表示该订单未中标)
	unsigned char type;								// 订单类型(0:即时召车;1:预约召车;2:车辆指派)	
	gb905_timestamp_t ride_time;					// 要车时间(YYMMDDhhmmss 全0表示不启用)
	unsigned int passenger_lon;						// 乘客位置经度(0.0001'  填0表示不启用)
	unsigned int passenger_lat;						// 乘客位置纬度(0.0001'  填0表示不启用)
	unsigned int destination_lon;					// 目的地位置经度(0.0001'  填0表示不启用)
	unsigned int destination_lat;					// 目的地位置纬度(0.0001'  填0表示不启用)
}__packed ui_order_detials_t;

/** 
* @brief 	  	发送简明信息到UI
*
*/
void ui_send_order_briefing(void)
{
	ui_send_order_briefing_t brief_info;

	DbgFuncEntry();

	ui_protocol_build_header(&brief_info.header,SYSTEM2UI_ORDER_BRIEF,sizeof(ui_send_order_briefing_t));
	
	get_order_brief_info((unsigned char*)&brief_info.brief);

	ui_protocol_send_data((unsigned char *)&brief_info,sizeof(ui_send_order_briefing_t));

	DbgFuncExit();
}

/** 
* @brief 	  	发送详情信息到UI
* 
*/
void ui_send_order_detials(void)
{
	ui_send_order_detials_t detials_info;
	ui_protocol_header_t *header;
	ui_order_detials_t *detials;
	unsigned char *send_buf;
	char *data_buf;
	int send_len = 0;
	int phone_num_len = 0;
	int description_len = 0;

	DbgFuncEntry();

	get_order_detail_info((unsigned char*)&detials_info.detials);
	phone_num_len = strlen(detials_info.detials.phone) + 1;// 1 : "/0"
	description_len = strlen(detials_info.detials.description) + 1;// 1 : "/0"
	send_len = 3 + sizeof(ui_protocol_header_t) + sizeof(ui_order_detials_t) + phone_num_len + description_len;//3:7e+crc+7e	

	send_buf = malloc(send_len);
	memset(send_buf,0x00,send_len);
	
	header = (ui_protocol_header_t *)(send_buf + 1); // 1: 7e
	ui_protocol_build_header(header,SYSTEM2UI_ORDER_DETIALS,sizeof(ui_order_detials_t) + phone_num_len + description_len);

	detials = (ui_order_detials_t *)(send_buf + 1 + sizeof(ui_protocol_header_t));
	detials->id = detials_info.detials.order_id;
	detials->type = detials_info.detials.order_type;
	memcpy(detials->ride_time.bcd,detials_info.detials.ride_time.bcd,sizeof(detials->ride_time.bcd));
	detials->passenger_lon = detials_info.detials.passenger_lon;
	detials->passenger_lat = detials_info.detials.passenger_lat;	
	detials->destination_lon = detials_info.detials.destination_lon;
	detials->destination_lat = detials_info.detials.destination_lat;

	data_buf = (char *)(send_buf + 1 + sizeof(ui_protocol_header_t) + sizeof(ui_order_detials_t));
	memcpy(data_buf,detials_info.detials.phone,strlen(detials_info.detials.phone));
	data_buf += phone_num_len;
	
	memcpy(data_buf,detials_info.detials.description,strlen(detials_info.detials.description));
	
	ui_protocol_send_data(send_buf,send_len);

	free(send_buf);
	
	DbgFuncExit();	
}


/** 
* @brief 	  			收到UI抢单结果的处理
* @param order_id		订单ID
* @param result		抢单结果
*
*/
void ui_order_grab_treat(unsigned int order_id,unsigned char result)
{
	
	DbgFuncEntry();

	//抢标
	if(result == UI_GRAP_RESULT_OK)
	{
		gb905_order_grab_treat();
	}

	else
	{
		DbgPrintf("refuse order !!! \r\n");
		//拒绝订单
	}
	
	DbgFuncExit();		
}


/** 
* @brief 	  	收到UI对当前订单的处理(抢单，拒绝，完成，取消)
* 
*/
void ui_order_control_treat(unsigned char *buf,int len)
{
	ui_order_control_t *order_control;
	
	DbgFuncEntry();

	order_control = (ui_order_control_t *)buf;

	switch(order_control->type)
	{
		case UI_ORDER_FINISH:
			gb905_order_finish_treat(order_control->id);
			break;

		case UI_ORDER_CANCEL:
			gb905_order_isu_cancel_treat(EndianReverse32(order_control->id),order_control->param);
			break;

		case UI_ORDER_GRAP:
			ui_order_grab_treat(order_control->id,order_control->param);
			break;
			
		default:
			break;
	}
	
	DbgFuncExit();		
}

/** 
* @brief 	  	发送历史订单列表到UI
* 
*/
void ui_send_history_order_list(unsigned char display_type)
{
	ui_protocol_header_t *header;
	ui_history_info_t *history_info;
	ui_order_info_t *ui_order;
	save_order_t order_info;	
	unsigned char *send_buf;
	unsigned char *data_buf;
	int send_len = 0;
	int order_msg_len = 0;
	int i;
	
	DbgFuncEntry();

	order_get_info(&order_info);

	order_msg_len = order_info.num*sizeof(ui_order_info_t);

	for(i=0;i<order_info.num;i++)
	{
		order_msg_len = order_msg_len + strlen((char*)order_info.order_msg[i].phone) + 1;//1:sizeof '/0'
		order_msg_len = order_msg_len + strlen((char*)order_info.order_msg[i].description) + 1;//1:sizeof '/0'
	}
	
	send_len = 3 + sizeof(ui_protocol_header_t) + order_msg_len + sizeof(ui_history_info_t);//3:7e+crc+7e	

	send_buf = malloc(send_len);

	header = (ui_protocol_header_t*)(send_buf + 1);//1:7e

	ui_protocol_build_header(header,SYSTEM2UI_HISTORY_INFO,order_msg_len + sizeof(ui_history_info_t));

	history_info = (ui_history_info_t*)(send_buf + 1 + sizeof(ui_protocol_header_t));

	history_info->type = display_type;
	history_info->num = (unsigned char)order_info.num;
	history_info->start_id = 0;
	history_info->end_id = order_info.num;

	data_buf = (unsigned char*)(send_buf + 1 + sizeof(ui_protocol_header_t) + sizeof(ui_history_info_t));

	for(i=0;i<order_info.num;i++)
	{
		ui_order = (ui_order_info_t *)data_buf;
		ui_order->state = order_info.order_msg[i].state;
		ui_order->id = order_info.order_msg[i].order_id;
		ui_order->type = order_info.order_msg[i].order_type;
		ui_order->passenger_lon = order_info.order_msg[i].passenger_lon;
		ui_order->passenger_lat = order_info.order_msg[i].passenger_lat;
		ui_order->destination_lon = order_info.order_msg[i].destination_lon;
		ui_order->destination_lat = order_info.order_msg[i].destination_lat;

		data_buf = data_buf + sizeof(ui_order_info_t);
		
		memcpy(data_buf,order_info.order_msg[i].phone,strlen((char*)order_info.order_msg[i].phone) + 1);
		data_buf = data_buf + strlen((char*)order_info.order_msg[i].phone) + 1;

		memcpy(data_buf,order_info.order_msg[i].description,strlen((char *)order_info.order_msg[i].description) + 1);		
		data_buf = data_buf + strlen((char*)order_info.order_msg[i].description) + 1;
	}

	ui_protocol_send_data(send_buf,send_len);

	free(send_buf);
	
	DbgFuncExit();

}

