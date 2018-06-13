/****************************************Copyright (c)**************************************************                         
** File name:			driver.h
** Descriptions:		驾驶员信息(有TSM模块获取)
*
*
*
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-26
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/


#ifndef _DRIVER_H
#define _DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

void set_driver_info(unsigned char *buf);
void get_driver_info(unsigned char *buf);

#ifdef __cplusplus
}
#endif
	
#endif

