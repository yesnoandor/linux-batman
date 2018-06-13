/****************************************Copyright (c)**************************************************                         
** File name:			gb905_meter.h
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
#ifndef _GB905_METER_H
#define _GB905_METER_H

#include	"module/gb905/gb905_common.h"

#include	"module/gb905_peri/gb905_peri_common.h"


#ifdef __cplusplus
extern "C" {
#endif

// 计价器当前状态
typedef union 
{
	unsigned char whole;
	
	struct
	{
		// low --> high
		unsigned char loading:1;	// =0,空车；=1,重车	
		unsigned char login:1;		// =0,签退；=1,登录		
		unsigned char force_on:1;	// =0,正常；=1,强制开机
		unsigned char force_off:1;	// =0,正常；=1,强制关机
		unsigned char :0;
    }flag;
}meter_state_t;




// 用BCD  码表示时间限制
typedef struct {
	unsigned char bcd[5];				// YYYY-MM-DD-hh
}__packed meter_bcd_time_threhold_t;

// 用BCD  码表示当前时间
typedef struct{
	unsigned char bcd[6];				// YYYY-MM-DD-hh-mm
}__packed gb905_meter_timestamp_t;




//计价器状态查询数据结构(ISU -->  METER)
typedef struct{
	gb905_peri_header_t header;
	gb905_peri_bcd_timestamp_t timestamp;
	gb905_peri_tail_t tail;
}__packed gb905_meter_query_state_t;

// 计价器状态消息体数据结构		(METER --> ISU)
typedef struct{
	uuid_device_id_t	uuid_device_id;				// 设备编号
	unsigned char  		hw_version;					// 硬件版本号
	unsigned char 		sw_major_version;			// 软件主版本号		
	unsigned char		sw_minor_version;			// 软件次版本号	
	unsigned char		device_state;				// 设备状态
	unsigned char		running_state;				// 计价器工作状态
	unsigned char		license_plate[6];			// 车牌号
	unsigned char 		company_license_number[16];	// 公司经营许可证号(ASCII码,  不足16  字节，右补0)
	unsigned char 		driver_license_number[19];	// 驾驶员从业资格证号(ASCII码,  不足19  字节，右补0)
	unsigned int		operation_conut;			// 总营运次数
}__packed gb905_meter_state_body_t;


// 计价器参数查询数据结构(ISU -->  METER)
typedef struct{
	gb905_peri_header_t header;
	gb905_peri_tail_t tail;
}__packed gb905_meter_query_param_t;

// 计价器参数消息体数据结构		(METER <--> ISU)
typedef struct{
	meter_bcd_time_threhold_t enable_time;			// 参数启用时间( BCD[5] : YYYY-MM-DD-hh)
	unsigned char price_return_per_day_bcd[2];		// 白天往返单价( BCD[2] : xx.xx yuan/km)
	unsigned char price_return_per_night_bcd[2];	// 夜间往返单价( BCD[2] : xx.xx yuan/km)
	unsigned char price_oneway_per_day_bcd[2];		// 白天单程单价( BCD[2] : xx.xx yuan/km)
	unsigned char price_oneway_per_night_bcd[2];	// 夜间单程单价( BCD[2] : xx.xx yuan/km)
	unsigned char price_subsidy_per_day_bcd[2];		// 白天二次空贴单价( BCD[2] : xx.xx yuan/km)
	unsigned char price_subsidy_per_night_bcd[2];	// 夜间二次空贴单价( BCD[2] : xx.xx yuan/km)
	unsigned char price_start_per_day_bcd[2];		// 白天起步价( BCD[2] : xx.xx yuan/km)
	unsigned char price_start_per_night_bcd[2];		// 夜间起步价( BCD[2] : xx.xx yuan/km)

	unsigned char onword_kilometre_bcd[2];			// 续程里程数( BCD[2] : xx.xx km)
	unsigned char start_kilometre_bcd[2];			// 启程公里( BCD[2] : xx.xx km)
	unsigned char oneway_kilometre_bcd[2];			// 单程公里( BCD[2] : xx.xx km)
	unsigned char empty_subsidy_kilometre_bcd[2];	// 二次空贴公里( BCD[2] : xx.xx km)
	
	unsigned char price_wait_per_day_bcd[2];		// 白天等候单价( BCD[2] : xx.xx yuan)
	unsigned char price_wait_per_night_bcd[2];		// 夜间等候单价( BCD[2] : xx.xx yuan)
	
	unsigned char time_free_wait_bcd[2];			// 免费等候时间( BCD[2] : xxxx min_sec)
	unsigned char time_increase_price_bcd[2];		// 加价时间( BCD[2] : xxxx min_sec)
	unsigned char start_time_per_night_bcd[2];		// 夜间开始时间( BCD[2] : xxxx hour_min)
	unsigned char end_time_per_night_bcd[2];		// 夜间结束时间( BCD[2] : xxxx hour_min)
	unsigned char rfu[22];							// 系统预留
	unsigned char vendor_reserved[64];				// 厂商自定义参数区
}__packed gb905_meter_parameter_body_t;

// 计价器参数设置数据结构(ISU -->  METER)
typedef struct{
	gb905_peri_header_t header;
	gb905_meter_parameter_body_t body;
	gb905_peri_tail_t tail;
}__packed gb905_meter_set_param_t;


// 计价器参数设置应答消息体数据结构
typedef struct{
	unsigned char ack_result;						// 操作结果
	meter_bcd_time_threhold_t enable_time;			// 启用时间
}__packed gb905_meter_set_param_ack_body_t;



// 计价器心跳消息体数据结构
typedef struct{
	meter_state_t meter_state;
	unsigned char company_license_number[16];	// 公司经营许可证号(ASCII码,  不足16  字节，右补0)
	unsigned char driver_license_number[19];	// 驾驶员从业资格证号(ASCII码,  不足19  字节，右补0)
}__packed gb905_meter_heart_beat_body_t;


//计价器心跳应答消息体数据结构
typedef struct{
	unsigned short terminal_state;						// 终端运行状态
	meter_bcd_time_threhold_t time_limit;				// 时间限制
	unsigned char number_of_times_limit;				// 次数限制
	unsigned short rfu;
}__packed gb905_meter_heart_beat_ack_body_t;

//计价器心跳应答数据结构
typedef struct{
	gb905_peri_header_t header;
	gb905_meter_heart_beat_ack_body_t body;
	gb905_peri_tail_t tail;
}__packed gb905_meter_heart_beat_ack_t;







//计价器开机/  关机应答消息体数据结构
typedef struct{
	unsigned char company_license_number[16];			// 公司经营许可证号(ASCII码,  不足16  字节，右补0)
	unsigned char driver_license_number[19];			// 驾驶员从业资格证号(ASCII码,  不足19  字节，右补0)
	unsigned char plate_number[6];						// 车牌号(ASCII 码)
	gb905_meter_timestamp_t swipe_timestamplit;			// 刷卡时间
	unsigned short terminal_status;						// ISU 状态
	unsigned char time_limit[5];						// 时间限制: YYYYMMDDhh, 全0   代表无限制
	unsigned char number_of_times_limit[2];				// 次数限制: 0000 表示无限制
	unsigned char result;								// 操作结果
}__packed gb905_meter_loginout_ack_body_t;

//计价器开机/  关机应答数据结构
typedef struct{
	gb905_peri_header_t header;
	gb905_meter_loginout_ack_body_t ack_body;
	gb905_peri_tail_t tail;
}__packed gb905_meter_loginout_ack_t;

#if 0
typedef struct{
	unsigned char name[12];
	unsigned char card_number[18];
	unsigned char business_type[48];
	unsigned char card_validity[8];
	unsigned char server_company[40];
	unsigned char star;
	unsigned char driver_number[6];
	unsigned char company_number[5];
	unsigned char return_result[2];
		
}__packed gb905_tsm_driver_info_t;
#endif

//计价器开机成功消息体数据结构(METER  -->  ISU)
typedef struct{
	unsigned char company_license_number[16];		// 公司经营许可证号(ASCII码,  不足16  字节，右补0)
	unsigned char driver_license_number[19];		// 驾驶员从业资格证号(ASCII码,  不足19  字节，右补0)
	unsigned char plate_number[6];					// 车牌号(ASCII 码)
	gb905_meter_timestamp_t open_timestamplit;	// 计价器开机时间
	unsigned int total_operation_number;			// 总营运次数
	unsigned char result;							// 操作结果
}__packed gb905_meter_open_success_body_t;


// 计价器关机成功消息体数据结构(METER  -->  ISU)
typedef struct{
	unsigned char company_license_number[16];		// 公司经营许可证号(ASCII码,  不足16  字节，右补0)
	unsigned char driver_license_number[19];		// 驾驶员从业资格证号(ASCII码,  不足19  字节，右补0)
	unsigned char plate_number[6];					// 车牌号(ASCII 码)
	
	unsigned char pulse[2];							// 计价器K   值
	gb905_meter_timestamp_t open_timestamplit;	// 当班开机时间
	gb905_meter_timestamp_t close_timestamplit;	// 当班关机时间
	
	unsigned char duty_mileage[3];					// 当班里程(xxxxx.x km)
	unsigned char driver_operation_km[3];			// 当班营运里程
	unsigned char driver_num[2];					// 车次
	unsigned char conut_time[3];					// 计时时间
	unsigned char total_money[3];					// 总计金额
	unsigned char card_money[3];					// 卡收金额
	unsigned char card_num[2];						// 卡次
	unsigned char night_km[2];						// 班间里程
	unsigned char total_km[4];						// 总计里程
	unsigned char total_operation_km[4];			// 总营运里程
	unsigned char unit_price[2];					// 单价
	unsigned int total_operation_num;				// 总营运次数
}__packed gb905_meter_close_success_body_t;


// 计价器永久时钟误差查询数据结构
typedef gb905_meter_query_state_t	gb905_meter_calibration_time_t;



// 计价器升级消息体数据结构
typedef struct{
	unsigned char vendor_id;
	unsigned char hw_version;
	unsigned char main_sw_version;
	unsigned char vice_sw_version;
}__packed gb905_meter_update_body_t;

// 计价器升级的完整数据结构
typedef struct{
	gb905_peri_header_t header;
	gb905_meter_update_body_t body;
	gb905_peri_tail_t tail;
}__packed gb905_meter_update_t;


//  计价器营运数据结构(METER  -->  ISU)
typedef struct{
	unsigned char plate_number[6];					// 车牌号(ASCII 码)
	unsigned char company_license_number[16];		// 公司经营许可证号(ASCII码,  不足16  字节，右补0)
	unsigned char driver_license_number[19];		// 驾驶员从业资格证号(ASCII码,  不足19  字节，右补0)
	unsigned char get_on_timestamp[5];				// 上车时间(BCD码YY-MM-DD-hh-mm)
	unsigned char get_off_timestamp[2];				// 下车时间(BCD码hh-mm)
	unsigned char distance[3];						// 计程公里(BCD码xxxxx.x km)
	unsigned char empty_distance[2];				// 空驶公里(BCD码xxx.x km)
	unsigned char surcharge[3];						// 附加费(BCD码xxxxx.x 元)
	unsigned char waiting_time[2];					// 等待计时时间(BCD码hh-mm)
	unsigned char transaction_amount[3];			// 交易金额(BCD码xxxxx.x 元)
	unsigned int  car_number;						// 当前车次	
	unsigned char transaction_type;					// 交易类型
}__packed gb905_meter_operation_t;


void gb905_meter_query_state(void);
void gb905_meter_query_parameter(void);
void gb905_meter_set_parameter(void);

bool gb905_meter_open_close_ack(void);
void gb905_meter_update(void);


int gb905_meter_protocol_ayalyze(unsigned char * buf,int len);

void gb905_meter_transparent(unsigned char *msg_buf,unsigned short msg_len);

void gb905_meter_heart_beat_treat(void);

#ifdef __cplusplus
}
#endif
	
#endif



