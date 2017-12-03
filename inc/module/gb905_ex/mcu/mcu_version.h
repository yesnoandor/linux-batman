/****************************************Copyright (c)**************************************************                         
** File name:			mcu_version.h
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

#ifndef _MCU_VERSION_H
#define	_MCU_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

void mcu_send_version_req(void);
unsigned char mcu_get_version_treat(unsigned char *buf,int len);

#ifdef __cplusplus
}
#endif

#endif

