/****************************************Copyright (c)**************************************************                         
** File name:			platform.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-11-25
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/


#ifndef _PLATFORM_H
#define	_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined (PLATFORM_UBUNTU)
#include	"platform/ubuntu/ubuntu.h"
#elif defined (PLATFORM_QCOM)
#include	"platform/qcom/qcom.h"
#elif defined (PLATFORM_HW)
#include	"platform/hw/hw.h"
#elif defined (PLATFORM_MTK)
#include	"platform/mtk/mtk.h"
#else
#error "No Support This Platform"
#endif

void platform_hardware_init(void);
void platform_hardware_post_init(void);

void platform_hardware_relay_output(int index,int level);

void platform_system_power_off(void);
void platform_system_reset(void);
void platform_system_recovery(void);

#ifdef __cplusplus
}
#endif


#endif

