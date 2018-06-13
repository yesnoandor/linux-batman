/****************************************Copyright (c)**************************************************                         
** File name:			gb905_update_common.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-16
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_UPDATE_COMMON_H
#define	_GB905_UPDATE_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define		UPDATE_ISU_FILE						"isu.bin"
#define		UPDATE_METER_FILE					"meter.bin"
#define		UPDATE_TOPLIGHT_FILE				"toplight.bin"
#define		UPDATE_TSM_FILE						"tsm.bin"

/*
* 描述GB905  无线升级协议的头信息
*/
typedef struct{
	unsigned short magic_id;						// '$$'
	unsigned short msg_id;							// 消息ID
	unsigned short msg_len;							// 消息体属性 （当终端标识第一字节为’0x10’时，消息体属性目前存储的为消息包的大小）
	unsigned short msg_serial_number;				// 消息体流水号 （按发送顺序从0 开始循环累加）
	unsigned char  protocol_version;
	uuid_device_id_t uuid_device_id;				// 终端标识之设备编号
}__packed gb905_update_header_t,*p_gb905_update_header_t;


/*
*  基本信息的数据结构
*/
typedef struct{
	unsigned char device_type;			// 设备类型 =0x00,智能服务终端
	unsigned char vendor_id;			// 厂商编号 =0x02,飞田公司
}__packed gb905_base_info_t;

/*
* 升级信息请求的消息体
*/
typedef gb905_base_info_t	gb905_update_info_req_body_t;


/*
* 升级信息请求的整个消息数据结构
*/
typedef struct
{
    gb905_update_header_t header;
    gb905_update_info_req_body_t body;
    unsigned short crc;
}__packed gb905_update_info_req_t;


/*
* 描述升级信息响应消息体的数据结构
*/
typedef struct{
	gb905_base_info_t base_info;		// 设备类型和厂商编号
	unsigned char hw_version;			// 硬件版本
	unsigned char sw_version[2];		// 软件版本
	unsigned int  total_length;			// 代码总长度(单位：字节)
	unsigned int  check_sum;			// 和校验
	//unsigned int  download_len;							// spec 上没有
	//unsigned short crc;								// spec上没有
}__packed gb905_update_info_rep_body_t;

typedef gb905_update_info_rep_body_t gb905_update_info_t;

/*
* 升级代码请求的消息体
*/
typedef struct{
	gb905_base_info_t base_info;		// 设备类型和厂商编号
	unsigned int address;				// 本次下载的偏移地址
}__packed gb905_update_code_req_body_t;


/*
* 升级代码请求的整个消息数据结构
*/
typedef struct
{
    gb905_update_header_t header;
    gb905_update_code_req_body_t body;
    unsigned short crc;
}__packed gb905_update_code_req_t;


/*
* 描述升级代码响应的数据结构
*/
typedef struct{
	gb905_update_info_req_body_t base_info;
	unsigned int address;							// 起始地址
	unsigned short size;							// 数据包大小
	unsigned char content[512];						// 数据内容
}__packed gb905_update_code_rep_body_t;


typedef gb905_update_info_req_t	gb905_update_complete_req_t;
typedef gb905_base_info_t	gb905_update_complete_req_ack_body_t;


typedef gb905_base_info_t	gb905_update_cancel_req_body_t;


void gb905_update_info_req(void);
void gb905_update_code_req(int offset);
void gb905_update_complete_req(void);


int gb905_update_protocol_ayalyze(unsigned char * buf,int len);
void gb905_update_send_data(unsigned char * buf, int len);

void gb905_update_debug_header(gb905_update_header_t * header);

#ifdef __cplusplus
}
#endif

#endif

