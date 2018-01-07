/****************************************Copyright (c)**************************************************                         
** File name:			gb905_order.h
** Descriptions:		 
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
#ifndef _GB905_ORDER_H
#define _GB905_ORDER_H
	
#ifdef __cplusplus
extern "C" {
#endif


// 下发订单简明信息结构
typedef struct{
	unsigned int id;						// 订单id
	unsigned char type;						// 订单类型(0:即时召车;1:预约召车;2:车辆指派)
	gb905_timestamp_t ride_time;			// 要车时间(YYMMDDhhmmss)
	unsigned char description;				// 业务描述首字节(对乘客要车大概地点的描述)
}__packed gb905_order_briefing_t;


//下发订单详情信息结构
typedef struct{
	unsigned int order_id;						//订单id(若无后续内容则表示该订单未中标)
	unsigned char order_type;					//订单类型(0:即时召车;1:预约召车;2:车辆指派)
	gb905_timestamp_t ride_time;				//要车时间(YYMMDDhhmmss 全0表示不启用)
	unsigned int passenger_lon;					//乘客位置经度(0.0001'  填0表示不启用)
	unsigned int passenger_lat;					//乘客位置纬度(0.0001'  填0表示不启用)
	unsigned int destination_lon;				//目的地位置经度(0.0001'  填0表示不启用)
	unsigned int destination_lat;				//目的地位置纬度(0.0001'  填0表示不启用)
	unsigned char service_fee[2];				//电召服务费(格式为XXX_X  ,全0表示无服务费)
	unsigned char details;						//后续详情首字节(后续包括电话号码和业务详情描述)
}__packed gb905_order_details_t;

//下发取消订单信息结构
typedef struct{
	unsigned int order_id;						//订单id
}__packed gb905_order_cancel_t;

//驾驶员抢单信息结构
typedef struct{
	unsigned int order_id;						//订单id(对应简明中的订单ID)
}__packed driver_grap_order_body_t;

//上传驾驶员抢单的整体结构
typedef struct{
	unsigned char start_magic_id;
	gb905_header_t header;
	driver_grap_order_body_t grap_order;
	unsigned char xor;
	unsigned char end_magic_id;
}__packed gb905_driver_grap_order_t;

//驾驶员取消信息结构
typedef struct{
	unsigned int order_id;						//订单id
	unsigned char cancel_reason;				//取消原因(0:路堵;1:事故;2:其他)
}__packed driver_cancel_order_body_t;

//上传驾驶员取消订单的整体结构
typedef struct{
	unsigned char start_magic_id;
	gb905_header_t header;
	driver_cancel_order_body_t cancel_order;
	unsigned char xor;
	unsigned char end_magic_id;
}__packed gb905_driver_cancel_order_t;

//驾驶员完成订单信息结构
typedef struct{
	unsigned int order_id;						//订单id(对应简明中的订单ID)
}__packed driver_finish_order_body_t;

//上传驾驶员完成订单的整体结构
typedef struct{
	unsigned char start_magic_id;
	gb905_header_t header;
	driver_finish_order_body_t finish_order;
	unsigned char xor;
	unsigned char end_magic_id;
}__packed gb905_driver_finish_order_t;


unsigned char gb905_order_briefing_treat(unsigned char *buf,int len);
unsigned char gb905_order_detials_treat(unsigned char *buf,int len);
unsigned char gb905_order_cancel_treat(unsigned char *buf,int len);

void gb905_driver_grap_order_treat(unsigned int order_id);
void gb905_driver_cancel_order_treat(unsigned int order_id,unsigned char cancel_reason);
void gb905_driver_finish_order_treat(unsigned int order_id);


#ifdef __cplusplus
}
#endif
	
#endif

