/****************************************Copyright (c)**************************************************                         
** File name:			gb905_order.c
** Descriptions:		平台下发简明(8B00);平台下发详情(8B01);平台取消订单(8B09);
**					驾驶员抢单(0B01);驾驶员完成订单(0B07);驾驶员取消订单(0B08);
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-05
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

#include	"middleware/socket/fleety_socket.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/order/gb905_order.h"

#define		DEBUG_Y
#include	"libs/debug.h"



//-------------------------------------------------------------------------------------------

/*
* @brief 打印订单简明信息内容
* @param briefing	  	简明信息结构地址
* @order_description	简明业务描述的地址
*
*/
static void gb905_debug_order_briefing(gb905_order_briefing_t *briefing,char *order_description)
{
	DbgFuncEntry();

	DbgPrintf("briefing order id is %d\r\n",EndianReverse32(briefing->id));
	DbgPrintf("briefing order type is 0x%02x\r\n",briefing->type);
	DbgPrintf("briefing order description is %s\r\n",order_description);
	
	DbgFuncExit();
}


/*
* @brief 打印订单详情信息内容
* @param details	  	详情信息结构地址
* @phone_num		乘客电话电话号码地址
* @order_details		业务详情信息描述的地址
*
*/
static void gb905_debug_order_details(gb905_order_details_t *details,char *phone_num,char *order_details)
{
	DbgFuncEntry();

	DbgPrintf("details order id is %d\r\n",EndianReverse32(details->order_id));
	DbgPrintf("details order type is 0x%02x\r\n",details->order_type);
	DbgPrintf("details order time is %02x-%02x-%02x %02x:%02x:%02x\r\n",details->ride_time.bcd[0],\
		details->ride_time.bcd[1],details->ride_time.bcd[2],details->ride_time.bcd[3],\
		details->ride_time.bcd[4],details->ride_time.bcd[5]);
	
	DbgPrintf("details order passenger lon is %d\r\n",details->passenger_lon);
	DbgPrintf("details order passenger lat is %d\r\n",details->passenger_lat);
	DbgPrintf("details order destination lon is %d\r\n",details->destination_lon);
	DbgPrintf("details order destination lat is %d\r\n",details->destination_lat);
	
	DbgPrintf("details order service fee is %d-%d\r\n",details->service_fee[0],details->service_fee[1]);

	DbgPrintf("details order phone num is %s\r\n",phone_num);
	DbgPrintf("details order details is %s\r\n",order_details);
	
	DbgFuncExit();
}


/*
* @brief 	构造抢单信息
* @param grap_order	  整个抢单信息的结构地址
* @param order_id		  所抢单的订单ID
*
*/
static void gb905_build_order_grap(gb905_driver_grap_order_t *grap_order,unsigned int order_id)
{
	DbgFuncEntry();

    gb905_build_header(&grap_order->header,MESSAGE_ORDER_GRAB,sizeof(gb905_driver_grap_order_t));

    grap_order->grap_order.order_id = order_id;
	
	DbgFuncExit();	
}

/*
* @brief 	构造取消订单的信息
* @param cancel_order	  整个取消订单信息的结构地址
* @param order_id		  所取消的订单ID
* @param cancel_reason 取消订单的原因
* 
*/
static void gb905_build_order_cancel(gb905_driver_cancel_order_t *cancel_order,unsigned int order_id,unsigned char cancel_reason)
{
	DbgFuncEntry();

	gb905_build_header(&cancel_order->header,MESSAGE_ORDER_CANCEL_UP,sizeof(gb905_driver_cancel_order_t));

	cancel_order->cancel_order.order_id = order_id;
	cancel_order->cancel_order.cancel_reason = cancel_reason;
	
	DbgFuncExit();	
}

/*
* @brief 	构造完成订单的信息
* @param finish_order	  整个完成订单信息的结构地址
* @param order_id		  所完成的订单ID
* 
*/
static void gb905_build_order_finish(gb905_driver_finish_order_t *finish_order,unsigned int order_id)
{
	DbgFuncEntry();

	gb905_build_header(&finish_order->header,MESSAGE_ORDER_FINISH,sizeof(gb905_driver_finish_order_t));

	finish_order->finish_order.order_id = order_id;
	
	DbgFuncExit();	
}

/**
* @brief			订单简明下发消息的处理
* @param buf	  	订单简明下发消息体的数据地址
* @param len	  	订单简明下发消息体的数据长度
*
* @return  返回终端通用应答的状态
*/
unsigned char gb905_order_briefing_treat(unsigned char *buf,int len)
{
	gb905_order_briefing_t *briefing;
	char *point;
	
	DbgFuncEntry();

	briefing = (gb905_order_briefing_t *)buf;
	point = (char *)&briefing->description;	//指针指向业务描述的内容

	gb905_debug_order_briefing(briefing,point);
	
	DbgFuncExit();

	return GB905_RESULT_OK;
}

/**
* @brief			订单详情下发消息的处理
* @param buf	  	订单详情下发消息体的数据地址
* @param len	  	订单详情下发消息体的数据长度
*
* @return  返回终端通用应答的状态
*/
unsigned char gb905_order_detials_treat(unsigned char *buf,int len)
{
	gb905_order_details_t *details;
	char *phone_point;
	char *details_point;
	
	DbgFuncEntry();

	//若订单id之后无其他的字段,则表示未中标
	if(len == sizeof(details->order_id))
	{
	
		//抢标失败处理

		return GB905_RESULT_OK;
	}

	details = (gb905_order_details_t *)buf;

	phone_point = (char *)&details->details;//指针指向电话号码

	details_point = phone_point + strlen(phone_point) +1;//指针指向业务详情描述 1:skip '/0'

	gb905_debug_order_details(details,phone_point,details_point);
	
	DbgFuncExit();	

	return GB905_RESULT_OK;
}

/**
* @brief			订单取消下发消息的处理
* @param buf	  	订单取消下发消息体的数据地址
* @param len	  	订单取消下发消息体的数据长度
*
* @return  返回终端通用应答的状态
*/
unsigned char gb905_order_cancel_treat(unsigned char *buf,int len)
{
	gb905_order_cancel_t *cancel;
	DbgFuncEntry();

	cancel = (gb905_order_cancel_t *)buf;

	DbgPrintf("cancel oredr id is %d \r\n",EndianReverse32(cancel->order_id));

	//语音提示驾驶员订单已被取消
	
	DbgFuncExit();

	return GB905_RESULT_OK;
}


/*
* @brief 发送抢单信息到服务器
* @param order_id	  所抢订单的ID
*
*/
void gb905_driver_grap_order_treat(unsigned int order_id)
{
	gb905_driver_grap_order_t grap_order;

	DbgFuncEntry();

	gb905_build_order_grap(&grap_order,order_id);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&grap_order,sizeof(gb905_driver_grap_order_t));
	
	DbgFuncExit();
}

/*
* @brief 					发送取消订单的信息到服务器
* @param order_id	 		所取消订单的ID
* @param cancel_reason	取消订单的原因
*
*/
void gb905_driver_cancel_order_treat(unsigned int order_id,unsigned char cancel_reason)
{

	gb905_driver_cancel_order_t cancel_order;
	
	DbgFuncEntry();

	gb905_build_order_cancel(&cancel_order,order_id,cancel_reason);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&cancel_order,sizeof(gb905_driver_cancel_order_t));
	
	DbgFuncExit();	
}

/*
* @brief 发送完成订单的信息到服务器
* @param order_id	  所完成订单的ID
*
*/
void gb905_driver_finish_order_treat(unsigned int order_id)
{

	gb905_driver_finish_order_t finish_order;
	
	DbgFuncEntry();

	gb905_build_order_finish(&finish_order,order_id);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&finish_order,sizeof(gb905_driver_finish_order_t));
	
	DbgFuncExit();	
}


