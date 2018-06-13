/****************************************Copyright (c)**************************************************                         
** File name:			order_sqlite3.h
** Descriptions:		��ʷ�������ݿ���Ϣ
*				��ʷ������Ϣ���ݿ���ֶ�����:
*
* 				@id				����ID(���ݿ��Զ�����)
* 				@order_id			����ID
* 				@order_type			��������
* 				@order_state			����״̬
*				@start_lon			�˿;���
*				@start_lat			�˿�γ��
*				@end_lon			Ŀ�ĵؾ���
*				@end_lat				Ŀ�ĵ�γ��
*				@phone_number		�˿͵绰
*				@descriptor			��������
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

