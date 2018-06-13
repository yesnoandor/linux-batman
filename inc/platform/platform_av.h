/****************************************Copyright (c)**************************************************                         
** File name:			platform_av.h
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


#ifndef _PLATFORM_AV_H
#define	_PLATFORM_AV_H

#ifdef __cplusplus
extern "C" {
#endif


void platform_phoho_take_picture(char * path,unsigned char camera_id,photo_params_t *photo_params);
int platform_get_camera_status(void);

#ifdef __cplusplus
}
#endif


#endif

