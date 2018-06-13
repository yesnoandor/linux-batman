/****************************************Copyright (c)**************************************************                         
** File name:			gb905_report.h
** Descriptions:		 
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
#ifndef _GB905_REPORT_H
#define _GB905_REPORT_H

#ifdef __cplusplus
extern "C" {
#endif

// 报警标志定义
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned emergency:1;				// 紧急报警
		unsigned early_warning:1;			// 预警
		unsigned gps_fault:1;				// GPS 模块故障
		unsigned gps_antenna_cut_fault:1;	// GPS 天线未接或被剪断
		unsigned gps_antenna_short_fault:1;	// GPS 天线短路
		unsigned under_voltage:1;			// 电源欠压
		unsigned power_down:1;				// 电源掉电
		unsigned lcd_device_fault:1;		// 显示终端故障
		unsigned tts_fault:1;				// tts 模块故障
		unsigned camera_fault:1;			// 摄像头故障
		unsigned meter_fault:1;				// 计价器故障
		unsigned eval_fault:1;				// 服务评价器故障
		unsigned led_adver_fault:1;			// LED 广告屏故障
		unsigned lcd_module_fault:1;		// LCD 显示屏故障
		unsigned tsm_fault:1;				// 安全模块故障
		unsigned led_light_fault:1;			// LED 顶灯故障
		unsigned overspeed:1;				// 超速报警
		unsigned driving_timeout:1;			// 连续驾驶超时
		unsigned day_driving_timeout:1;		// 当天累计驾驶超时
		unsigned parking_timeout:1;			// 超时停车
		unsigned out_of_area:1;				// 进出区域/路线
		unsigned road_driving:1;			// 路段行驶时间不足/过长
		unsigned prohibited_area:1;			// 禁行路段行驶
		unsigned sersor_fault:1;			// 车速传感器故障
		unsigned illegal_ignition:1;		// 车辆非法点火
		unsigned illegal_displacement:1;	// 车辆非法位移
		unsigned abnormal_storage:1;		// 终端存储异常
		unsigned :0;
    }flag;
}report_alarm_t;


// 状态标志定义
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned fix:1;					// =0,已卫星定位;=1,未卫星定位
		unsigned lat:1;					// =0,北纬;=1,南纬
		unsigned lon:1;					// =0,东经;=1,西经
		unsigned operating:1;			// =0,营运状态;=1,停运状态
		unsigned reservation:1;			// =0,未预约;=1,预约
		unsigned weighting:1;			// =0,默认;=1, 空转重
		unsigned emptying:1;			// =0,默认;=1, 重转空
		unsigned reserved:1;
		unsigned acc:1;					// =0,ACC 关;=1,ACC 开
		unsigned loading:1;				// =0,空车;=1,重车
		unsigned oil:1;					// =0,车辆油路正常;=1,车辆油路断开
		unsigned circuit:1;				// =0,车辆电路正常;=1,车辆电路断开
		unsigned door_lock:1;			// =0,车门解锁;=1, 车门加锁
		unsigned car_lock:1;			// =0,车辆解锁;=1, 车辆锁定
		unsigned :0;
    }flag;
}report_status_t;



// 位置汇报的消息体信息数据格式
typedef struct {
	report_alarm_t	alarm;				// 报警标志
	report_status_t status;				// 状态
	
	unsigned int latitude;				// 纬度 (1/10000 分)
	unsigned int longitude;				// 经度 (1/10000 分)
	unsigned short speed;				// 速度 (1/10KM/H)
	unsigned char direction;			// 方向 (0~178,刻度=2  度,正北为0,顺时针)

	gb905_timestamp_t timestamp;		// 时间 (BCD码 YY-MM-DD-hh-mm-ss)
}__packed report_body_t;

// 位置汇报的整个消息数据格式
typedef  struct
{
	unsigned char start_magic_id;
	gb905_header_t header;
	report_body_t report_body;
	unsigned char xor;
	unsigned char end_magic_id;
} __packed gb905_report_t;

// 位置查询应答的消息体信息数据格式
typedef struct {
	unsigned short ack_serial_number;
	report_body_t report;
}__packed report_ack_body_t;

// 位置查询应答的整个消息数据格式
typedef  struct
{
    unsigned char start_magic_id;
    gb905_header_t header;
    report_ack_body_t report_ack_body;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed gb905_report_ack_t;



void gb905_report_send(void);
void gb905_report_ack_send(unsigned short seq_number);

void gb905_build_report_body(report_body_t * report_body);

#ifdef __cplusplus
}
#endif
	
#endif

