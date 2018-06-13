/****************************************Copyright (c)**************************************************                         
** File name:			order.h
** Descriptions:		 
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

#ifndef _ORDER_H
#define _ORDER_H

#ifdef __cplusplus
extern "C" {
#endif

#define		MAX_ORDER_PHONE_CHARS_SIZE		12
#define		MAX_ORDER_DESC_CHARS_SIZE		64
#define		MAX_ORDER_NUM					100		

enum{
	ORDER_STATUS_IDLE = 0,
	ORDER_STATUS_BRIEF,
	ORDER_STATUS_BRIEF_ACK,
	ORDER_STATUS_CRAB,
	ORDER_STATUS_CRAB_ACK,
	ORDER_STATUS_DETAIL,
	ORDER_STATUS_DETAIL_ACK,
	ORDER_STATUS_COMPLETE,
	ORDER_STATUS_COMPLETE_ACK,
	ORDER_STATUS_ISU_CANCEL,
	ORDER_STATUS_ISU_CANCEL_ACK,
	ORDER_STATUS_SERVER_CANCEL,
	ORDER_STATUS_SERVER_CANCEL_ACK,
};


// 下发订单简明信息结构
typedef struct{
	unsigned int id;								// 订单id
	unsigned char type;								// 订单类型(0:即时召车;1:预约召车;2:车辆指派)
	gb905_timestamp_t ride_time;					// 要车时间(YYMMDDhhmmss)
	char description[MAX_ORDER_DESC_CHARS_SIZE];	// 业务描述首字节(对乘客要车大概地点的描述)
}__packed order_info_brief_t;


// 下发订单详情信息结构
typedef struct{
	unsigned int order_id;							// 订单id(若无后续内容则表示该订单未中标)
	unsigned char order_type;						// 订单类型(0:即时召车;1:预约召车;2:车辆指派)
	gb905_timestamp_t ride_time;					// 要车时间(YYMMDDhhmmss 全0表示不启用)
	unsigned int passenger_lon;						// 乘客位置经度(0.0001'  填0表示不启用)
	unsigned int passenger_lat;						// 乘客位置纬度(0.0001'  填0表示不启用)
	unsigned int destination_lon;					// 目的地位置经度(0.0001'  填0表示不启用)
	unsigned int destination_lat;					// 目的地位置纬度(0.0001'  填0表示不启用)
	unsigned char service_fee[2];					// 电召服务费(格式为XXX_X  ,全0表示无服务费)
	char phone[MAX_ORDER_PHONE_CHARS_SIZE];			// 乘客电话号码
	char description[MAX_ORDER_DESC_CHARS_SIZE];	// 订单详情描述
}__packed order_info_detail_t;

// 
typedef struct {
	unsigned char status;
	order_info_brief_t brief;
	order_info_detail_t detail;
}order_info_t;


// 历史订单保存信息结构
typedef struct{
	unsigned char state;								//订单状态
	unsigned int order_id;							// 订单id(若无后续内容则表示该订单未中标)
	unsigned char order_type;						// 订单类型(0:即时召车;1:预约召车;2:车辆指派)
	unsigned int passenger_lon;						// 乘客位置经度(0.0001'  填0表示不启用)
	unsigned int passenger_lat;						// 乘客位置纬度(0.0001'  填0表示不启用)
	unsigned int destination_lon;					// 目的地位置经度(0.0001'  填0表示不启用)
	unsigned int destination_lat;					// 目的地位置纬度(0.0001'  填0表示不启用)
	char phone[MAX_ORDER_PHONE_CHARS_SIZE];			// 乘客电话号码
	char description[MAX_ORDER_DESC_CHARS_SIZE];	// 订单详情描述
}__packed history_order_info_t;


typedef struct{
	unsigned int num;									//最大保存订单的数量
	history_order_info_t order_msg[MAX_ORDER_NUM];		//订单数据结构
}__packed save_order_t;

void get_order_brief_info(unsigned char * buf);
void set_order_brief_info(unsigned char * buf);
void debug_order_brief_info(void);


void get_order_detail_info(unsigned char * buf);
void set_order_detail_info(unsigned char * buf);
void debug_order_detail_info(void);

void set_order_status_info(unsigned char state);
unsigned char get_order_status_info(void);

void order_save_info(save_order_t *info);
void order_get_info(save_order_t *info);

#ifdef __cplusplus
}
#endif

#endif


