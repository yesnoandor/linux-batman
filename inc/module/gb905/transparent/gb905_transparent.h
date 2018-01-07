/****************************************Copyright (c)**************************************************                         
** File name:			gb905_transparent.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-02
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_TRANSPARENT_H
#define _GB905_TRANSPARENT_H
	
#ifdef __cplusplus
extern "C" {
#endif


void gb905_transparent_download_treat(unsigned char *buf,int len);

void gb905_transparent_upload_treat(unsigned char dev_id, unsigned char factory_id, unsigned short cmd_id, unsigned char* buf, unsigned short len);


#ifdef __cplusplus
}
#endif
	
#endif

