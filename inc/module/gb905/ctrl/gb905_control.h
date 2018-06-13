/****************************************Copyright (c)**************************************************                         
** File name:			gb905_control.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-27
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _GB905_CONTROL_H
#define _GB905_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

enum{
	GB905_UPDATE_NONE=0,						// 版本号一致，无需升级
	GB905_UPDATE_OK,							// 升级成功
	GB905_UPDATE_FAIL,							// 升级失败
	GB905_UPDATE_VENDOR_MISMATCH,				// 厂商标识不一致
	GB905_UPDATE_HW_MISMATCH,					// 硬件版本号不一致
	GB905_UPDATE_DOWNLOAD_FAIL,					// 下载升级文件失败
	GB905_UPDATE_SERVER_CANCEL,					// 服务器主动取消
	GB905_UPDATE_ISU_CANCEL						// ISU 设备主动取消
};



typedef struct{
	unsigned char cmd;							// 命令字
	unsigned int content;						// 命令参数(无线升级才使用)
}__packed gb905_control_body_t;


typedef  struct
{
	unsigned char	device_type;				// 设备类型
	unsigned char	vendor_id;					// 厂商标识
	unsigned char	hw_version;					// 硬件版本号
	unsigned char	sw_version[2];				// 软件版本号
} __packed gb905_update_device_info_t;

typedef  struct
{   
	gb905_update_device_info_t device_info;
 	unsigned char result;
} __packed gb905_update_result_body_t;

typedef  struct
{   
    unsigned char start_magic_id;
    gb905_header_t header;
    
	gb905_update_result_body_t body;
 	
    unsigned char xor;
    unsigned char end_magic_id;
} __packed gb905_update_result_t;

// 无线升级参数
typedef  struct
{
	gb905_update_device_info_t	device_info;

	char 	update_server_apn[32];				// 升级服务器APN，无线通信拨号访问点
	char 	update_server_username[32];			// 升级服务器无线通信拨号用户名
	char 	update_server_password[32];			// 升级服务器无线通信拨号密码
	char 	update_server_ipaddr[32];			// 升级服务器地址,IP 或域名

	unsigned short	update_server_tcp_port;		// 升级服务器TCP 端口
}gb905_update_param_t;


unsigned char gb905_control_treat(unsigned char *buf,int len);

void gb905_send_update_result(unsigned char result);


#ifdef __cplusplus
}
#endif



#endif




