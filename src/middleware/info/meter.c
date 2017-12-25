/****************************************Copyright (c)**************************************************                         
** File name:			operation.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-14
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



#include	"module/gb905_peri/meter/gb905_meter.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static gb905_meter_operation_t meter_operation_info;

static gb905_meter_open_success_body_t meter_open_success_info;
static gb905_meter_close_success_body_t meter_close_success_info;


void get_meter_operation_info(char * meter_operation)
{
	DbgFuncEntry();
	
	memcpy(meter_operation,&meter_operation_info,sizeof(gb905_meter_operation_t));
	
	DbgFuncExit();
}

void set_meter_operation_info(char * meter_operation)
{
	DbgFuncEntry();
	
	memcpy(&meter_operation_info,meter_operation,sizeof(gb905_meter_operation_t));
	
	DbgFuncExit();
}


void get_meter_open_success_info(char * meter_open)
{
	DbgFuncEntry();
	
	memcpy(meter_open,&meter_open_success_info,sizeof(gb905_meter_open_success_body_t));
	
	DbgFuncExit();
}

void set_meter_open_success_info(char * meter_open)
{
	DbgFuncEntry();
	
	memcpy(&meter_open_success_info,meter_open,sizeof(gb905_meter_open_success_body_t));
	
	DbgFuncExit();
}

void get_meter_close_success_info(char * meter_close)
{
	DbgFuncEntry();
	
	memcpy(meter_close,&meter_close_success_info,sizeof(gb905_meter_close_success_body_t));
	
	DbgFuncExit();
}

void set_meter_close_success_info(char * meter_close)
{
	DbgFuncEntry();
	
	memcpy(&meter_close_success_info,meter_close,sizeof(gb905_meter_close_success_body_t));
	
	DbgFuncExit();
}

