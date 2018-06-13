/****************************************Copyright (c)**************************************************                         
** File name:			xmodem_uart.c
** Descriptions:		升级的信息
**						
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
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

#include	"middleware/uart/fleety_uart.h"
#include	"middleware/uart/xmodem_uart.h"


#define		DEBUG_Y
#include	"libs/debug.h"

#define		METER_UART_INDEX		0

static int xmodem_uart_fd = 0;

void xmodem_open(void)
{
	xmodem_uart_fd = fleety_uart_set_mode(METER_UART_INDEX,1);
	//xmodem_uart_fd = fleety_sync_uart_init();
}

void xmodem_close(void)
{
	fleety_uart_set_mode(METER_UART_INDEX,0);
	xmodem_uart_fd = 0;
}

int xmodem_read(unsigned char *buf,int byte)
{
	int nread = 0;

	DbgFuncEntry();

	nread = read(xmodem_uart_fd,buf,byte);

	DbgFuncExit();
	
	return nread;
}


int xmodem_write(unsigned char *buf,int byte)
{
	int nwrite = 0;
	
	DbgFuncEntry();
	
	nwrite = write(xmodem_uart_fd,buf,byte);
	
	DbgFuncExit();

	return nwrite;
}
