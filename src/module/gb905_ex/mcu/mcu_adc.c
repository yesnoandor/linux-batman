/****************************************Copyright (c)**************************************************                         
** File name:			mcu_adc.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-24
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

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905_ex/mcu/mcu_common.h"
#include    "module/gb905_ex/mcu/mcu_adc.h"

#include	"middleware/info/status.h"

#include	"app/report/fleety_report.h"


#define		DEBUG_Y
#include	"libs/debug.h"

#define UNDER_VOLTAGE_THRESHOLD_VALUE         11.5
#define POWER_DOWN_THRESHOLD_VALUE            9.0  

static double adc_value = 0;


typedef  struct
{
    unsigned char start_magic_id;
    mcu_protocol_header_t header;
	unsigned char channel;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed mcu_protocol_adc_t;


void mcu_send_adc_req(unsigned char channel)
{
	mcu_protocol_adc_t adc;

	DbgFuncEntry();
	
	mcu_protocol_build_header(&adc.header,SYSTEM2MCU_ADC_REQ,sizeof(channel));

	adc.channel = channel;

	mcu_protocol_send_data((unsigned char *)&adc,sizeof(mcu_protocol_adc_t));
		
	DbgFuncExit();
}

void mcu_get_adc_treat(unsigned char *buf,int len)
{
    unsigned short adc_in = 0;
    
	DbgFuncEntry();

	adc_in = *(unsigned short *)(&buf[1 + sizeof(mcu_protocol_header_t)]);

    adc_value = adc_in/1000.0;
    
	DbgPrintf("adc_in = %d\r\n",adc_in);
    DbgPrintf("adc_value = %f\r\n",adc_value);

	DbgFuncExit();
	
}

void gb905_adc_value_monitor_treat(void)
{
    taxi_status_t status;
    taxi_get_status(&status);
    
    //达到欠压报警的门限,没达到掉电的门限
    if((adc_value > POWER_DOWN_THRESHOLD_VALUE) &&(adc_value < UNDER_VOLTAGE_THRESHOLD_VALUE))
    {
        //如果不在欠压报警状态，则设置欠压报警状态
        if(!status.alarm.flag.under_voltage)
        {
            DbgError("terminal under voltage alarm!!!\r\n");
            clr_power_down_alarm_status();
            set_under_voltage_alarm_status();

            //状态变化，立即汇报位置信息
            fleety_report();
        }
    }
    else if(adc_value < POWER_DOWN_THRESHOLD_VALUE)
    {
        //gb905_power_off_alarm_treat();
    }
    else
    {
        //如果在掉电报警状态，则清除掉电报警状态
        if(status.alarm.flag.power_down)
        {
            clr_power_down_alarm_status();
        }
        //如果在欠压报警状态，则清除欠压报警状态
        if(status.alarm.flag.under_voltage)
        {
            clr_under_voltage_alarm_status();
        }
    }

}

void gb905_power_off_alarm_treat(void)
{
    taxi_status_t status;
    taxi_get_status(&status);

    DbgFuncEntry();
    
    //如果不在掉电报警状态，则设置掉电报警状态
    if(!status.alarm.flag.power_down)
    {
        DbgError("terminal power down alarm!!!\r\n");
        clr_under_voltage_alarm_status();
        set_power_down_alarm_status();
        
        //状态变化，立即汇报位置信息
        fleety_report();
    }

    DbgFuncExit();
    
}
