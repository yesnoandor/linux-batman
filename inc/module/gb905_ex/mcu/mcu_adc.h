/****************************************Copyright (c)**************************************************                         
** File name:			mcu_adc.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-11-29
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/


#ifndef _MCU_ADC_H
#define	_MCU_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#define ADC_REP_TIMEOUT         10

enum{
    ADC_CHANNEL_VERSION = 0,
    ADC_CHANNEL_BATTERY,
    ADC_CHANNEL_ACC,
    ADC_CHANNEL_TEMPERATURE
};

void mcu_send_adc_req(unsigned char channel);

void mcu_get_adc_treat(unsigned char *buf,int len);

void gb905_adc_value_monitor_treat(void);

void gb905_power_off_alarm_treat(void);

#ifdef __cplusplus
}
#endif


#endif
