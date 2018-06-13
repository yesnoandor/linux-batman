/****************************************Copyright (c)**************************************************                         
** File name:			mtk.h
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
#ifndef _MTK_H
#define	_MTK_H


#ifdef __cplusplus
extern "C" {
#endif

#define		PLATFORM				"mtk"
#define		PRODUCT					"M762"


#define		METER_UART_DEV_NAME			"/dev/ttyMT2"
#define		TOPLIGHT_UART_DEV_NAME		"/dev/ttyMT1"
#define		TSM_UART_DEV_NAME			"/dev/ttyACM1"
#define		GPS_UART_DEV_NAME			""
#define		MCU_UART_DEV_NAME			"/dev/ttyACM4"
#define		GPRS_UART_DEV_NAME			""
#define		INSPECT_UART_DEV_NAME		"/dev/ttyACM2"

#define		METER_UART_DEV_BPS			B9600
#define		TOPLIGHT_UART_DEV_BPS		B9600
#define		TSM_UART_DEV_BPS			B9600
#define		GPS_UART_DEV_BPS			B9600
#define		MCU_UART_DEV_BPS			B115200
#define		GPRS_UART_DEV_BPS			B9600
#define		INSPECT_UART_DEV_BPS		B115200

#define		AUTH_SERVER_IP_ADDR		"218.90.157.214"
#define		AUTH_SERVER_PORT		8688
#define		MAIN_SERVER_IP_ADDR		"180.168.34.250"		//"192.168.62.34"	//"180.168.34.250"
#define		MAIN_SERVER_PORT		5606					//10004			//5285
#define		AUX_SERVER_IP_ADDR		""
#define		AUX_SERVER_PORT			0
#define		UI_SERVER_IP_ADDR		"127.0.0.1"
#define		UI_SERVER_PORT			8090

#define		DB_FILE_PATH			""

#define 	EXPORT_PATH				""
#define		PHOTO_PATH				""

#define		UDISK_PATH				""
#define		STATUS_PATH				""

#define		METER_VENDOR_ID			0x13		// 溧阳03；正尚13
#define		TOPLIGHT_VENDOR_ID		0x00		// 0x10		//debug test 0x02
#define		TSM_VENDOR_ID			0x05		//金坛03；福州05；测试01

#define		DEFAULT_HW_VERSION		"570LL"

#ifdef __cplusplus
}
#endif

#endif

