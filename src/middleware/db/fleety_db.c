/****************************************Copyright (c)**************************************************                         
** File name:			fleety_db.c
** Descriptions:		ʵ��λ�û㱨��λ�û㱨Ӧ��ȹ��ܵ�Ӧ���߳�
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
* @brief 	��ʼ�������ļ������ݿ�����õ��ڴ����ݽṹ
*
*/
void fleety_db_init(void)
{
	init_device_info();

	init_setting_params();

	init_product_params();

	init_socket_params();
}
