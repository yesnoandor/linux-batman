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

/** 
* @brief 	Ӳ��ƽ̨��ʼ��(��ͬƽ̨��Ҫ��ʼ����ͬ��Ӳ��)
*
*/
void platform_hardware_init(void)
{
#if defined (PLATFORM_UBUNTU)
	ubuntu_keys_simu_init();
	ubuntu_imei_simu_init();
#endif

#if defined (PLATFORM_QCOM)
#endif


#if defined (PLATFORM_HW)
	mcu_protocol_init();
#endif 

#if defined (PLATFORM_MTK)
#endif		
}


/** 
* @brief 	Ӳ��ƽ̨�ļĵ������
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



