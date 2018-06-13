/****************************************Copyright (c)**************************************************                         
** File name:			mcu_gpio.h
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


#ifndef _MCU_GPIO_H
#define	_MCU_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

void mcu_gpo_output(int index,int level);

void mcu_send_gpio_out(unsigned int out);
unsigned char mcu_gpio_in_treat(unsigned char *buf,int len);
void mcu_gpio_map_req(void);
void mcu_gpio_map_treat(unsigned char *buf,int len);


#ifdef __cplusplus
}
#endif

#endif

