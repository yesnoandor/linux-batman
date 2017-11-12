#ifndef _GB905_TRACE_H
#define	_GB905_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

enum{
	LOCATION_TRACE_TIME_POLICY = 0x00,
	LOCATION_TRACE_DISTANCE_POLICY = 0x11,
	LOCATION_TRACE_TIME_DISTANCE_POLICY = 0x01,
	LOCATION_TRACE_DISTANCE_TIME_POLICY = 0x10,
	LOCATION_TRACE_STOP_POLICY = 0xFF,
};

typedef struct 
{	
	// 跟踪策略
	// 		= 0x00,时间间隔，持续时间
	//		= 0x11,距离间隔，持续距离
	//		= 0x01,时间间隔，持续距离
	//		= 0x10,距离间隔，持续时间
	//		= 0xFF,停止当前跟踪
	unsigned char policy;
	unsigned short interval;		// 时间(s) /距离间隔(m)
	unsigned int threhold;			// 持续时间(s) /间隔(m)
}__packed location_trace_params_t;

// 描述
typedef struct 
{
	// =True,位置跟踪启动
	// =False,位置跟踪停止
	bool location_trace_flag;

	// 位置跟踪的参数
	location_trace_params_t location_trace_params;
}location_trace_t;

