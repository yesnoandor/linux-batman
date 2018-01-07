/****************************************Copyright (c)**************************************************                         
** File name:			platform.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-25
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

#include	"platform/hw/hw_system.h"

#include	"module/gb905_ex/mcu/mcu_common.h"
#include	"module/gb905_ex/ui/ui_common.h"

#include	"module/gps/gps_nmea.h"
#include	"module/gprs/gprs_at_cmd.h"

#define		DEBUG_Y
#include	"libs/debug.h"

/** 
* @brief 	硬件平台初始化(不同平台需要初始化不同的硬件)
*
*/
void platform_hardware_init(void)
{
#if defined (PLATFORM_UBUNTU)
	ubuntu_keys_simu_init();
	ubuntu_imei_simu_init();
#endif

#if defined (PLATFORM_QCOM)
	ui_protocol_init();
	qcom_keys_init();
#endif


#if defined (PLATFORM_HW)
	ui_protocol_init();
	mcu_protocol_init();
	gps_nmea_init();
	gprs_at_protocol_init();
#endif 

#if defined (PLATFORM_MTK)
#endif		
}

/** 
* @brief 	硬件平台初始化(不同平台需要初始化不同的硬件)
*
*/
void platform_hardware_post_init(void)
{
#if defined (PLATFORM_UBUNTU)

#endif

#if defined (PLATFORM_QCOM)
	ui_imei_info_req();
#endif


#if defined (PLATFORM_HW)
	gprs_get_imei();
#endif

#if defined (PLATFORM_MTK)
#endif		
}

/** 
* @brief 	硬件平台的寄电器输出
*
*/
void platform_hardware_relay_output(int index,int level)
{
#if defined (PLATFORM_UBUNTU)
	ubuntu_relay_simu_output(index,level);
#endif

#if defined (PLATFORM_QCOM)

#endif


#if defined (PLATFORM_HW)

#endif 

#if defined (PLATFORM_MTK)

#endif	

}


/** 
* @brief 	硬件平台的关机
*
*/
void platform_system_power_off(void)
{
#if defined (PLATFORM_UBUNTU)
	ubuntu_system_power_off();
#endif

#if defined (PLATFORM_QCOM)
	qcom_system_power_off();
#endif


#if defined (PLATFORM_HW)
	hw_system_power_off();
#endif 

#if defined (PLATFORM_MTK)

#endif	
}

/** 
* @brief 	硬件平台的重启
*
*/
void platform_system_reset(void)
{
#if defined (PLATFORM_UBUNTU)
	ubuntu_system_reset();
#endif

#if defined (PLATFORM_QCOM)
	qcom_system_reset();
#endif


#if defined (PLATFORM_HW)
	hw_system_reset();
#endif 

#if defined (PLATFORM_MTK)

#endif	
}

/** 
* @brief 	硬件平台的恢复出厂设置
*
*/
void platform_system_recovery(void)
{
#if defined (PLATFORM_UBUNTU)
	ubuntu_system_recovery();
#endif

#if defined (PLATFORM_QCOM)
	qcom_system_recovery();
#endif


#if defined (PLATFORM_HW)
	hw_system_recovery();
#endif 

#if defined (PLATFORM_MTK)

#endif	
}