/****************************************Copyright (c)**************************************************                         
** File name:			gb905_uart.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-08
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef __FLEETY_UART_H
#define __FLEETY_UART_H
	
#ifdef __cplusplus
extern "C" {
#endif
	
enum{
	METER_UART = 0,
	TOPLIGHT_UART,
	TSM_UART,
	GPS_UART,
	MCU_UART,
	GPRS_UART,

	MAX_UART_NUM,
};
	
	
void fleety_uart_init(void);
void fleety_uart_send(int index,unsigned char *buff,int len);
	
	
#ifdef __cplusplus
}
#endif
	
	
	
#endif
	
	
	


