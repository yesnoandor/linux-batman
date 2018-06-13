/****************************************Copyright (c)**************************************************                         
** File name:			fleety_db.c
** Descriptions:		实现位置汇报，位置汇报应答等功能的应用线程
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-11
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

#include	"middleware/info/device.h"
#include	"middleware/info/setting.h"
#include	"middleware/info/product.h"


#define		DEBUG_Y
#include	"libs/debug.h"


/** 
* @brief 	初始化所有文件和数据库的设置到内存数据结构
*
*/
void fleety_db_init(void)
{
	init_device_info();

	init_setting_params();

	init_product_params();

	init_socket_params();
}
