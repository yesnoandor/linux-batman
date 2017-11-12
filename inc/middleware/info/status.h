/****************************************Copyright (c)**************************************************                         
** File name:			status.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-12
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _STATUS_H
#define _STATUS_H


#ifdef __cplusplus
extern "C" {
#endif

// 软件状态定义
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned login:1;				// =0,登出 ;=1,登陆
		unsigned :0;
    }flag;
}sw_status_t;

// 硬件状态定义
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned acc:1;					// =0,ACC 关;=1,ACC 开
		unsigned loading:1;				// =0,空车 ;=1,重车
		unsigned :0;
    }flag;
}hw_status_t;




// 模组状态定义
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned meter:1;				// =0,unconnected;	=1,connected
		unsigned toplight:1;			// =0,unconnected;	=1,connected
		unsigned tsm:1;					// =0,unconnected;	=1,connected
		unsigned :0;
    }flag;
}module_status_t;


// 
typedef struct {
	sw_status_t	sw;				
	hw_status_t hw;				
	module_status_t module;
	report_alarm_t alarm;
}taxi_status_t;

void taxi_get_status(taxi_status_t * status);

#ifdef __cplusplus
}
#endif

#endif

