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


#define 	__packed 				__attribute__((packed))

#define		meter_uart_device		""		//"/dev/ttyUSB0"
#define		toplight_uart_device	""					//"/dev/ttyUSB1"
#define		tsm_uart_device			""
#define		gps_uart_device			""
#define		mcu_uart_device			""



#define		auth_server_ip_addr		"218.90.157.214"
#define		auth_server_port		8688
#define		main_server_ip_addr		"192.168.0.102"			//"192.168.62.34"	//"180.168.34.250"
#define		main_server_port		10005					//10004			//5285
#define		aux_server_ip_addr		""
#define		aux_server_port			0
#define		ui_server_ip_addr		""
#define		ui_server_port			7777

#ifdef __cplusplus
}
#endif

#endif

