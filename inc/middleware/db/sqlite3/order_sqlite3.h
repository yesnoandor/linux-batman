/****************************************Copyright (c)**************************************************                         
** File name:			order_sqlite3.h
** Descriptions:		历史订单数据库信息
*				历史订单信息数据库表字段如下:
*
* 				@id				序列ID(数据库自动生成)
* 				@order_id			订单ID
* 				@order_type			订单类型
* 				@order_state			订单状态
*				@start_lon			乘客经度
*				@start_lat			乘客纬度
*				@end_lon			目的地经度
*				@end_lat				目的地纬度
*				@phone_number		乘客电话
*				@descriptor			订单详情
**

**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-24
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _ORDER_SQLITE3_H
#define _ORDER_SQLITE3_H

#ifdef __cplusplus
extern "C" {
#endif
#include	"libs/sqlite3/sqlite3.h"

int order_sqlite3_init(sqlite3 * db);
int order_sqlite3_insert_record(void);
int order_sqlite3_update_state(void);
int order_sqlite3_restore_record(void);


#ifdef __cplusplus
}
#endif
	
#endif

