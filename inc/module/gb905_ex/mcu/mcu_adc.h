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

void mcu_send_adc_req(unsigned char channel);

void mcu_get_adc_treat(unsigned char *buf,int len);

#ifdef __cplusplus
}
#endif


#endif
