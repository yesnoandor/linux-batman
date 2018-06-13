/****************************************Copyright (c)**************************************************                         
** File name:			demo_call.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-23
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

#include	"module/gb905/phone_book/gb905_phone_book.h"
#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_call.h"

#include	"middleware/uart/fleety_uart.h"


#include	"app/call/fleety_call.h"

#define		DEBUG_Y
#include	"libs/debug.h"

//falg:1  phone_num:18721127972
static unsigned char buf[]={
		0x01,0x31,0x38,0x37,0x32,
		0x31,0x31,0x32,0x37,0x39,
		0x37,0x32,0x00
};

int demo_call(void) 
{
	DbgFuncEntry();
	
	fleety_uart_init();
	fleety_call_function_init();
	
	ui_call_out_treat(buf,strlen((char *)buf));

	DbgFuncExit();
	
	return 0;
}  
  

