/****************************************Copyright (c)**************************************************                         
** File name:			ubuntu.h
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
#ifndef _UBUNTUN_H
#define	_UBUNTUN_H


#ifdef __cplusplus
extern "C" {
#endif

#include	"platform/ubuntu/ubuntu_keys.h"
#include	"platform/ubuntu/ubuntu_relay.h"
#include	"platform/ubuntu/ubuntu_imei.h"
#include	"platform/ubuntu/ubuntu_system.h"


#define 	__packed 				__attribute__((packed))

#define		meter_uart_device		""		//"/dev/ttyUSB0"
#define		toplight_uart_device	""		//"/dev/ttyUSB1"
#define		tsm_uart_device			""
#define		gps_uart_device			""
#define		mcu_uart_device			""
#define		gprs_uart_device		""

#define		meter_uart_bps			B9600
#define		toplight_uart_bps		B9600
#define		tsm_uart_bps			B9600
#define		gps_uart_bps			B9600
#define		mcu_uart_bps			B115200
#define		gprs_uart_bps			B9600

// "180.168.34.250"	5606	:  ÍôÑóµÄÍâÍø¶Ë¿Ú	

#define		auth_server_ip_addr		"218.90.157.214"
#define		auth_server_port		8688
#define		main_server_ip_addr		"180.168.34.250"			//"192.168.62.34"	//"180.168.34.250"
#define		main_server_port		5606					//10004			//5285
#define		aux_server_ip_addr		""
#define		aux_server_port			0
#define		ui_server_ip_addr		""						//"192.168.1.7"
#define		ui_server_port			0						//8090

#define		DB_FILE_PATH				""

#ifdef __cplusplus
}
#endif

#endif

