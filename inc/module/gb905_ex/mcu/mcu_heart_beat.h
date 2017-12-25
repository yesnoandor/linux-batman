/****************************************Copyright (c)**************************************************                         
** File name:			mcu_heart_beat.h
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




#ifndef _MCU_HEART_BEAT_H
#define	_MCU_HEART_BEAT_H

#ifdef __cplusplus
extern "C" {
#endif

#define		MCU_HEART_BEAT_TIMEOUT		60

void mcu_heart_beat_reset(void);
void mcu_heart_beat_send(void);
void mcu_heart_beat_treat(void);



#ifdef __cplusplus
}
#endif

#endif

