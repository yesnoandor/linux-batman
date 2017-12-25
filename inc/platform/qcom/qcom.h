/****************************************Copyright (c)**************************************************                         
** File name:			msm8909.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-09
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _MSM8909_H
#define	_MSM8909_H


#ifdef __cplusplus
extern "C" {
#endif


#define		meter_uart_device		"/dev/ttyS0"
#define		toplight_uart_device	"/dev/ttyS1"
#define		tsm_uart_device			"/dev/ttyS1"
#define		gps_uart_device			""
#define		gprs_uart_device		""
#define		mcu_uart_device			""


#define		auth_server_ip_addr		"218.90.157.214"
#define		auth_server_port		8688
#define		main_server_ip_addr		"192.168.1.11"			//"192.168.62.34"	//"180.168.34.250"
#define		main_server_port		10005					//10004			//5285
#define		aux_server_ip_addr		""
#define		aux_server_port			0
#define		ui_server_ip_addr		"127.0.0.1"
#define		ui_server_port			7777

#define		DB_FILE_PATH			""

#ifdef __cplusplus
}
#endif

#endif

