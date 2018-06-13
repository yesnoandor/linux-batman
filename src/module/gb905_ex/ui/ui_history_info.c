/****************************************Copyright (c)**************************************************                         
** File name:			ui_histoory_info.c
** Descriptions:		UI(与APK信息交互)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-22
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

#include	"middleware/db/sqlite3/phone_book_sqlite3.h"
#include	"middleware/db/sqlite3/event_sqlite3.h"
#include	"middleware/db/sqlite3/notice_sqlite3.h"
#include	"middleware/db/sqlite3/order_sqlite3.h"

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_history_info.h"
#include	"module/gb905_ex/ui/ui_event.h"
#include	"module/gb905_ex/ui/ui_notice.h"
#include	"module/gb905_ex/ui/ui_phone_book.h"
#include	"module/gb905_ex/ui/ui_order.h"

#define		DEBUG_Y
#include	"libs/debug.h"

enum{
	DISP_TYPE_PHONE_BOOK = 1,
	DISP_TYPE_EVENT,
	DISP_TYPE_NOTICE,
	DISP_TYPE_ORDER,
};

void ui_history_info_display_treat(unsigned char *buf,int len)
{
	unsigned char display_type;
	
	DbgFuncEntry();

	display_type = *buf;

	DbgPrintf("ui query display type is 0x%02x\r\n",display_type);
	
	switch(display_type)
	{
		case DISP_TYPE_PHONE_BOOK:
			phone_book_sqlite3_restore_record();
			break;

		case DISP_TYPE_EVENT:
			event_sqlite3_restore_record();
			break;

		case DISP_TYPE_NOTICE:
			notice_sqlite3_restore_record();
			break;

		case DISP_TYPE_ORDER:
			order_sqlite3_restore_record();
			break;

		default:
			DbgWarn("this type is not support!!!\r\n");
			break;
	}
	ui_send_history_info(display_type);
	
	DbgFuncExit();
}

void ui_send_history_info(unsigned char display_type)
{
	DbgFuncEntry();

	switch(display_type)
	{
		case DISP_TYPE_PHONE_BOOK:
			ui_send_phone_book_list(display_type);
			break;

		case DISP_TYPE_EVENT:
			ui_send_history_event_list(display_type);
			break;

		case DISP_TYPE_NOTICE:
			ui_send_history_notice_list(display_type);
			break;

		case DISP_TYPE_ORDER:
			ui_send_history_order_list(display_type);
			break;

		default:
			DbgWarn("this type is not support!!!\r\n");
			break;
	}
	DbgFuncExit();
}
