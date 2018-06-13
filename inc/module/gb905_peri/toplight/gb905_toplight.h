/****************************************Copyright (c)**************************************************                         
** File name:			gb905_toplight.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-18
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_TOPLIGHT_H
#define _GB905_TOPLIGHT_H

#ifdef __cplusplus
extern "C" {
#endif



#define		TOPLIGHT_TIMEOUT		10

// 顶灯星级显示
enum{
	TOPLIGHT_STAR_NONE=0,
	TOPLIGHT_STAR_ONE,
	TOPLIGHT_STAR_TWO,
	TOPLIGHT_STAR_THREE,
	TOPLIGHT_STAR_FOUR,
	TOPLIGHT_STAR_FIVE,
};

typedef struct{
	uuid_device_id_t	uuid_device_id;			// 设备编号
	unsigned char  		hw_version;				// 硬件版本号
	unsigned char 		sw_major_version;		// 软件主版本号
	unsigned char		sw_minor_version;		// 软件次版本号		
	unsigned char		device_state;			// 设备状态
	unsigned char		running_state;			// 工作状态
	
	unsigned char		night_mode;				// 夜间模式
	unsigned char 		rfu[5];
}__packed gb905_toplight_state_t;

typedef struct{
	unsigned char start_hour;					// 起始时间(hhmm)
	unsigned char start_min;
	unsigned char end_hour;						// 停止时间(hhmm)
	unsigned char end_min;
}__packed gb905_toplight_night_parems_t;


// 顶灯升级消息体数据结构
typedef struct{
	unsigned char vendor_id;
	unsigned char hw_version;
	unsigned char main_sw_version;
	unsigned char vice_sw_version;
}__packed gb905_toplight_update_body_t;

// 顶灯升级的完整数据结构
typedef struct{
	gb905_peri_header_t header;
	gb905_toplight_update_body_t body;
	gb905_peri_tail_t tail;
}__packed gb905_toplight_update_t;


int gb905_toplight_protocol_ayalyze(unsigned char * buf,int len);

bool gb905_toplight_query_state(void);
bool gb905_toplight_reset(void);
bool gb905_toplight_set_baudrate(unsigned char band);
bool gb905_toplight_set_operation_status(unsigned char status);
bool gb905_toplight_set_star_status(unsigned char star);
bool gb905_toplight_set_night_mode(unsigned char mode);
int gb905_toplight_set_night_params(gb905_toplight_night_parems_t * night_params);

bool gb905_toplight_set_security(unsigned char* data,unsigned short data_len);
bool gb905_toplight_clear_security(void);

void gb905_toplight_transparent(unsigned char *msg_buf,unsigned short msg_len);

void gb905_toplight_heart_beat_treat(void);

#ifdef __cplusplus
}
#endif
	
#endif

