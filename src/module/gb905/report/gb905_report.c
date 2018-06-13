/****************************************Copyright (c)**************************************************                         
** File name:			gb905_report.c
** Descriptions:		位置汇报(0x0200)  和位置信息查询(0x8201/0x0201) 的协议解析
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-04
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#include	"common.h"

#include	"misc/endian.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"

#include	"middleware/info/gps.h"
#include	"middleware/info/status.h"
#include	"middleware/socket/fleety_socket.h"

#include	"middleware/db/sqlite3/report_sqlite3.h"

#include	"app/record/fleety_record.h"


#define		DEBUG_Y
#include	"libs/debug.h"


/** 
* @brief 	构造位置汇报的数据结构
* @param report 位置汇报数据结构地址
*
*/
void gb905_build_report_body(report_body_t * report_body)
{
	gps_info_t gps_info;
	taxi_status_t taxi_status;
	
	DbgFuncEntry();
	
	taxi_get_status(&taxi_status);
	gps_get_info(&gps_info);

	// 报警标志
	report_body->alarm.whole = 0;
	report_body->alarm.whole = EndianReverse32(taxi_status.alarm.whole);
	//report_body->alarm.flag.emergency = taxi_status.hw.flag.alarm;
	//report_body->alarm.whole = EndianReverse32(report_body->alarm.whole);

	// 状态标志
	report_body->status.whole = 0;

	report_body->status.flag.fix = gps_info.fixed;
	report_body->status.flag.lat = gps_info.latitude < 0 ? 1 : 0; 	// 南纬是负，北纬是正
	report_body->status.flag.lon = gps_info.longitude < 0 ? 1 : 0;	// 东经是正，西经是负
	report_body->status.flag.weighting = taxi_status.status.flag.weighting;
	report_body->status.flag.emptying = taxi_status.status.flag.emptying;
	report_body->status.flag.acc = taxi_status.hw.flag.acc;
	report_body->status.flag.loading = taxi_status.hw.flag.loading;
	report_body->status.flag.oil = taxi_status.hw.flag.oil;
	report_body->status.flag.circuit = taxi_status.hw.flag.circuit;
	report_body->status.flag.door_lock = taxi_status.hw.flag.door_lock;
	report_body->status.flag.car_lock = taxi_status.hw.flag.car_lock;
	
	report_body->status.whole = EndianReverse32(report_body->status.whole);

	// 位置信息
	report_body->latitude = EndianReverse32(gps_info.latitude);
	report_body->longitude = EndianReverse32(gps_info.longitude);
	report_body->speed = EndianReverse16(gps_info.speed);
	report_body->direction = gps_info.direction;
	gb905_build_timestamp(&(report_body->timestamp));
	
	DbgFuncExit();
}

/** 
* @brief 	构造整个位置汇报的数据结构
* @param gb905_report 		整个位置汇报数据结构地址
*
*/
void gb905_build_report(gb905_report_t * gb905_report)
{
	gb905_build_header(&gb905_report->header,MESSAGE_LOCATION_REPORT,sizeof(report_body_t));
#ifdef DEBUG_Y
	gb905_debug_header(&gb905_report->header);
#endif

	gb905_build_report_body(&gb905_report->report_body);
}

/** 
* @brief 	发送位置汇报数据
* @param gb905_report 		整个位置汇报数据结构地址
*
*/
void gb905_report_send(void)
{
	gb905_report_t gb905_report;

	DbgFuncEntry();

	gb905_build_report(&gb905_report);

	//release_alarm_after_report_ack(&gb905_location_report.header);
	
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&gb905_report,sizeof(gb905_report_t));

	gb905_record_msg(MAIN_SOCKET,&gb905_report.header,(unsigned char *)&gb905_report,sizeof(gb905_report_t));

	report_sqlite3_insert_record(EndianReverse16(gb905_report.header.msg_serial_number),(unsigned char *)&gb905_report,sizeof(gb905_report_t));

	DbgFuncExit();
}

/** 
* @brief 	构造位置查询应答的数据结构
* @param gb905_report_ack 	位置查询应答的数据结构地址
*
*/
void gb905_build_report_ack_body(report_ack_body_t * report_ack_body,unsigned short serial_number)
{
	DbgFuncEntry();
	
	report_ack_body->ack_serial_number = serial_number;
	gb905_build_report_body(&report_ack_body->report);
	
	DbgFuncExit();
}

/** 
* @brief 	构造整个位置查询应答的数据结构
* @param gb905_report 		整个位置汇报数据结构地址
*
*/
void gb905_build_report_ack(gb905_report_ack_t * gb905_report_ack,unsigned short seq_number)
{
	gb905_build_header(&gb905_report_ack->header,MESSAGE_GET_LOCATION_ACK, sizeof(report_ack_body_t));
#ifdef DEBUG_Y		
	gb905_debug_header(&gb905_report_ack->header);
#endif

	gb905_build_report_ack_body(&gb905_report_ack->report_ack_body,seq_number);
}


/** 
* @brief 	构造位置查询应答的数据结构
* @param gb905_report_ack 	位置查询应答的数据结构地址
*
*/
void gb905_report_ack_send(unsigned short seq_number)
{
	gb905_report_ack_t  gb905_report_ack;
	
	DbgFuncEntry();

	gb905_build_report_ack(&gb905_report_ack,seq_number);
		
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&gb905_report_ack,sizeof(gb905_report_ack_t));
	
	DbgFuncExit();
}

