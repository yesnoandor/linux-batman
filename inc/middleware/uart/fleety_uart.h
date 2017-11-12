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
	METAR_UART,
	TOPLIGHT_UART
};
	
	
void fleety_uart_init(void);
void fleety_uart_send(int index,unsigned char *buff,int len);
	
	
#ifdef __cplusplus
}
#endif
	
	
	
#endif
	
	
	


