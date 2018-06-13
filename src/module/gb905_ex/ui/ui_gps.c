/****************************************Copyright (c)**************************************************                         
** File name:			ui_gps.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-01
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

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_gps.h"


#include	"middleware/info/gps.h"

#define		DEBUG_Y
#include	"libs/debug.h"




unsigned char ui_gps_info_treat(unsigned char *buf,unsigned int len)
{
	gps_info_t *gsp_info;

	DbgFuncEntry();
	
	gsp_info = (gps_info_t *)buf;
	
	gps_set_info(gsp_info);
	gps_debug_info();
	
	DbgFuncExit();

	return GB905_EX_RESULT_OK;
}


