/****************************************Copyright (c)**************************************************                         
** File name:			order.c
** Descriptions:		订单信息
**						1. 状态
**						2. 简明信息
**						3. 订单详情
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-09
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

#include	"middleware/info/order.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static 		order_info_t		order_info;
static 		save_order_t		order_info_now;

// 得到订单简明信息
void get_order_brief_info(unsigned char * buf)
{
	memcpy(buf,(unsigned char *)&order_info.brief,sizeof(order_info_brief_t));
}


// 设置订单简明信息
void set_order_brief_info(unsigned char * buf)
{
	memset((unsigned char *)&order_info.brief,0x00,sizeof(order_info_brief_t));
	memcpy((unsigned char *)&order_info.brief,buf,sizeof(order_info_brief_t) - MAX_ORDER_DESC_CHARS_SIZE);
	strcpy(order_info.brief.description,(char *)(buf + sizeof(order_info_brief_t) - MAX_ORDER_DESC_CHARS_SIZE));
}

void debug_order_brief_info(void)
{
	DbgFuncEntry();

	DbgPrintf("brief order id is %d\r\n",EndianReverse32(order_info.brief.id));
	DbgPrintf("brief order type is 0x%02x\r\n",order_info.brief.type);

	DbgPrintf("brief order time is %02x-%02x-%02x %02x:%02x:%02x\r\n",
		order_info.brief.ride_time.bcd[0],
		order_info.brief.ride_time.bcd[1],
		order_info.brief.ride_time.bcd[2],
		order_info.brief.ride_time.bcd[3],
		order_info.brief.ride_time.bcd[4],
		order_info.brief.ride_time.bcd[5]);
	DbgPrintf("briefing order description is %s\r\n",order_info.brief.description);
	
	DbgFuncExit();
}


// 得到订单详细信息
void get_order_detail_info(unsigned char * buf)
{
	memcpy(buf,(unsigned char *)&order_info.detail,sizeof(order_info_detail_t));
}


// 设置订单详细信息
void set_order_detail_info(unsigned char * buf)
{	
	memset((unsigned char *)&order_info.detail,0x00,sizeof(order_info_detail_t));
	memcpy((unsigned char *)&order_info.detail,buf,sizeof(order_info_detail_t) - MAX_ORDER_PHONE_CHARS_SIZE - MAX_ORDER_DESC_CHARS_SIZE);

	buf +=  sizeof(order_info_detail_t) - MAX_ORDER_PHONE_CHARS_SIZE - MAX_ORDER_DESC_CHARS_SIZE;
	strcpy(order_info.detail.phone,(char *)buf);
	buf += strlen((char *)buf) + 1;
	strcpy(order_info.detail.description,(char *)buf);
}

void debug_order_detail_info(void)
{
	DbgFuncEntry();

	DbgPrintf("details order id is %d\r\n",EndianReverse32(order_info.detail.order_id));
	DbgPrintf("details order type is 0x%02x\r\n",order_info.detail.order_type);
	DbgPrintf("details order time is %02x-%02x-%02x %02x:%02x:%02x\r\n",
		order_info.detail.ride_time.bcd[0],
		order_info.detail.ride_time.bcd[1],
		order_info.detail.ride_time.bcd[2],
		order_info.detail.ride_time.bcd[3],
		order_info.detail.ride_time.bcd[4],
		order_info.detail.ride_time.bcd[5]);
	
	DbgPrintf("details order passenger lon is %d\r\n",order_info.detail.passenger_lon);
	DbgPrintf("details order passenger lat is %d\r\n",order_info.detail.passenger_lat);
	DbgPrintf("details order destination lon is %d\r\n",order_info.detail.destination_lon);
	DbgPrintf("details order destination lat is %d\r\n",order_info.detail.destination_lat);
	
	DbgPrintf("details order service fee is %d-%d\r\n",order_info.detail.service_fee[0],order_info.detail.service_fee[1]);

	DbgPrintf("details order phone num is %s\r\n",order_info.detail.phone);
	DbgPrintf("details order details is %s\r\n",order_info.detail.description);
	
	DbgFuncExit();
}


void set_order_status_info(unsigned char state)
{
	order_info.status = state;
}

unsigned char get_order_status_info(void)
{
	return order_info.status;
}

// 保存存储的订单信息
void order_save_info(save_order_t *info)
{

	DbgFuncEntry();

	memset(&order_info_now,0x00,sizeof(save_order_t));
	memcpy(&order_info_now,info,sizeof(save_order_t));

	DbgFuncExit();
}

// 获取存储的订单信息
void order_get_info(save_order_t *info)
{
	DbgFuncEntry();
	
	memset(info,0x00,sizeof(save_order_t));
	memcpy(info,&order_info_now,sizeof(save_order_t));

	DbgFuncExit();
}

