/****************************************Copyright (c)**************************************************                         
** File name:			gb905_common.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-01-19
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_COMMON_H
#define	_GB905_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

//#define		PROTOCOL_2011	0
//#define		PROTOCOL_2014	1

#define		MESSAGE_GENERAL_UP_ACK				0x0001
#define		MESSAGE_HEART_BEAT					0x0002
#define		MESSAGE_GET_PARAMS_ACK				0x0104
#define		MESSAGE_UPDATE_RESULT				0x0105
#define		MESSAGE_LOCATION_REPORT				0x0200
#define		MESSAGE_GET_LOCATION_ACK			0x0201
#define		MESSAGE_LOCATION_TRACE				0x0202
#define		MESSAGE_LOCATION_SUPPLEMENT_REPORT	0x0203
#define		MESSAGE_EVENT_LIST_ACK				0x0301
#define		MESSAGE_QUESTION_ACK				0x0302
#define 	MESSAGE_REMOTE_CTRL_ACK				0x0500
#define		MESSAGE_PICTURE_UPLOAD				0x0800
#define		MESSAGE_PICTURE_SEARCH_ACK			0x0802
#define		MESSAGE_RECORD_SEARCH_ACK			0x0805
#define		MESSAGE_AUDIO_UPLOAD				0x0806
#define		MESSAGE_ORDER_GRAB					0x0B01

#define		MESSAGE_LOGIN_MSG					0x0B03
#define		MESSAGE_LOGOUT_MSG					0x0B04
#define		MESSAGE_OPERATION_REPORT			0x0B05

#define		MESSAGE_ORDER_FINISH				0x0B07
#define		MESSAGE_ORDER_CANCEL_UP				0x0B08
#define		MESSAGE_PERIPHERY_UP				0x0B10

#define		MESSAGE_OBD_RT_REPORT				0x0F01
#define		MESSAGE_OBD_HBT_REPORT				0x0F02
#define		MESSAGE_OBD_TT_REPORT				0x0F03
#define		MESSAGE_OBD_GET_PARAMS_ACK			0x0F05
#define		MESSAGE_OBD_TRANSPARENT_ACK			0x0F06

#define		MESSAGE_QUERY_TERMINAL_VERSION_ACK	0X0F07
#define		MESSAGE_SEND_TERMIANL_VERSION		0X0F08

#define		MESSAGE_GENERAL_DOWN_ACK			0x8001
#define		MESSAGE_SET_PARAMS					0x8103
#define		MESSAGE_GET_PARAMS					0x8104
#define		MESSAGE_CTRL_TERMINAL				0x8105
#define		MESSAGE_GET_LOCATION				0x8201
#define		MESSAGE_TRACE_LOCATION				0x8202
#define		MESSAGE_TEXT_NOTICE					0x8300
#define		MESSAGE_EVENT_LIST					0x8301
#define		MESSAGE_QUESTION					0x8302
#define		MESSAGE_CALLBACK					0x8400
#define		MESSAGE_PHONE_BOOK					0x8401
#define		MESSAGE_REMOTE_CTRL					0x8500
#define		MESSAGE_PICTURE_CTRL				0x8801
#define		MESSAGE_PICTURE_SEARCH				0x8802
#define		MESSAGE_AV_UPLOAD_REQUEST			0x8803
#define		MESSAGE_RECORD_SEARCH				0x8805
#define		MESSAGE_ORDER_INFO					0x8B00 
#define		MESSAGE_ORDER_GRAB_ACK				0x8B01
#define		MESSAGE_ORDER_CANCEL_DOWN			0x8B09

#define		MESSAGE_CONFIRM_ALARM				0x8B0A
#define		MESSAGE_CANCEL_ALARM				0x8B0B
#define		MESSAGE_PERIPHERY_DOWN				0x8B10

#define		MESSAGE_DEVICE_INSPECTION			0x8B11
#define		MESSAGE_INSPECTION_ACK				0x0B11

#define		MESSAGE_OBD_GET_HBT_REPORT			0x8F02
#define		MESSAGE_OBD_SET_PARAMS				0x8F04
#define		MESSAGE_OBD_GET_PARAMS				0x8F05
#define		MESSAGE_OBD_TRANSPARENT				0x8F06

#define	  	MESSAGE_QUERY_TERMINAL_VERSION		0X8F07

#define		MESSAGE_DOWN_UPGRADE_CMD			0X8F09
#define		MESSAGE_SEND_UPGRADE_DATE			0X8F0A

//----------------以下为深圳市地标协议-----------------------------------------------------------------
#define		BB_ID_CP_REAL_AV_REQ				0x9501 //[sb.AV]实时音视频传输请求(A.2.7.39)
#define		BB_ID_CP_REAL_AV_CTL		 		0x9102 //[sb.AV]实时音视频传输控制(A.2.7.40)
#define		BB_ID_TE_REAL_AV_REG		 		0x1209 //[sb.AV]实时音视频媒体通道注册(A.2.7.41)
#define		BB_ID_CP_FILE_SEARCH		 		0x9505 //[sb.FILE]存储音视频检索(A.2.7.42)
#define		BB_ID_TE_FILE_LIST			 		0x1505 //[sb.FILE]终端上传音视频资源列表(A.2.7.43)
#define		BB_ID_CP_FILE_MONTH_SEARCH	 		0x9508 //[sb.FILE]终端搜索月历信息列表(A.2.7.44)
#define		BB_ID_TE_FILE_MONTH_LIST	 		0x1508 //[sb.FILE]月历查询应答(A.2.7.45)
#define		BB_ID_CP_FILE_UPLOAD_REQ	 		0x950b //[sb.FILE]文件上传指令(A.2.7.46)
#define		BB_ID_TE_FILE_UPLOAD_REG	 		0x120A //[sb.FILE]文件传输注册(A.2.7.46.1)
#define		BB_ID_CP_RECORD_PLAY_REQ	 		0x9509 //[sb.RECORD]平台下发远程录像回放请求(A.2.7.47)
#define		BB_ID_CP_RECORD_PLAY_CTL	 		0x950A //[sb.RECORD]平台下发远程录像回放控制(A.2.7.48)


enum{
	GB905_RESULT_OK = 0,
	GB905_RESULT_FAIL,
	GB905_RESULT_UNKNOWN,
};

/*
* 描述设备编号信息
*/
typedef struct
{
	unsigned char vendor_id;						// 厂商编号 =0x02,飞田公司
	unsigned char product_id;						// 设备类型 =0x00,智能服务终端
	unsigned char mtd_id[3];						// 序列号
}__packed uuid_device_id_t;

/*
* 描述GB905  协议的头信息
*/
typedef struct{
	unsigned short msg_id;							// 消息ID
	unsigned short msg_len;							// 消息体属性 （当终端标识第一字节为’0x10’时，消息体属性目前存储的为消息包的大小）
	unsigned char msg_vendor_type;					// 终端标识第一字节为’0x10’
	uuid_device_id_t uuid_device_id;				// 终端标识之设备编号
	unsigned short msg_serial_number;				// 消息体流水号 （按发送顺序从0 开始循环累加）
}__packed gb905_header_t ,*p_gb905_header_t;


// 通用应答的消息体信息数据格式
typedef struct{
	unsigned short seq;
	unsigned short id;
	unsigned char result;
}__packed ack_body_t;

//通用应答的整个消息数据格式 
typedef  struct
{
    unsigned char start_magic_id;
    gb905_header_t header;
    ack_body_t ack_body;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed gb905_ack_t;

// 用BCD  码表示时间
typedef struct {
	unsigned char bcd[6];				// YY-MM-DD-hh-mm-ss
}__packed gb905_bcd_timestamp_t;

// 用BCD  码表示时间
typedef struct {
	unsigned char bcd[6];				// YYYY-MM-DD-hh-mm
}__packed gb905_bcd_login_time_t;

typedef union 
{
	unsigned id;
	
	struct
	{
		// low --> high
		unsigned sec:6;			// 秒 （0～59）
		unsigned min:6;			// 分 （0～59）
		unsigned hour:5;		// 时 （0～23）
		unsigned mday:5;		// 天 （1～31）
		unsigned mon:4;			// 月 （1～12）
		unsigned year:6;		// 年 （2000～2064）
    }timestamp;
}gb905_timestamp_id_t;


void gb905_build_header(gb905_header_t * header, unsigned short msg_id, unsigned short msg_len);
void gb905_build_timestamp(gb905_bcd_timestamp_t * timestamp);
unsigned int gb905_build_timestamp_id(void);

void gb905_send_data(unsigned char socket_index,unsigned char * buf, int len);
void gb905_send_ack(gb905_header_t * header,unsigned char result);

int gb905_protocol_ayalyze(unsigned char * buf,int len);

void gb905_debug_header(gb905_header_t * header);
void gb905_debug_ack(ack_body_t * ack);

#ifdef __cplusplus
}
#endif

#endif

