/****************************************Copyright (c)**************************************************                         
** File name:			platform_av.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-02-03
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
#include    "libs/sdk/sdk_data_def.h"
#include    "libs/sdk/gps_main_server.h"

#include	"middleware/info/av.h"

#define		DEBUG_Y
#include	"libs/debug.h"


void platform_phoho_take_picture(char * path,unsigned char camera_id,photo_params_t *photo_params)
{
    DbgFuncEntry();
    
    #if defined (PLATFORM_UBUNTU)
    
    #endif
        	
    #if defined (PLATFORM_QCOM)

    #endif
        	
        	
    #if defined (PLATFORM_HW)
    Control_CreateMultimediaFile(MULTIMEDIAFILE_PIC,PIC_QVGA,path,(int)camera_id,0);
    #endif 

    #if defined (PLATFORM_MTK)
        	
    #endif	

    DbgFuncExit();
    
}

int platform_get_camera_status(void)
{
    int camera_status = 0;
    
    DbgFuncEntry();
    
    #if defined (PLATFORM_UBUNTU)
    
    #endif
        	
    #if defined (PLATFORM_QCOM)

    #endif
        	
        	
    #if defined (PLATFORM_HW)
    camera_status = get_camera_status();
    #endif 

    #if defined (PLATFORM_MTK)
        	
    #endif	

    DbgFuncExit();
    
    return camera_status;
}
