/****************************************Copyright (c)**************************************************                         
** File name:			hi3520d.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-07
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _HI3520D_H
#define	_HI3520D_H


#ifdef __cplusplus
extern "C" {
#endif

#define 	__packed 				__attribute__((packed))

#define		meter_uart_device		"/dev/ttyAMA2"			// "/dev/ttyAMA2"
#define		toplight_uart_device	"/dev/ttyAMA3"			// "/dev/ttyAMA3"
#define		tsm_uart_device			"/dev/ttyACM0"			// "/dev/ttyACM0"
#define		gps_uart_device			"/dev/ttyACM1"			// "/dev/ttyACM1"
#define		mcu_uart_device			"/dev/ttyACM4"			// "/dev/ttyACM4"
#define		gprs_uart_device		"/dev/ttyUSB2"	

#define		meter_uart_bps			B9600
#define		toplight_uart_bps		B9600
#define		tsm_uart_bps			B9600
#define		gps_uart_bps			B9600
#define		mcu_uart_bps			B115200
#define		gprs_uart_bps			B9600


#define		auth_server_ip_addr		"218.90.157.214"
#define		auth_server_port		8688
#define		main_server_ip_addr		"180.168.34.250"		// "192.168.62.34"	//"180.168.34.250"
#define		main_server_port		5606					// 10004			//5285
#define		aux_server_ip_addr		""
#define		aux_server_port			0
#define		ui_server_ip_addr		""
#define		ui_server_port			0

#define		DB_FILE_PATH			"/mnt/fleety/"

#ifdef __cplusplus
}
#endif

#endif

