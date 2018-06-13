/****************************************Copyright (c)**************************************************                         
** File name:			fleety_photo.h
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

#ifndef _FLEETY_PHOTO_H
#define	_FLEETY_PHOTO_H

#ifdef __cplusplus
extern "C" {
#endif
#define CAMERA_CHECK_TIMEOUT     30

void fleety_photo_start(void);
void fleety_photo_stop(void);
void fleety_photo_take_picture_treat(unsigned char reason);
void fleety_camera_fault_treat(void);


#ifdef __cplusplus
}
#endif

#endif


